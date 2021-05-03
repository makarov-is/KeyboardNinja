#include <windows.h>

#include "ErrorMessage.hpp"

void printLastError()
{
	DWORD error = GetLastError();
    if(error == 0) return;

    LPSTR messageBuffer = 0;
    size_t size = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                 NULL, error, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
    OutputDebugStringA(messageBuffer);
    LocalFree(messageBuffer);
}
