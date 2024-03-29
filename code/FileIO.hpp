#ifndef FILE_IO_H
#define FILE_IO_H

#include <windows.h>

char *allocTemporaryBuffer(UINT bufferSize);
bool checkFileSizeOK(UINT bytesRead, UINT outBufferSize);
void readFile(const WCHAR *filename, WCHAR **outBuffer, UINT outBufferSize, 
              UINT *textLength, UINT *bufferIndex);

#endif FILE_IO_H
