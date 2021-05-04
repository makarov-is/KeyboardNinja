#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <windows.h>
#include <d2d1.h>

#include "Shapes.hpp"

#define LAYOUT_RU 0x00000419
#define LAYOUT_ENG 0x00000409

class Keyboard
{
public:
	Keyboard();
	~Keyboard();
	void Keyboard::init(IDWriteTextFormat **textFormat);
	void Keyboard::drawKeyboard(ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget);
	void drawKey(const wchar_t *keyStr, UINT keyStrLength, 
	             int x, int y, int width, int height, int radius, 
             	 D2D1_COLOR_F bgColor, D2D1_COLOR_F letterColor,
             	 ID2D1HwndRenderTarget **renderTarget);
	void Keyboard::switchLayout();
	void Keyboard::setRULayout();

	ID2D1SolidColorBrush *keyboardBrush;
	IDWriteTextFormat *keyboardTextFormat;

	D2D1_COLOR_F keyboardPlateColor =           D2D1::ColorF(0x4CBEFA);
	D2D1_COLOR_F keyboardPlateBorderUpColor =   D2D1::ColorF(0x79CEFB);
	D2D1_COLOR_F keyboardPlateBorderDownColor = D2D1::ColorF(0x49A9DB);

	D2D1_COLOR_F keyColorGreen =  D2D1::ColorF(0x78E2AA);
	D2D1_COLOR_F keyColorCyan =   D2D1::ColorF(0x63E3FF);
	D2D1_COLOR_F keyColorPink =   D2D1::ColorF(0xF69EC4);
	D2D1_COLOR_F keyColorOrange = D2D1::ColorF(0xFFC07E);
	D2D1_COLOR_F keyColorYellow = D2D1::ColorF(0xFFEF7E);
	D2D1_COLOR_F keyColorGray =   D2D1::ColorF(0x9AD3FA);

	D2D1_COLOR_F keyColorShade1 = D2D1::ColorF(0xB7E0FC);
	D2D1_COLOR_F keyColorShade2 = D2D1::ColorF(0x44A8DD);
	D2D1_COLOR_F keyColorLetter = D2D1::ColorF(0x5B5461);
	D2D1_COLOR_F keyColorLetterDim = D2D1::ColorF(0x999999);

	wchar_t keyboardENG[4][12] = 
	{
		{L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'-', L'='},
		{L'Q', L'W', L'E', L'R', L'T', L'Y', L'U', L'I', L'O', L'P', L'[', L']'},
		{L'A', L'S', L'D', L'F', L'G', L'H', L'J', L'K', L'L', L';', L'\'', L' '},
		{L'Z', L'X', L'C', L'V', L'B', L'N', L'M', L',', L'.', L'/', L' ', L' '}
	};

	wchar_t keyboardRU[4][12] = 
	{
		{L'1', L'2', L'3', L'4', L'5', L'6', L'7', L'8', L'9', L'0', L'-', L'='},
		{L'É', L'Ö', L'Ó', L'Ê', L'Å', L'Í', L'Ã', L'Ø', L'Ù', L'Ç', L'Õ', L'Ú'},
		{L'Ô', L'Û', L'Â', L'À', L'Ï', L'Ð', L'Î', L'Ë', L'Ä', L'Æ', L'Ý', L' '},
		{L'ß', L'×', L'Ñ', L'Ì', L'È', L'Ò', L'Ü', L'Á', L'Þ', L'.', L' ', L' '}
	};

	D2D1_COLOR_F keyboardColorsRow1[12] = 
	{
		keyColorGreen, keyColorGreen, keyColorCyan, keyColorPink, keyColorOrange,
		keyColorOrange, keyColorYellow, keyColorPink, keyColorCyan, keyColorGreen,
		keyColorGreen, keyColorGreen
	};

	D2D1_COLOR_F keyboardColors[10] = 
	{
		keyColorGreen, keyColorCyan, keyColorPink, keyColorOrange, keyColorOrange, 
		keyColorYellow, keyColorYellow, keyColorPink, keyColorCyan, keyColorGreen
	};

	wchar_t (*keyboard)[12];
	UINT currentKeyboardLayout;
};

#endif KEYBOARD_H
