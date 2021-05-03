#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

#include "FileIO.hpp"

ID2D1Factory *factory;
ID2D1HwndRenderTarget *renderTarget;
ID2D1SolidColorBrush *brush;

D2D1_ROUNDED_RECT textRect;

IDWriteFactory *writeFactory;
IDWriteTextFormat *textFormat;
IDWriteTextLayout *textLayout;
D2D1_RECT_F textLayoutRect;

#define BUFFER_SIZE 2048
WCHAR *textBuffer = 0;

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

void onPaint()
{
	if(!textFormat)
	{
		writeFactory->CreateTextFormat(L"Open Sans", 0, DWRITE_FONT_WEIGHT_REGULAR, 
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
	renderTarget->Clear(D2D1::ColorF(0.33f, 0.77f, 1.0f));

	// NOTE: draw text area rectangle
	brush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f));
	renderTarget->FillRoundedRectangle(&textRect, brush);

	// NOTE: draw text
	brush->SetColor(D2D1::ColorF(0.0f, 0.0f, 0.0f));
	renderTarget->DrawTextLayout(D2D1::Point2F(textLayoutRect.left, textLayoutRect.top),
	                             textLayout, brush);

	renderTarget->EndDraw();
}

LRESULT CALLBACK KNWindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;
	HRESULT hr = 0;

	switch(msg)
	{
		case WM_CREATE:
		{
			hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
			hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory);
			if(hr != S_OK) return(-1);
		} break;

		case WM_PAINT:
		{
			initGraphicsResources(window);
			onPaint();
		} break;

		case WM_SIZE:
		{
			if(renderTarget)
			{
				RECT clientRect;
				GetClientRect(window, &clientRect);
				D2D1_SIZE_U clientRectSize = D2D1::SizeU(clientRect.right, clientRect.bottom);
				renderTarget->Resize(clientRectSize);
				InvalidateRect(window, 0, false);
			}
		} break;

		case WM_CLOSE:
		{
			DestroyWindow(window);
		} break;

		case WM_DESTROY:
		{
			PostQuitMessage(0);
		} break;

		default:
		{
			result = DefWindowProc(window, msg, wParam, lParam);
		} break;
	}

	return(result);
}

int WinMain(HINSTANCE instance, HINSTANCE prevInstance, LPSTR cmdLine, int showCmd)
{
	char *windowClassName = "KNWindow";

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
		windowClassName, "KeyboardNinja", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		desiredRect.right - desiredRect.left, 
		desiredRect.bottom - desiredRect.top, 
		0, 0, instance, 0
	);

	//NOTE: initialize textBuffer
	textBuffer = (WCHAR *)calloc(BUFFER_SIZE, sizeof(WCHAR));

	// NOTE: loading text file
	wchar_t *filename = L"texts\\ru.txt";
	readFile(filename, &textBuffer, BUFFER_SIZE);

	ShowWindow(window, showCmd);

	MSG msg = {};
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(0);
}
