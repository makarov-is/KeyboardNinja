#include "Image.hpp"

Image::Image()
{
	bitmapRect = D2D1::RectF(0, 0, 0, 0);
}

Image::~Image()
{
}

Image::Image(UINT x, UINT y, UINT width, UINT height)
{
	bitmapRect = D2D1::RectF(x, y, x + width, y + height);
}

void Image::setRect(UINT x, UINT y, UINT width, UINT height)
{
	bitmapRect = D2D1::RectF(x, y, x + width, y + height);
}

HRESULT Image::createBitmap(LPCWSTR fileName, 
                            ID2D1HwndRenderTarget **renderTarget, IWICImagingFactory **imagingFactory)
{
	HRESULT hr = 0;

	if(!bitmap && bitmapRect.right != 0 && bitmapRect.bottom != 0)
	{
		if(*imagingFactory)
		{
			hr = (*imagingFactory)->CreateDecoderFromFilename(fileName, 0, GENERIC_READ,
															  WICDecodeMetadataCacheOnLoad, &bitmapDecoder);
			hr = bitmapDecoder->GetFrame(0, &bitmapFrame);
			hr = (*imagingFactory)->CreateFormatConverter(&formatConverter);
			hr = formatConverter->Initialize(bitmapFrame, GUID_WICPixelFormat32bppPBGRA,
												WICBitmapDitherTypeNone, 0, 0, WICBitmapPaletteTypeCustom);

			hr = (*renderTarget)->CreateBitmapFromWicBitmap(formatConverter, 0, &(bitmap));
		}
	}

	return(hr);
}
