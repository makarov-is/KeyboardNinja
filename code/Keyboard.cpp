#include "Keyboard.hpp"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::init(IDWriteTextFormat **textFormat)
{
	keyboardTextFormat = (*textFormat);
}

void Keyboard::drawKey(const wchar_t *keyStr, UINT keyStrLength, 
                       int x, int y, int width, int height, int radius, 
					   D2D1_COLOR_F bgColor, D2D1_COLOR_F letterColor, 
					   ID2D1HwndRenderTarget **renderTarget)
{
	D2D1_COLOR_F oldColor = keyboardBrush->GetColor();

	drawRoundedRect(x, y - 1, width, height, radius, keyColorShade1, &keyboardBrush, renderTarget);
	drawRoundedRect(x, y + 1, width, height, radius, keyColorShade2, &keyboardBrush, renderTarget);
	drawRoundedRect(x, y, width, height, radius, bgColor, &keyboardBrush, renderTarget);

	keyboardBrush->SetColor(letterColor);
	(*renderTarget)->DrawText(keyStr, keyStrLength, keyboardTextFormat, 
	                          D2D1::RectF(x + 16, y + 10, x + 16 + 100, y + 20), keyboardBrush);

	keyboardBrush->SetColor(oldColor);
}

void Keyboard::drawKeyboard(ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget)
{
	if(!keyboardBrush) (*renderTarget)->CreateSolidColorBrush(keyboardPlateColor, &keyboardBrush);
	keyboardBrush->SetColor(keyboardPlateColor);

	drawRoundedRect(246, 360 - 1, 765, 300, 10, keyboardPlateBorderUpColor, brush, renderTarget);
	drawRoundedRect(246, 360 + 1, 765, 300, 10, keyboardPlateBorderDownColor, brush, renderTarget);
	drawRoundedRect(246, 360, 765, 300, 10, keyboardPlateColor, brush, renderTarget);

	// NOTE: drawing keys
	int w = 45;
	int h = 45;
	int r = 5;

	int posX = 256;
	int posY = 370;

	drawKey(L"A", 1, 256, posY, w, h, r, keyColorGray, keyColorLetterDim, renderTarget);
}
