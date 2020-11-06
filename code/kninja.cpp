#include <Windows.h>
#include <d2d1.h>
#include <dwrite.h>

ID2D1Factory *factory;
ID2D1HwndRenderTarget *renderTarget;
ID2D1SolidColorBrush *brush;

D2D1_ROUNDED_RECT textRect;

IDWriteFactory *writeFactory;
IDWriteTextFormat *textFormat;
IDWriteTextLayout *textLayout;

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

			textRect = D2D1::RoundedRect(D2D1::RectF(50.0f, 20.0f, 750.0f, 320.0f), 10.0f, 10.0f);
		}
	}

	return(result);
}

void onPaint()
{
	renderTarget->BeginDraw();

	renderTarget->Clear(D2D1::ColorF(0.33f, 0.77f, 1.0f));

	brush->SetColor(D2D1::ColorF(1.0f, 1.0f, 1.0f));
	renderTarget->FillRoundedRectangle(&textRect, brush);

	renderTarget->EndDraw();
}

LRESULT CALLBACK KNWindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch(msg)
	{
		case WM_CREATE:
		{
			D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &factory);
		} break;

		case WM_PAINT:
		{
			initGraphicsResources(window);
			onPaint();
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

	//HWND window = CreateWindow(windowClassName, "KeyboardNinja", WS_OVERLAPPEDWINDOW,
	                           //CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, instance, 0);
	HWND window = CreateWindow(
		windowClassName, "KeyboardNinja", WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 
		desiredRect.right - desiredRect.left, 
		desiredRect.bottom - desiredRect.top
		, 0, 0, instance, 0
	);
	ShowWindow(window, showCmd);

	DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown **)&writeFactory);

	MSG msg = {};
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(0);
}
