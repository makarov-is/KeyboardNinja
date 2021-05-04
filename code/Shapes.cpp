#include "Shapes.hpp"

D2D1_RECT_F rectAt(int x, int y, int width, int height)
{
	D2D1_RECT_F result = D2D1::RectF(x, y, x + width, y + height);
	return(result);
}

D2D1_ROUNDED_RECT roundedRectAt(int x, int y, int width, int height, int radius)
{
	D2D1_RECT_F rect = D2D1::RectF(x, y, x + width, y + height);
	D2D1_ROUNDED_RECT result = D2D1::RoundedRect(rect, radius, radius);

	return(result);
}

void drawRoundedRect(int x, int y, int width, int height, int radius, D2D1_COLOR_F color,
                     ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget)
{
	D2D1_COLOR_F oldColor = (*brush)->GetColor();

	D2D1_ROUNDED_RECT rect = roundedRectAt(x, y, width, height, radius);
	(*brush)->SetColor(color);
	(*renderTarget)->FillRoundedRectangle(&rect, (*brush));

	(*brush)->SetColor(oldColor);
}
