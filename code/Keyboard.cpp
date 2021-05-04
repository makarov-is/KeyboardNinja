#include "Keyboard.hpp"

Keyboard::Keyboard()
{
}

Keyboard::~Keyboard()
{
}

void Keyboard::drawKeyboard(ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget)
{
	if(!keyboardBrush) (*renderTarget)->CreateSolidColorBrush(keyboardPlateColor, &keyboardBrush);
	keyboardBrush->SetColor(keyboardPlateColor);

	drawRoundedRect(246, 360 - 1, 765, 300, 10, keyboardPlateBorderUpColor, brush, renderTarget);
	drawRoundedRect(246, 360 + 1, 765, 300, 10, keyboardPlateBorderDownColor, brush, renderTarget);
	drawRoundedRect(246, 360, 765, 300, 10, keyboardPlateColor, brush, renderTarget);
}
