#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE  
#endif

#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include "FileIO.hpp"
#include "Shapes.hpp"
#include "Keyboard.hpp"

ID2D1Factory *factory;
ID2D1HwndRenderTarget *renderTarget;
ID2D1SolidColorBrush *brush;
ID2D1SolidColorBrush *whiteBrush;

IDWriteFactory *writeFactory;
IDWriteTextFormat *textFormat;
IDWriteTextLayout *textLayout;

// NOTE: text layout
D2D1_RECT_F textLayoutRect;

// NOTE: rectangles (areas)
D2D1_ROUNDED_RECT textRect;

// NOTE: Keyboard object
Keyboard keyboard;

// NOTE: text
#define BUFFER_SIZE 2048
WCHAR *textBuffer = 0;

UINT bufferIndex = 0; // NOTE: current character index
UINT previousBufferIndex = 0; // NOTE: last typed character index
D2D1_RECT_F cursorRect = {0};

// NOTE: flags
bool typingBegan = false;

// NOTE: colors
D2D1_COLOR_F cursorFillColorGreen = D2D1::ColorF(0x5BC538);
D2D1_COLOR_F cursorFillColorRed = D2D1::ColorF(0xF36707);
D2D1_COLOR_F cursorFillColor = cursorFillColorGreen;
D2D1_COLOR_F colorBlack = D2D1::ColorF(0x000000);
D2D1_COLOR_F colorWhite = D2D1::ColorF(0xFFFFFF);
D2D1_COLOR_F backgroundColor = D2D1::ColorF(0x55C5FF);

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
		                                         D2D1::HwndRenderTargetProperties(window, renderTargetSize), 
		                                         &renderTarget);
		if(SUCCEEDED(result))
		{
			D2D1_COLOR_F brushColor = D2D1::ColorF(0.0f, 0.0f, 0.0f);
			result = renderTarget->CreateSolidColorBrush(brushColor, &brush);

			textRect = D2D1::RoundedRect(D2D1::RectF(190.0f, 30.0f, 1090.0f, 330.0f), 10.0f, 10.0f);
		}
	}

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

HRESULT onPaint(HWND windowHandle)
{
	HRESULT hr;

	if(!textFormat)
	{
		hr = writeFactory->CreateTextFormat(L"Open Sans", 0, DWRITE_FONT_WEIGHT_REGULAR, 
		                               		DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,
		                               		21.0f, L"en-us", &textFormat);
	}

	if(!textLayout)
	{
		textLayoutRect.left = 200;
		textLayoutRect.right = 1080;
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

	renderTarget->EndDraw();

	return(hr);
}

LRESULT CALLBACK KNWindowProc(HWND windowHandle, UINT message, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	HRESULT hr = 0;

	switch(message)
	{
		case WM_CREATE:
		{
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory);
			if(hr != S_OK) return(-1);
		} break;

		case WM_PAINT:
		{
			hr = initGraphicsResources(windowHandle);
			hr = onPaint(windowHandle);
		} break;

		case WM_CHAR:
		{
			wchar_t inputChar = (wchar_t)wParam;
			wchar_t c = textBuffer[bufferIndex];
			if(inputChar == c)
			{
				if(!typingBegan)
				{
					typingBegan = true;
				}

				cursorFillColor = cursorFillColorGreen;

				previousBufferIndex = bufferIndex;
				++bufferIndex;
			}

		} break;

		case WM_SIZE:
		{
			if(renderTarget)
			{
				RECT clientRect;
				GetClientRect(windowHandle, &clientRect);
				D2D1_SIZE_U clientRectSize = D2D1::SizeU(clientRect.right, clientRect.bottom);
				renderTarget->Resize(clientRectSize);
				InvalidateRect(windowHandle, 0, false);
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
	WCHAR *windowClassName = L"KNWindow";

	WNDCLASS windowClass = {};
	windowClass.lpfnWndProc = KNWindowProc;
	windowClass.hInstance = instance;
	windowClass.lpszClassName = windowClassName;

	RegisterClass(&windowClass);

	RECT desiredRect = {0};
	desiredRect.right = 1280;
	desiredRect.bottom = 720;
	AdjustWindowRect(&desiredRect, WS_OVERLAPPEDWINDOW, false);

	HWND window = CreateWindow(
		windowClassName, L"KeyboardNinja", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		desiredRect.right - desiredRect.left, 
		desiredRect.bottom - desiredRect.top, 
		0, 0, instance, 0
	);

	//NOTE: initialize textBuffer
	textBuffer = (WCHAR *)calloc(BUFFER_SIZE, sizeof(WCHAR));

	// NOTE: loading text file
	wchar_t *filename = L"texts\\ru.txt";
	readFile(filename, &textBuffer, BUFFER_SIZE);

	// NOTE: drawing areas
	textRect = roundedRectAt(190, 30, 900, 300, 10);

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
