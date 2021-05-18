#include "LCD1602A.h"

void LCD1602_PinSetByte(uint8_t Byte);								//Set pin according 'Byte'
void LCD1602_PinSetHalfByte(uint8_t Byte);							//Set high pin according 'Byte'
void LCD1602_SendHalfByte(uint8_t Byte);							//Send high half byte
void LCD1602_Delay_us(uint32_t Count);								//Delay in microseconds

uint8_t Flag_LCD1602_Mode;

//======================================================================
void LCD1602_Init(uint8_t Mode) {
	switch(Mode) {
	case Mode_4bit:
		LCD1602_RW_WRITE;
		LCD1602_RS_COMMAND;
		LCD1602_Delay_us(30000);
		LCD1602_PinSetHalfByte(0x30);
		LCD1602_E_STROB;
		LCD1602_Delay_us(40);
		LCD1602_PinSetHalfByte(0x30);
		LCD1602_E_STROB;
		LCD1602_Delay_us(40);
		LCD1602_PinSetHalfByte(0x20);
		LCD1602_E_STROB;
		LCD1602_Delay_us(10000);
		Flag_LCD1602_Mode = Mode_4bit;
		LCD1602_SendCommand(TwoLine4Bits);
		LCD1602_Delay_us(10000);
		break;

	case Mode_8bit:
		Flag_LCD1602_Mode = Mode_8bit;
		LCD1602_Delay_us(30000);
		LCD1602_SendCommand(0x30);
		LCD1602_Delay_us(40);
		LCD1602_SendCommand(0x30);
		LCD1602_Delay_us(40);
		LCD1602_SendCommand(0x30);
		LCD1602_Delay_us(40);
		LCD1602_SendCommand(0x38);
		LCD1602_Delay_us(5000);
		LCD1602_SendCommand(TwoLine8Bits);
		LCD1602_Delay_us(5000);
		break;

	default:break;
	}

	LCD1602_SendCommand(Disp_ON);
	LCD1602_Delay_us(500);
	LCD1602_SendCommand(ShiftRight);
	LCD1602_Delay_us(500);
	LCD1602_SendCommand(ShiftCursRight);
	LCD1602_Delay_us(500);
	LCD1602_Clear();
}

//======================================================================
void LCD1602_Delay_us(uint32_t Count) {
    Count *= (SystemCoreClock/4000000);
    for(;Count>0;Count--);
}

//======================================================================
void LCD1602_PinSetByte(uint8_t Byte) {
	if((Byte>>7)&0x01) LCD1602_PORT->BSRR = LCD1602_D7;
	else LCD1602_PORT->BSRR = (LCD1602_D7<<16);
	if((Byte>>6)&0x01) LCD1602_PORT->BSRR = LCD1602_D6;
	else LCD1602_PORT->BSRR = (LCD1602_D6<<16);
	if((Byte>>5)&0x01) LCD1602_PORT->BSRR = LCD1602_D5;
	else LCD1602_PORT->BSRR = (LCD1602_D5<<16);
	if((Byte>>4)&0x01) LCD1602_PORT->BSRR = LCD1602_D4;
	else LCD1602_PORT->BSRR = (LCD1602_D4<<16);
	if((Byte>>3)&0x01) LCD1602_PORT->BSRR = LCD1602_D3;
	else LCD1602_PORT->BSRR = (LCD1602_D3<<16);
	if((Byte>>2)&0x01) LCD1602_PORT->BSRR = LCD1602_D2;
	else LCD1602_PORT->BSRR = (LCD1602_D2<<16);
	if((Byte>>1)&0x01) LCD1602_PORT->BSRR = LCD1602_D1;
	else LCD1602_PORT->BSRR = (LCD1602_D1<<16);
	if((Byte)&0x01) LCD1602_PORT->BSRR = LCD1602_D0;
	else LCD1602_PORT->BSRR = (LCD1602_D0<<16);
}

//======================================================================
void LCD1602_PinSetHalfByte(uint8_t Byte) {
	if((Byte>>7)&0x01) LCD1602_PORT->BSRR = LCD1602_D7;
	else LCD1602_PORT->BSRR = (LCD1602_D7<<16);
	if((Byte>>6)&0x01) LCD1602_PORT->BSRR = LCD1602_D6;
	else LCD1602_PORT->BSRR = (LCD1602_D6<<16);
	if((Byte>>5)&0x01) LCD1602_PORT->BSRR = LCD1602_D5;
	else LCD1602_PORT->BSRR = (LCD1602_D5<<16);
	if((Byte>>4)&0x01) LCD1602_PORT->BSRR = LCD1602_D4;
	else LCD1602_PORT->BSRR = (LCD1602_D4<<16);
}

//======================================================================
void LCD1602_SendHalfByte(uint8_t Byte) {
	LCD1602_RW_WRITE;
	LCD1602_RS_COMMAND;
	LCD1602_PinSetHalfByte(Byte);
	LCD1602_E_STROB;
}

//======================================================================
void LCD1602_SendCommand(uint8_t Cmd) {
	switch(Flag_LCD1602_Mode) {
	case Mode_8bit:
		LCD1602_RW_WRITE;
		LCD1602_RS_COMMAND;
		LCD1602_PinSetByte(Cmd);
		LCD1602_E_STROB;
		LCD1602_Delay_us(1000);
		break;

	case Mode_4bit:
		LCD1602_RW_WRITE;
		LCD1602_RS_COMMAND;
		LCD1602_PinSetHalfByte(Cmd);
		LCD1602_E_STROB;
		LCD1602_Delay_us(5000);
		Cmd<<=4;
		LCD1602_PinSetHalfByte(Cmd);
		LCD1602_E_STROB;
		LCD1602_Delay_us(1000);
		break;
	default:break;
	}
}

//======================================================================
void LCD1602_SendData(uint8_t Data) {
	switch(Flag_LCD1602_Mode) {
	case Mode_8bit:
		LCD1602_RW_WRITE;
		LCD1602_RS_DATA;
		LCD1602_PinSetByte(Data);
		LCD1602_E_STROB;
		LCD1602_Delay_us(1000);
		break;

	case Mode_4bit:
		LCD1602_RW_WRITE;
		LCD1602_RS_DATA;
		LCD1602_PinSetHalfByte(Data);
		LCD1602_E_STROB;
		LCD1602_Delay_us(1000);
		Data<<=4;
		LCD1602_PinSetHalfByte(Data);
		LCD1602_E_STROB;
		LCD1602_Delay_us(1000);
		break;
	default:break;
	}
}

//======================================================================
void LCD1602_SetCursorPosition(uint8_t x, uint8_t line) {
	uint8_t buffer=0;
	switch(line) {
		case 1:
			buffer=(0x80+x-1);
			LCD1602_SendCommand(buffer);
			break;
		case 2:
			buffer=(0x80|(0x40+x-1));
			LCD1602_SendCommand(buffer);
			break;
		default:break;
	}
	LCD1602_Delay_us(1000);
}

//======================================================================
void LCD1602_Clear(void) {
	LCD1602_SendCommand(Clear_Display);
	LCD1602_Delay_us(3000);
}

//======================================================================
void LCD1602_SendString(uint8_t x, uint8_t line, char data[]) {
	LCD1602_SetCursorPosition(x,line);
	for(uint8_t i=0; data[i]!='\0'; i++)
		LCD1602_SendData(data[i]);
}

//======================================================================
void LCD1602_ClearStr(uint8_t x, uint8_t line, uint8_t len) {
	LCD1602_SetCursorPosition(x,line);
	for(;len>0;len--)
		LCD1602_SendData(' ');
}



