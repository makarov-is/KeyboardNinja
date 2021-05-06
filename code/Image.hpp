#ifndef IMAGE_H
#define IMAGE_H

#include <windows.h>
#include <d2d1.h>
#include <wincodec.h>
#include <combaseapi.h>

class Image
{
public:
	Image();
	~Image();
	Image(UINT x, UINT y, UINT width, UINT height);
	void setRect(UINT x, UINT y, UINT width, UINT height);
	HRESULT createBitmap(LPCWSTR fileName, 
	                     ID2D1HwndRenderTarget **renderTarget, IWICImagingFactory **imagingFactory);

	IWICBitmapDecoder *bitmapDecoder;
	IWICFormatConverter *formatConverter;
	IWICBitmapFrameDecode *bitmapFrame;

	ID2D1Bitmap *bitmap;
	D2D1_RECT_F bitmapRect;
};

#endif IMAGE_H
