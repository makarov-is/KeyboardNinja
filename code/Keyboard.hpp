#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>
#include <d2d1.h>

#include "Shapes.hpp"

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	void Keyboard::drawKeyboard(ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget);

	ID2D1SolidColorBrush *keyboardBrush;

	D2D1_COLOR_F keyboardPlateColor =           D2D1::ColorF(0x4CBEFA);
	D2D1_COLOR_F keyboardPlateBorderUpColor =   D2D1::ColorF(0x79CEFB);
	D2D1_COLOR_F keyboardPlateBorderDownColor = D2D1::ColorF(0x49A9DB);
};

#endif KEYBOARD_H
