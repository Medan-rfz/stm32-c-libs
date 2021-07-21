#include "L3GD20.h"

#define L3GD20_FLAG_TIMEOUT 100000

uint8_t L3GD20_TIMEOUT_UserCallback(void);
uint8_t L3GD20_SendByte(uint8_t byte);

uint8_t L3GD20_RxBuff[10];
uint8_t L3GD20_TxBuff[10];


uint32_t L3GD20_Timeout = 0;


uint8_t L3GD20_SendByte(uint8_t byte);
uint8_t L3GD20_TIMEOUT_UserCallback(void);


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
void L3GD20_Init(void)
{
	L3GD20_Write(0x20, 0xCF);
	L3GD20_Write(0x23, 0x30);
	
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
uint8_t L3GD20_SendByte(uint8_t byte)
{
	L3GD20_Timeout = L3GD20_FLAG_TIMEOUT;
	while(!LL_SPI_IsActiveFlag_TXE(SPI1)){
		if((L3GD20_Timeout--) == 0) return L3GD20_TIMEOUT_UserCallback();
	}
	
	SPI2_DR_8bit = byte;
	
	L3GD20_Timeout = L3GD20_FLAG_TIMEOUT;
	while(!LL_SPI_IsActiveFlag_RXNE(SPI1)){
		if((L3GD20_Timeout--) == 0) return L3GD20_TIMEOUT_UserCallback();
	}
	
	return SPI2_DR_8bit;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
uint8_t L3GD20_Read(uint8_t ReadAddr)
{
	uint8_t temp;

	ReadAddr |= (uint8_t)0x80;
	
	GPIOE->BSRR = GPIO_BSRR_BR_3;
	
	L3GD20_SendByte(ReadAddr);
	// Send dummy byte (0x00) to generate the SPI clock to L3GD20 (Slave device)
	temp = L3GD20_SendByte(0x00);
	
	GPIOE->BSRR = GPIO_BSRR_BS_3;
	
	return temp;
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
void L3GD20_Write(uint8_t WriteAddr, uint8_t data)
{
	GPIOE->BSRR = GPIO_BSRR_BR_3;
	
	L3GD20_SendByte(WriteAddr);
	
	L3GD20_SendByte(data);
	
	GPIOE->BSRR = GPIO_BSRR_BS_3;
}


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
double L3GD20_GetValues(uint8_t NbrVal)
{
	uint16_t Val = 0;
	uint8_t Sign = 0;
	double deltaOrientation = 0;
	
	switch(NbrVal) {
		case GetValX:
			Val = L3GD20_Read(0x29);
			Val = Val << 8;
			Val |= L3GD20_Read(0x28);
			break;
			
		case GetValY:
			Val = L3GD20_Read(0x2B);
			Val = Val << 8;
			Val |= L3GD20_Read(0x2A);
			break;
			
		case GetValZ:
			Val = L3GD20_Read(0x2D);
			Val = Val << 8;
			Val |= L3GD20_Read(0x2C);
			break;
			
		default: break;
	}
	
	
	if ((Val & 0x8000) == 0) {			// Определение направления вращения по знаку числа
		Sign = 0;					
	}	
	else {
		Sign = 1;
		Val &= 0x7FFF;
		Val = 0x8000 - Val;
	}				
	
	
//	if (Val <= 0x1)						// Порог срабатывания, для уменьшения погрешности из-за шумов
//	{
//		Val = 0;
//	}	
		
	if (Sign == 0) {
		deltaOrientation = (0.07 * Val * 0.01);			// 0.07 задаётся исходя из настроек режима работы гироскопа
	}																				// 0.01 = 1 мс - период опроса (100 Гц)
	else {
		deltaOrientation = ((-1)  * 0.07 * Val * 0.01);
	}
	
	return deltaOrientation;
}


//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-//
uint8_t L3GD20_TIMEOUT_UserCallback(void)
{
	while(1)
	{
	
	}
	return 0;
}





