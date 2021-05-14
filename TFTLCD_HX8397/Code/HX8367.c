#include "HX8367.h"
#include "Delay.h"
#include "stm32f4xx.h"		// Connect lib for your stm

/////////////////////////////////////////////////////////
/////				Settings block					/////
/////////////////////////////////////////////////////////

//#define FULL_MANUAL_PINS

#define HX8367_DATA_PORT GPIOB
#define HX8367_CTRN_PORT GPIOA

#define HX8367_D0_PIN 0U
#define HX8367_D1_PIN 1U
#define HX8367_D2_PIN 2U
#define HX8367_D3_PIN 3U
#define HX8367_D4_PIN 4U
#define HX8367_D5_PIN 5U
#define HX8367_D6_PIN 6U
#define HX8367_D7_PIN 7U

#define HX8367_RST_PIN 1U
#define HX8367_CS_PIN 2U
#define HX8367_RS_PIN 3U
#define HX8367_WR_PIN 4U
#define HX8367_RD_PIN 5U

#ifndef FULL_MANUAL_PINS

#define	HX8367_D0_PORT HX8367_DATA_PORT
#define	HX8367_D1_PORT HX8367_DATA_PORT
#define	HX8367_D2_PORT HX8367_DATA_PORT
#define	HX8367_D3_PORT HX8367_DATA_PORT
#define	HX8367_D4_PORT HX8367_DATA_PORT
#define	HX8367_D5_PORT HX8367_DATA_PORT
#define	HX8367_D6_PORT HX8367_DATA_PORT
#define	HX8367_D7_PORT HX8367_DATA_PORT
#define	HX8367_RST_PORT HX8367_CTRN_PORT
#define	HX8367_CS_PORT HX8367_CTRN_PORT
#define	HX8367_RS_PORT HX8367_CTRN_PORT
#define	HX8367_WR_PORT HX8367_CTRN_PORT
#define	HX8367_RD_PORT HX8367_CTRN_PORT

#else

#define	HX8367_D0_PORT //PORT
#define	HX8367_D1_PORT //PORT
#define	HX8367_D2_PORT //PORT
#define	HX8367_D3_PORT //PORT
#define	HX8367_D4_PORT //PORT
#define	HX8367_D5_PORT //PORT
#define	HX8367_D6_PORT //PORT
#define	HX8367_D7_PORT //PORT
#define	HX8367_RST_PORT //PORT
#define	HX8367_CS_PORT //PORT
#define	HX8367_RS_PORT //PORT
#define	HX8367_WR_PORT //PORT
#define	HX8367_RD_PORT //PORT

#endif

/////////////////////////////////////////////////////////
/////			  End settings block				/////
/////////////////////////////////////////////////////////

#define HX8367_D0_SET HX8367_D0_PORT->BSRR = (1<<HX8367_D0_PIN)
#define HX8367_D0_RESET HX8367_D0_PORT->BSRR = (1<<(HX8367_D0_PIN + 16))
#define HX8367_D1_SET HX8367_D1_PORT->BSRR = (1<<HX8367_D1_PIN)
#define HX8367_D1_RESET HX8367_D1_PORT->BSRR = (1<<(HX8367_D1_PIN + 16))
#define HX8367_D2_SET HX8367_D2_PORT->BSRR = (1<<HX8367_D2_PIN)
#define HX8367_D2_RESET HX8367_D2_PORT->BSRR = (1<<(HX8367_D2_PIN + 16))
#define HX8367_D3_SET HX8367_D3_PORT->BSRR = (1<<HX8367_D3_PIN)
#define HX8367_D3_RESET HX8367_D3_PORT->BSRR = (1<<(HX8367_D3_PIN + 16))
#define HX8367_D4_SET HX8367_D4_PORT->BSRR = (1<<HX8367_D4_PIN)
#define HX8367_D4_RESET HX8367_D4_PORT->BSRR = (1<<(HX8367_D4_PIN + 16))
#define HX8367_D5_SET HX8367_D5_PORT->BSRR = (1<<HX8367_D5_PIN)
#define HX8367_D5_RESET HX8367_D5_PORT->BSRR = (1<<(HX8367_D5_PIN + 16))
#define HX8367_D6_SET HX8367_D6_PORT->BSRR = (1<<HX8367_D6_PIN)
#define HX8367_D6_RESET HX8367_D6_PORT->BSRR = (1<<(HX8367_D6_PIN + 16))
#define HX8367_D7_SET HX8367_D7_PORT->BSRR = (1<<HX8367_D7_PIN)
#define HX8367_D7_RESET HX8367_D7_PORT->BSRR = (1<<(HX8367_D7_PIN + 16))

#define HX8367_RST_SET HX8367_RST_PORT->BSRR = (1<<HX8367_RST_PIN)
#define HX8367_RST_RESET HX8367_RST_PORT->BSRR = (1<<(HX8367_RST_PIN + 16))
#define HX8367_CS_SET HX8367_CS_PORT->BSRR = (1<<HX8367_CS_PIN)
#define HX8367_CS_RESET HX8367_CS_PORT->BSRR = (1<<(HX8367_CS_PIN + 16))
#define HX8367_RS_SET HX8367_RS_PORT->BSRR = (1<<HX8367_RS_PIN)
#define HX8367_RS_RESET HX8367_RS_PORT->BSRR = (1<<(HX8367_RS_PIN + 16))
#define HX8367_WR_SET HX8367_WR_PORT->BSRR = (1<<HX8367_WR_PIN)
#define HX8367_WR_RESET HX8367_WR_PORT->BSRR = (1<<(HX8367_WR_PIN + 16))
#define HX8367_RD_SET HX8367_RD_PORT->BSRR = (1<<HX8367_RD_PIN)
#define HX8367_RD_RESET HX8367_RD_PORT->BSRR = (1<<(HX8367_RD_PIN + 16))

typedef struct
{
	uint8_t R;
	uint8_t G;
	uint8_t B;
	uint8_t byte0;
	uint8_t byte1;
} HX8367_Color;

static HX8367_Color _HX8367_color;

__STATIC_INLINE void HX8367_SetDataOnPins(uint8_t data)
{
	if((uint8_t)0x01U & data) HX8367_D0_SET;
	else HX8367_D0_RESET;
	if((uint8_t)0x01U & (data>>1)) HX8367_D1_SET;
	else HX8367_D1_RESET;
	if((uint8_t)0x01U & (data>>2)) HX8367_D2_SET;
	else HX8367_D2_RESET;
	if((uint8_t)0x01U & (data>>3)) HX8367_D3_SET;
	else HX8367_D3_RESET;
	if((uint8_t)0x01U & (data>>4)) HX8367_D4_SET;
	else HX8367_D4_RESET;
	if((uint8_t)0x01U & (data>>5)) HX8367_D5_SET;
	else HX8367_D5_RESET;
	if((uint8_t)0x01U & (data>>6)) HX8367_D6_SET;
	else HX8367_D6_RESET;
	if((uint8_t)0x01U & (data>>7)) HX8367_D7_SET;
	else HX8367_D7_RESET;
}

__STATIC_INLINE void HX8367_SetModePins(uint32_t modeMask)
{
	MODIFY_REG(HX8367_D0_PORT->MODER, (0x3 << HX8367_D0_PIN * 2U), (modeMask << HX8367_D0_PIN * 2U));
	MODIFY_REG(HX8367_D1_PORT->MODER, (0x3 << HX8367_D1_PIN * 2U), (modeMask << HX8367_D1_PIN * 2U));
	MODIFY_REG(HX8367_D2_PORT->MODER, (0x3 << HX8367_D2_PIN * 2U), (modeMask << HX8367_D2_PIN * 2U));
	MODIFY_REG(HX8367_D3_PORT->MODER, (0x3 << HX8367_D3_PIN * 2U), (modeMask << HX8367_D3_PIN * 2U));
	MODIFY_REG(HX8367_D4_PORT->MODER, (0x3 << HX8367_D4_PIN * 2U), (modeMask << HX8367_D4_PIN * 2U));
	MODIFY_REG(HX8367_D5_PORT->MODER, (0x3 << HX8367_D5_PIN * 2U), (modeMask << HX8367_D5_PIN * 2U));
	MODIFY_REG(HX8367_D6_PORT->MODER, (0x3 << HX8367_D6_PIN * 2U), (modeMask << HX8367_D6_PIN * 2U));
	MODIFY_REG(HX8367_D7_PORT->MODER, (0x3 << HX8367_D7_PIN * 2U), (modeMask << HX8367_D7_PIN * 2U));
}

__STATIC_INLINE uint8_t HX8367_ReadDataFromPins()
{
	uint8_t res = 0;

	HX8367_SetModePins(0x00);

	if(HX8367_D0_PORT->IDR & (1<<HX8367_D0_PIN)) res |= (1<<0);
	if(HX8367_D1_PORT->IDR & (1<<HX8367_D1_PIN)) res |= (1<<1);
	if(HX8367_D2_PORT->IDR & (1<<HX8367_D2_PIN)) res |= (1<<2);
	if(HX8367_D3_PORT->IDR & (1<<HX8367_D3_PIN)) res |= (1<<3);
	if(HX8367_D4_PORT->IDR & (1<<HX8367_D4_PIN)) res |= (1<<4);
	if(HX8367_D5_PORT->IDR & (1<<HX8367_D5_PIN)) res |= (1<<5);
	if(HX8367_D6_PORT->IDR & (1<<HX8367_D6_PIN)) res |= (1<<6);
	if(HX8367_D7_PORT->IDR & (1<<HX8367_D7_PIN)) res |= (1<<7);

	HX8367_SetModePins(0x01);

	return res;
}

void HX8367_WriteReg(uint8_t data)
{
	HX8367_RS_RESET;
	HX8367_RD_SET;
	HX8367_CS_RESET;
	HX8367_WR_RESET;
	HX8367_SetDataOnPins(data);
	HX8367_WR_SET;
	HX8367_CS_SET;
}

void HX8367_WriteData8(uint8_t data)
{
	HX8367_RS_SET;
	HX8367_RD_SET;
	HX8367_CS_RESET;
	HX8367_WR_RESET;
	HX8367_SetDataOnPins(data);
	HX8367_WR_SET;
	HX8367_CS_SET;
}

void HX8367_WriteData16(uint8_t data0, uint8_t data1)
{
	HX8367_RS_SET;
	HX8367_RD_SET;
	HX8367_CS_RESET;
	HX8367_WR_RESET;
	HX8367_SetDataOnPins(data0);
	HX8367_WR_SET;
	HX8367_WR_RESET;
	HX8367_SetDataOnPins(data1);
	HX8367_WR_SET;
	HX8367_CS_SET;
}

uint8_t HX8367_ReadData()
{
	uint8_t res = 0;

	HX8367_RS_SET;
	HX8367_WR_SET;
	HX8367_CS_RESET;
	HX8367_RD_RESET;
	res = HX8367_ReadDataFromPins();
	HX8367_RD_SET;
	HX8367_CS_SET;

	return res;
}

void HX8367_Reset()
{
	HX8367_RST_SET;
	DelayMs(1);
	HX8367_RST_RESET;
	HX8367_CS_SET;
	HX8367_WR_SET;
	HX8367_RS_SET;
	HX8367_RD_SET;
	DelayMs(5);
	HX8367_RST_SET;
	DelayMs(120);
}

inline void HX8367_Init()
{
	HX8367_Reset();			// Hardware reset

	HX8367_WriteReg(0xEA);
	HX8367_WriteData8(0x00);
	HX8367_WriteData8(0x20);

	HX8367_WriteReg(0xEC);
	HX8367_WriteData8(0x0C);
	HX8367_WriteData8(0xC4);

	HX8367_WriteReg(0xE8);
	HX8367_WriteData8(0x38);
	HX8367_WriteReg(0xE9);
	HX8367_WriteData8(0x10);
	HX8367_WriteReg(0xF1);
	HX8367_WriteData8(0x01);
	HX8367_WriteReg(0xF2);
	HX8367_WriteData8(0x10);

	// Gamma
	HX8367_WriteReg(0x40);
	HX8367_WriteData8(0x01);
	HX8367_WriteData8(0x00);
	HX8367_WriteData8(0x00);
	HX8367_WriteData8(0x10);
	HX8367_WriteData8(0x0E);
	HX8367_WriteData8(0x24);
	HX8367_WriteData8(0x04);
	HX8367_WriteData8(0x50);
	HX8367_WriteData8(0x02);
	HX8367_WriteData8(0x13);
	HX8367_WriteData8(0x19);
	HX8367_WriteData8(0x19);
	HX8367_WriteData8(0x16);

	HX8367_WriteReg(0x50);
	HX8367_WriteData8(0x1B);
	HX8367_WriteData8(0x31);
	HX8367_WriteData8(0x2F);
	HX8367_WriteData8(0x3F);
	HX8367_WriteData8(0x3F);
	HX8367_WriteData8(0x3E);
	HX8367_WriteData8(0x2F);
	HX8367_WriteData8(0x7B);
	HX8367_WriteData8(0x09);
	HX8367_WriteData8(0x06);
	HX8367_WriteData8(0x06);
	HX8367_WriteData8(0x0C);
	HX8367_WriteData8(0x1D);
	HX8367_WriteData8(0xCC);

	// Power voltage
	HX8367_WriteReg(0x1B);
	HX8367_WriteData8(0x1B);
	HX8367_WriteReg(0x1A);
	HX8367_WriteData8(0x01);
	HX8367_WriteReg(0x24);
	HX8367_WriteData8(0x2F);
	HX8367_WriteReg(0x25);
	HX8367_WriteData8(0x57);

	// VCOM
	HX8367_WriteReg(0x23);
	HX8367_WriteData8(0x88);

	HX8367_WriteReg(0x18);
	HX8367_WriteData8(0x34);
	HX8367_WriteReg(0x19);
	HX8367_WriteData8(0x01);
	HX8367_WriteReg(0x01);
	HX8367_WriteData8(0x00);
	HX8367_WriteReg(0x1F);
	HX8367_WriteData8(0x88);
	DelayUs(100);
	HX8367_WriteReg(0x1F);
	HX8367_WriteData8(0x80);
	DelayUs(100);
	HX8367_WriteReg(0x1F);
	HX8367_WriteData8(0x90);
	DelayUs(100);
	HX8367_WriteReg(0x1F);
	HX8367_WriteData8(0xD0);
	DelayUs(100);

	HX8367_WriteReg(0x17);
	HX8367_WriteData8(0x05);
	HX8367_WriteReg(0x36);
	HX8367_WriteData8(0x00);
	HX8367_WriteReg(0x28);
	HX8367_WriteData8(0x38);
	DelayUs(100);
	HX8367_WriteReg(0x28);
	HX8367_WriteData8(0x3F);
	HX8367_WriteReg(0x16);
	HX8367_WriteData8(0x58); // X - inv
	DelayMs(1);
}

HX8367_Stat HX8367_SetWindow(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	if((x1 > x2) || (y1 > y2))
		return HX8367_statErr;

	uint8_t tmpH = 0, tmpL = 0;

	HX8367_WriteReg(0x02);		// column start
	HX8367_WriteData8(0x00);
	HX8367_WriteReg(0x03);
	HX8367_WriteData8(x1);

	HX8367_WriteReg(0x04);		// column end
	HX8367_WriteData8(0x00);
	HX8367_WriteReg(0x05);
	HX8367_WriteData8(x2);

	tmpH = (uint8_t)(y1 >> 8);
	tmpL = (uint8_t)y1;
	HX8367_WriteReg(0x06);		// row start
	HX8367_WriteData8(tmpH);
	HX8367_WriteReg(0x07);
	HX8367_WriteData8(tmpL);

	tmpH = (uint8_t)(y2 >> 8);
	tmpL = (uint8_t)y2;
	HX8367_WriteReg(0x08);		// row end
	HX8367_WriteData8(tmpH);
	HX8367_WriteReg(0x09);
	HX8367_WriteData8(tmpL);

	HX8367_WriteReg(0x22);		// set window

	return HX8367_statOK;
}

__STATIC_INLINE uint8_t HX8367_NormalizeColor(uint8_t in, uint8_t normValue)
{
	// (0...255) to (0...normValue)
	uint8_t out;
	return out = (uint8_t)((double)in * (double)normValue / 255.0);
}

HX8367_Stat HX8367_SetColor(uint8_t R, uint8_t G, uint8_t B)	// TODO Переделать для разных схем (5-6-5 и 6-6-6)
{
	_HX8367_color.R = R;
	_HX8367_color.G = G;
	_HX8367_color.B = B;

	_HX8367_color.R = _HX8367_color.R > 31 ? 31 : _HX8367_color.R;
	_HX8367_color.G = _HX8367_color.G > 63 ? 63 : _HX8367_color.G;
	_HX8367_color.B = _HX8367_color.B > 31 ? 31 : _HX8367_color.B;

	_HX8367_color.byte0 = ((_HX8367_color.R << 3) & 0xF8) | ((_HX8367_color.G >> 3) & 0x07);
	_HX8367_color.byte1 = ((_HX8367_color.G << 5) & 0xE0) | (_HX8367_color.B & 0x1F);

	return HX8367_statOK;
}

void HX8367_SendColorPixel()		// TODO Переделать для разных схем (5-6-5 и 6-6-6)
{
	HX8367_WriteData8(_HX8367_color.byte0);
	HX8367_WriteData8(_HX8367_color.byte1);
}

HX8367_Stat HX8367_DrawPoint(uint16_t x, uint16_t y)
{
	if((x > 239) || (y > 319))
		return HX8367_statErr;

	HX8367_SetWindow(x,y,x,y);
	HX8367_SendColorPixel();
	return HX8367_statOK;
}

HX8367_Stat HX8367_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	int xerr=0, yerr=0, dx, dy, distance;
	int incx, incy, uRow, uCol;

	dx = x2-x1;
	dy = y2-y1;
	uRow = x1;
	uCol = y1;

	if(dx > 0) incx = 1;
	else if(dx == 0) incx = 0;
	else
	{
		incx = -1;
		dx = -dx;
	}

	if(dy > 0) incy = 1;
	else if(dy == 0) incy = 0;
	else
	{
		incy = -1;
		dy = -dy;
	}

	if( dx > dy) distance = dx;
	else distance = dy;

	for(uint16_t t = 0; t <= distance+1; t++)
	{
		HX8367_DrawPoint(uRow,uCol);
		xerr += dx;
		yerr += dy;
		if(xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if(yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}

	return HX8367_statOK;
}

HX8367_Stat HX8367_DrawFillRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	__IO uint32_t pixelsNumber;

	if(HX8367_SetWindow(x1, y1, x2, y2) == HX8367_statErr)
		return HX8367_statErr;

	pixelsNumber = (x2 - x1 + 1) * (y2 - y1 + 1); 	// dx * dy

	while(pixelsNumber--)
	{
		HX8367_SendColorPixel();
	}

	return HX8367_statOK;
}

HX8367_Stat HX8367_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, int thickness)
{
	while(thickness--)
	{
		HX8367_DrawLine(x1,y1,x2,y1);
		HX8367_DrawLine(x2,y1,x2,y2);
		HX8367_DrawLine(x2,y2,x1,y2);
		HX8367_DrawLine(x1,y2,x1,y1);
		x1++; x2--; y1++; y2--;
	}

	return HX8367_statOK;
}

HX8367_Stat HX8367_DrawCircle(uint16_t x0, uint16_t y0, uint16_t R, int thickness)
{
	int x, y ,delta, error;

	while(thickness--)
	{
		x = 0;
		y = R;
		delta = 1 - 2 * R;
		error = 0;

		while (y >= x)
		{
			HX8367_DrawPoint(x0 + x, y0 + y);
			HX8367_DrawPoint(x0 + x, y0 - y);
			HX8367_DrawPoint(x0 - x, y0 + y);
			HX8367_DrawPoint(x0 - x, y0 - y);
			HX8367_DrawPoint(x0 + y, y0 + x);
			HX8367_DrawPoint(x0 + y, y0 - x);
			HX8367_DrawPoint(x0 - y, y0 + x);
			HX8367_DrawPoint(x0 - y, y0 - x);

			error = 2 * (delta + y) - 1;

			if ((delta < 0) && (error <= 0))
			{
				delta += 2 * ++x + 1;
				continue;
			}

			if ((delta > 0) && (error > 0))
			{
				delta -= 2 * --y + 1;
				continue;
			}

			delta += 2 * (++x - --y);
		}

		(R != 0) ? R-- : R;
	}

	return HX8367_statOK;
}

void HX8367_test()
{
	HX8367_SetColor(255, 255, 255);
	HX8367_DrawFillRectangle(0, 0, 239, 319);

	HX8367_SetColor(0, 0, 0);
	HX8367_DrawRectangle(10,30,100,100,4);
	HX8367_DrawCircle(30,150,3,10);
}





