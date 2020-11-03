#include <Windows.h>
#include <d2d1.h>

ID2D1Factory *factory;
ID2D1HwndRenderTarget *renderTarget;
ID2D1SolidColorBrush *backgroundBrush;

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
			D2D1_COLOR_F backgroundColor = D2D1::ColorF(0.0f, 0.0f, 0.0f);
			result = renderTarget->CreateSolidColorBrush(backgroundColor, &backgroundBrush);
		}
	}

	return(result);
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

	HWND window = CreateWindow(windowClassName, "KeyboardNinja", WS_OVERLAPPEDWINDOW,
	                           CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, 0, 0, instance, 0);
	ShowWindow(window, showCmd);

	MSG msg = {};
	while(GetMessage(&msg, 0, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return(0);
}
