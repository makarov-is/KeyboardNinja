#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE  
#endif

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include <locale.h>
#include <string>

#include "FileIO.hpp"
#include "Shapes.hpp"
#include "Keyboard.hpp"
#include "Image.hpp"

ID2D1Factory *factory;
ID2D1HwndRenderTarget *renderTarget;

ID2D1SolidColorBrush *brush;
ID2D1SolidColorBrush *whiteBrush;

IDWriteFactory *writeFactory;
IDWriteTextFormat *textFormat;
IDWriteTextFormat *statsTextFormat;
IDWriteTextFormat *keyboardTextFormat;

// NOTE: text layout
IDWriteTextLayout *textLayout;
D2D1_RECT_F textLayoutRect;

// NOTE: rectangles (areas)
D2D1_ROUNDED_RECT textRect;
D2D1_ROUNDED_RECT statsRect;
D2D1_RECT_F shadeRect;

// NOTE: Keyboard object
Keyboard keyboard;

// NOTE: text
#define BUFFER_SIZE 2048
WCHAR *textBuffer = 0;
UINT textLength;

UINT bufferIndex = 0; // NOTE: current character index
UINT previousBufferIndex = 0; // NOTE: last typed character index
D2D1_RECT_F cursorRect = {0};

// NOTE: time
DWORD startTime = 0;
DWORD finishTime = 0;
DWORD resultTime = 0;

UINT currentTypingSpeed;
UINT onScreenSpeed;

// NOTE: accuracy
UINT typedChars = 0;
UINT typos = 0;
float accuracy = 100.0f;
bool misprint = false;

// NOTE: flags
bool typingBegan = false;
bool pauseMode = false;

// NOTE: colors
D2D1_COLOR_F cursorFillColorGreen = D2D1::ColorF(0x5BC538);
D2D1_COLOR_F cursorFillColorRed = D2D1::ColorF(0xF36707);
D2D1_COLOR_F cursorFillColor = cursorFillColorGreen;
D2D1_COLOR_F colorBlack = D2D1::ColorF(0x000000);
D2D1_COLOR_F colorWhite = D2D1::ColorF(0xFFFFFF);
D2D1_COLOR_F backgroundColor = D2D1::ColorF(0x55C5FF);

// NOTE: images
IWICImagingFactory *imagingFactory;
Image speedometerIcon;
Image accuracyIcon;

//===============================================================

HRESULT initGraphicsResources(HWND window)
{
	HRESULT result = S_OK;

	if(!renderTarget)
	{
		RECT clientRect;
		GetClientRect(window, &clientRect);
		D2D1_SIZE_U renderTargetSize = D2D1::SizeU(clientRect.right, clientRect.bottom);

		result = factory->CreateHwndRenderTarget(D2D1::RenderTargetProperties(),
			D2D1::HwndRenderTargetProperties(window, renderTargetSize), &renderTarget);
		if(SUCCEEDED(result))
		{
			result = renderTarget->CreateSolidColorBrush(colorBlack, &brush);
		}
	}

	if(!textFormat)
	{
		result = writeFactory->CreateTextFormat(L"Open Sans", 0, DWRITE_FONT_WEIGHT_REGULAR, 
		                               			DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		                               			21.0f, L"en-us", &textFormat);

		result = writeFactory->CreateTextFormat(L"Open Sans", 0, DWRITE_FONT_WEIGHT_REGULAR, 
		                                       DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 
		                                       18.0f, L"en-us", &statsTextFormat);

		result = writeFactory->CreateTextFormat(L"Open Sans", 0, DWRITE_FONT_WEIGHT_REGULAR, 
		                                        DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, 
		                                        18.0f, L"en-us", &keyboardTextFormat);
	}

	// NOTE: Creating bitmaps
	speedometerIcon.setRect(1030, 43, 20, 20);
	accuracyIcon.setRect(1025, 168, 25, 25);
	result = speedometerIcon.createBitmap(L"images\\speedometer.png", &renderTarget, &imagingFactory);
	result = accuracyIcon.createBitmap(L"images\\accuracy.png", &renderTarget, &imagingFactory);

	keyboard.init(&keyboardTextFormat);

	return(result);
}

void drawCursor(HWND windowHandle, D2D1_RECT_F layoutRect)
{
	float cursorX;
	float cursorY;
	DWRITE_HIT_TEST_METRICS cursorMetrics;
	textLayout->HitTestTextPosition(bufferIndex, 0, &cursorX, &cursorY, &cursorMetrics);

	cursorRect.left = layoutRect.left + cursorX;
	cursorRect.right = cursorRect.left + cursorMetrics.width;
	cursorRect.top = layoutRect.top + cursorY;
	cursorRect.bottom = cursorRect.top + cursorMetrics.height;

	D2D1_ROUNDED_RECT cursorRectR;
	cursorRectR.rect = cursorRect;
	cursorRectR.radiusX = 1.0f;
	cursorRectR.radiusY = 1.0f;

	brush->SetColor(cursorFillColor);
	renderTarget->FillRoundedRectangle(&cursorRectR, brush);
}

void discardTextLayout(IDWriteTextLayout **textLayout)
{
	(*textLayout)->Release();
    (*textLayout) = 0;
}

HRESULT onPaint(HWND windowHandle)
{
	HRESULT hr;

	if(!textLayout)
	{
		textLayoutRect.left = 120;
		textLayoutRect.right = 1000;
		textLayoutRect.top = 40;
		textLayoutRect.bottom = 320;
		writeFactory->CreateTextLayout(textBuffer, BUFFER_SIZE, textFormat,
		                               textLayoutRect.right - textLayoutRect.left,
		                               textLayoutRect.bottom - textLayoutRect.top,
		                               &textLayout);
	}

	renderTarget->BeginDraw();

	// NOTE: clear background
	renderTarget->Clear(backgroundColor);

	// NOTE: draw text area rectangle
	brush->SetColor(colorWhite);
	renderTarget->FillRoundedRectangle(&textRect, brush);

	// NOTE: drawing text cursor
	drawCursor(windowHandle, textLayoutRect);

	// NOTE: highlighting current character with white color
    if(!whiteBrush) renderTarget->CreateSolidColorBrush(colorWhite, &whiteBrush);
    DWRITE_TEXT_RANGE currentCursorPosition = {bufferIndex, 1};
    hr = textLayout->SetDrawingEffect(whiteBrush, currentCursorPosition);

    // NOTE: repainting last typed characters (black)
    UINT chars = 4;
    if(bufferIndex > (chars - 1))
    {
    	DWRITE_TEXT_RANGE previousCursorPosition = {bufferIndex - chars, chars};
    	DWRITE_TEXT_RANGE previousCharacter = {previousBufferIndex, 1};
    	brush->SetColor(colorBlack);
    	textLayout->SetDrawingEffect(brush, previousCursorPosition);
    	textLayout->SetDrawingEffect(brush, previousCharacter);
    }
    else
    {
    	if(typingBegan)
    	{
    		DWRITE_TEXT_RANGE previousCharacter = {previousBufferIndex, 1};
    		brush->SetColor(colorBlack);
    		textLayout->SetDrawingEffect(brush, previousCharacter);
    	}
    }

	// NOTE: draw text
	brush->SetColor(colorBlack);
	renderTarget->DrawTextLayout(D2D1::Point2F(textLayoutRect.left, textLayoutRect.top),
	                             textLayout, brush);

	// NOTE: drawing keyboard
	keyboard.drawKeyboard(&brush, &renderTarget);

	// NOTE: drawing shade rect prior to stats for highlighting
	if(pauseMode)
    {
    	brush->SetColor(D2D1::ColorF(0x000000, 0.3f));
		renderTarget->FillRectangle(&shadeRect, brush);
    }
    brush->SetColor(colorWhite);
    renderTarget->FillRoundedRectangle(&statsRect, brush);

	brush->SetColor(D2D1::ColorF(0xBCBCC2));
    renderTarget->DrawText(L"Скорость:", 9, statsTextFormat, rectAt(1055, 40, 100, 10), brush);
    renderTarget->DrawText(L"Точность:", 9, statsTextFormat, rectAt(1055, 170, 100, 10), brush);

    std::wstring speedStr = std::to_wstring((UINT)onScreenSpeed);
    renderTarget->DrawText(speedStr.c_str(), speedStr.length(), statsTextFormat, rectAt(1055, 60, 100, 10), brush);

    std::wstring accuracyStr = std::to_wstring(accuracy);
    renderTarget->DrawText(accuracyStr.c_str(), 5, statsTextFormat, rectAt(1055, 190, 100, 10), brush);

    // NOTE: Drawing bitmap
	renderTarget->DrawBitmap(speedometerIcon.bitmap, speedometerIcon.bitmapRect);
	renderTarget->DrawBitmap(accuracyIcon.bitmap, accuracyIcon.bitmapRect);

	renderTarget->EndDraw();

	return(hr);
}

void readRandomFile(UINT layout)
{
	WCHAR *filename = L"texts\\ru";
#if 1
	if(layout == LAYOUT_RU)
	{
		filename = L"texts\\ru";
	}
	else if(layout == LAYOUT_ENG)
	{
		filename = L"texts\\en";
	}
#endif

	std::wstring name(filename);

	WCHAR buffer[5];
	int randNumber = rand() % 10;
	WCHAR *txt = _itow(randNumber, buffer, 10);
	name += std::wstring(txt);
	name += std::wstring(L".txt");

	readFile(name.c_str(), &textBuffer, BUFFER_SIZE, &textLength, &bufferIndex);
}

void restart()
{
	bufferIndex = 0;
	typedChars = 0;

	WCHAR charCopy = textBuffer[bufferIndex];
	keyboard.findKeyOnBoard(_wcsupr(&charCopy));

	currentTypingSpeed = 0;
	onScreenSpeed = 0;

	pauseMode = false;
	keyboard.borderVisible = true;

	accuracy = 100.0f;
	typos = 0;

	cursorFillColor = cursorFillColorGreen;
}

LRESULT CALLBACK KNWindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	HRESULT hr = 0;

	switch(message)
	{
		case WM_CREATE:
		{
			// NOTE: ImagingFactory
			hr = CoInitialize(0);
			hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, 
                   				  __uuidof(IWICImagingFactory), (void **)(&imagingFactory));

			// NOTE: WriteFactory
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory);

			hr = initGraphicsResources(windowHandle);

			if(hr != S_OK) return(-1);
		} break;

		case WM_PAINT:
		{
			hr = onPaint(windowHandle);
		} break;

		case WM_CHAR:
		{
			WCHAR inputChar = (WCHAR)wParam;
			WCHAR c = textBuffer[bufferIndex];
			if(inputChar == c)
			{
				if(!typingBegan)
				{
					typingBegan = true;

					startTime = timeGetTime();
				}

				misprint = false;
				cursorFillColor = cursorFillColorGreen;

				previousBufferIndex = bufferIndex;
				++bufferIndex;
				++typedChars;

				WCHAR charCopy = textBuffer[bufferIndex];
				keyboard.findKeyOnBoard(_wcsupr(&charCopy));

				float timePassedInSeconds = (timeGetTime() - startTime) / 1000.0f;
				currentTypingSpeed = (UINT)(typedChars / timePassedInSeconds * 60.0f);

				if(bufferIndex % 5 == 0)
				{
					onScreenSpeed = currentTypingSpeed;
				}

				if(bufferIndex == textLength)
				{
					typingBegan = false;
					finishTime = timeGetTime();
					resultTime = finishTime - startTime;

					onScreenSpeed = currentTypingSpeed;

					keyboard.borderVisible = false;
					pauseMode = true;
				}
			}
			else
			{
				if(typingBegan)
				{
					cursorFillColor = cursorFillColorRed;

					if(!misprint)
					{
						misprint = true;

						++typos;
						accuracy = 100.0f - ((float)typos / (float)textLength * 100.0f);
					}
				}
			}

		} break;

		case WM_KEYDOWN:
		case WM_KEYUP:
		{
			UINT vkCode = (UINT)wParam;
			bool isDown = ((lParam & (1 << 31)) == 0);

			if(pauseMode)
			{
				if(vkCode == VK_RETURN && isDown)
				{
					discardTextLayout(&textLayout);
					readRandomFile(keyboard.currentKeyboardLayout);

					restart();
				}

				if(vkCode == VK_SPACE && isDown)
				{
					restart();
				}
			}

			if(vkCode == VK_F1 && isDown)
			{
				keyboard.switchLayout();

				discardTextLayout(&textLayout);
				readRandomFile(keyboard.currentKeyboardLayout);
				restart();
			}

			if(vkCode == VK_F5 && isDown)
			{
				previousBufferIndex = bufferIndex;
				bufferIndex =  textLength - 2;

				cursorFillColor = cursorFillColorGreen;

				wchar_t charCopy = textBuffer[bufferIndex];
				keyboard.findKeyOnBoard(_wcsupr(&charCopy));
			}
		} break;

		case WM_CLOSE:
		{
			DestroyWindow(windowHandle);
		} break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;

		default:
		{
			result = DefWindowProc(windowHandle, message, wParam, lParam);
		} break;
	}

	return(result);
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	// NOTE: for _wcsupr() function
	setlocale(LC_ALL, "Russian");

	WCHAR *windowClassName = L"KNWindow";

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = KNWindowProc;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = windowClassName;

	RegisterClass(&windowClass);

	RECT desiredRect = {0};
	desiredRect.right = 1280;
	desiredRect.bottom = 720;
	AdjustWindowRect(&desiredRect, WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, false);

	HWND window = CreateWindow(
		windowClassName, L"KeyboardNinja", WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, 
		CW_USEDEFAULT, CW_USEDEFAULT, 
		desiredRect.right - desiredRect.left, 
		desiredRect.bottom - desiredRect.top, 
		0, 0, instance, 0
	);

	shadeRect = rectAt(0, 0, 1280, 720);

	// NOTE: drawing areas
	textRect = roundedRectAt(110, 30, 900, 300, 10);
	statsRect = roundedRectAt(110 + 900 + 10, 30, 140, 300, 10);

	//NOTE: initialize textBuffer
	textBuffer = (WCHAR *)calloc(BUFFER_SIZE, sizeof(WCHAR));

	// NOTE: loading text file
	WCHAR *filename = L"texts\\ru.txt";
	readFile(filename, &textBuffer, BUFFER_SIZE, &textLength, &bufferIndex);

	// NOTE: initialize all variables
	restart();

	// NOTE: show window on screen
	ShowWindow(window, showCmd);

	MSG msg = {};
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(0);
}
