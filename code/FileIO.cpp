#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE  
#endif

#include <windows.h>

#include "FileIO.hpp"

char *allocTemporaryBuffer(UINT bufferSize)
{
	char *result = (char *)calloc(bufferSize, sizeof(char));
	return(result);
}

bool checkFileSizeOK(UINT bytesRead, UINT outBufferSize)
{
	bool result = false;
	if(bytesRead > outBufferSize - 1)
	{
		result = true;
	}
	return(result);
}

void readFile(const wchar_t *filename, WCHAR **outBuffer, UINT outBufferSize)
{
	bool result = false;
	HANDLE file = 0;

	char *buffer = allocTemporaryBuffer(outBufferSize);

	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(file == INVALID_HANDLE_VALUE)
	{
		//TODO: error handling
	    return;
	}

	UINT bytesRead = 0;
	BOOL read = ReadFile(file, buffer, outBufferSize, (LPDWORD)&bytesRead, 0);
	if(!read)
	{
		//TODO: error handling
		CloseHandle(file);
		return;
	}

	if(!checkFileSizeOK(bytesRead ,outBufferSize))
	{
		// TODO: load default txt file
		CloseHandle(file);
		return;
	}

	// NOTE: converting char to wchar
	MultiByteToWideChar(CP_UTF8, 0, buffer, outBufferSize, *outBuffer, outBufferSize);
}
