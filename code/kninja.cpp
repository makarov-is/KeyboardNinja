#include <Windows.h>

LRESULT CALLBACK KNWindowProc(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT result = 0;

	switch(msg)
	{
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

	return(0);
}
