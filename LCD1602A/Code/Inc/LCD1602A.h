
#ifndef __LCD1602A_H
#define __LCD1602A_H

#include "main.h"
#include "stdint.h"

//=====Function=====
//-----Users-----
void LCD1602_Init(uint8_t Mode);									//Initialization LCD1602
void LCD1602_SetCursorPosition(uint8_t x, uint8_t line);			//Set position cursor on display
void LCD1602_SendCommand(uint8_t Cmd);								//Send commands
void LCD1602_SendData(uint8_t Data);								//Send data
void LCD1602_SendString(uint8_t x, uint8_t line, char data[]);		//Write string in set position
void LCD1602_ClearStr(uint8_t x, uint8_t line, uint8_t len);		//Clear 'len'-symbols in set position
void LCD1602_Clear(void);											//Clear all display

//=====Structures=====

//=====Variables=====
extern uint8_t Flag_LCD1602_Mode;

//=====GPIO=====
#define LCD1602_PORT GPIOD
//#define LCD1602_RCC RCC_AHBPeriph_GPIOD
#define LCD1602_D0 GPIO_PIN_0
#define LCD1602_D1 GPIO_PIN_1
#define LCD1602_D2 GPIO_PIN_2
#define LCD1602_D3 GPIO_PIN_3
#define LCD1602_D4 GPIO_PIN_4
#define LCD1602_D5 GPIO_PIN_5
#define LCD1602_D6 GPIO_PIN_6
#define LCD1602_D7 GPIO_PIN_7
#define LCD1602_8_PIN_DATA LCD1602_D0|LCD1602_D1|LCD1602_D2|LCD1602_D3|LCD1602_D4|LCD1602_D5|LCD1602_D6|LCD1602_D7
#define LCD1602_4_PIN_DATA LCD1602_D4|LCD1602_D5|LCD1602_D6|LCD1602_D7

#define LCD1602_RS GPIO_PIN_8
#define LCD1602_RW GPIO_PIN_9
#define LCD1602_E GPIO_PIN_10
#define LCD1602_PIN_CONTROL LCD1602_RS|LCD1602_RW|LCD1602_E

#define LCD1602_RS_DATA LCD1602_PORT->BSRR = LCD1602_RS;
#define LCD1602_RS_COMMAND LCD1602_PORT->BSRR = (LCD1602_RS<<16);
#define LCD1602_RW_READ LCD1602_PORT->BSRR = LCD1602_RW;
#define LCD1602_RW_WRITE LCD1602_PORT->BSRR = (LCD1602_RW<<16);
#define LCD1602_E_SET LCD1602_PORT->BSRR = LCD1602_E;
#define LCD1602_E_RESET LCD1602_PORT->BSRR = (LCD1602_E<<16);
#define LCD1602_E_STROB {LCD1602_E_SET;LCD1602_Delay_us(50);LCD1602_E_RESET;}

//=====Command=====
#define Clear_Display 0x01
#define Disp_ON 0x0C
#define ShiftCursRight 0x14
#define TwoLine8Bits 0x38
#define TwoLine4Bits 0x28
#define ShiftRight 0x06

//=====Other=====
#define Mode_4bit 1
#define Mode_8bit 2


#endif
