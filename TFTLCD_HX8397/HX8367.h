#ifndef __HX8367_H
#define __HX8367_H

#include <stdint.h>

typedef enum
{
	HX8367_statOK,
	HX8367_statErr,
} HX8367_Stat;

typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t byte0;
	uint8_t byte1;
} HX8367_Color;

void HX8367_Init();
HX8367_Stat HX8367_SetColor(uint8_t R, uint8_t G, uint8_t B);
HX8367_Stat HX8367_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
HX8367_Stat HX8367_DrawPixel(uint16_t x, uint16_t y);
HX8367_Stat HX8367_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
HX8367_Stat HX8367_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
HX8367_Stat HX8367_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, int thickness);
HX8367_Stat HX8367_DrawCircle(uint16_t x0, uint16_t y0, uint16_t R, int thickness);
void HX8367_DrawChar_11x14(uint16_t *x, uint16_t *y, HX8367_Color *colorChar, HX8367_Color *colorBackground, uint8_t c);
void HX8367_DrawString(uint16_t x, uint16_t y, HX8367_Color *colorChar, HX8367_Color *colorBackground,
						void (*func)(uint16_t*, uint16_t*, HX8367_Color*, HX8367_Color*, uint8_t), char *str);
void HX8367_test();

#endif


