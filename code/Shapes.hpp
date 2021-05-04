#ifndef SHAPED_H
#define SHAPED_H

#include <d2d1.h>

D2D1_RECT_F rectAt(int x, int y, int width, int height);
D2D1_ROUNDED_RECT roundedRectAt(int x, int y, int width, int height, int radius);
void drawRoundedRect(int x, int y, int width, int height, int radius, D2D1_COLOR_F color,
                     ID2D1SolidColorBrush **brush, ID2D1HwndRenderTarget **renderTarget);

#endif SHAPED_H
