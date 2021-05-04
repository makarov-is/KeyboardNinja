#ifndef UNICODE
#define UNICODE
#endif

#ifndef _UNICODE
#define _UNICODE  
#endif

#include <windows.h>

#include "FileIO.hpp"
#include "ErrorMessage.hpp"

char *allocTemporaryBuffer(UINT bufferSize)
{
	char *result = (char *)calloc(bufferSize, sizeof(char));
	return(result);
}

bool checkFileSizeOK(UINT bytesRead, UINT outBufferSize)
{
	bool result = true;
	if(bytesRead <= 0 || outBufferSize <= 0)
	{
		result = false;
		return(result);
	}

	if(bytesRead > outBufferSize - 1)
	{
		result = false;
	}
	return(result);
}

void readFile(const WCHAR *filename, WCHAR **outBuffer, UINT outBufferSize, 
              UINT *textLength, UINT *bufferIndex)
{
	bool result = false;
	HANDLE file = 0;

	char *buffer = allocTemporaryBuffer(outBufferSize);
	*bufferIndex = 0;

	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(file == INVALID_HANDLE_VALUE)
	{
		printLastError();
	    return;
	}

	UINT bytesRead = 0;
	BOOL read = ReadFile(file, buffer, outBufferSize, (LPDWORD)&bytesRead, 0);
	if(!read)
	{
		printLastError();

		CloseHandle(file);
		return;
	}

	if(!checkFileSizeOK(bytesRead ,outBufferSize))
	{
		printLastError();

		CloseHandle(file);
		return;
	}

	// NOTE: converting char to wchar
	MultiByteToWideChar(CP_UTF8, 0, buffer, outBufferSize, *outBuffer, outBufferSize);

	// NOTE: calculating number of characters
	int wcharIndex = 0;
	while(wcharIndex < outBufferSize)
	{
		WCHAR wchar = (*outBuffer)[wcharIndex];
		if(wchar == '\0') break;
		++wcharIndex;
	}
	*textLength = wcharIndex;
}
