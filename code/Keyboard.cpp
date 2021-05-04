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

	keyboard = keyboardRU;
	currentKeyboardLayout = LAYOUT_RU;
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

	// NOTE: tilde
	drawKey(L"", 1, posX, posY, w, h, r, keyColorGray, keyColorLetterDim, renderTarget);
	// NOTE: first row
	posX = 307;
	for(int i = 0; i < 12; i++)
	{
		drawKey(&keyboard[0][i], 1, posX, posY, w, h, r, keyboardColorsRow1[i], keyColorLetter, renderTarget);
		posX += 51;
	}
	// NOTE: backspace
	drawKey(L"", 1, posX, posY, 86, h, r, keyColorGray, keyColorLetterDim, renderTarget);

	// NOTE: second row
	posY += 55;
	drawKey(L"TAB", 3, 256, posY, 68, h, r, keyColorGray, keyColorLetterDim, renderTarget);
	posX = 330;
	for(int j = 0; j < 10; j++)
	{
		drawKey(&keyboard[1][j], 1, posX, posY, w, h, r, keyboardColors[j], keyColorLetter, renderTarget);
		posX += 51;
	}
	drawKey(&keyboard[1][10], 1, posX, posY, w, h, r, keyboardColors[0], keyColorLetter, renderTarget);
	posX += 51;
	drawKey(&keyboard[1][11], 1, posX, posY, w, h, r, keyboardColors[0], keyColorLetter, renderTarget);
	posX += 51;
	drawKey(L"\\", 1, posX, posY, 63, h, r, keyColorGray, keyColorLetterDim, renderTarget);
}
