#ifndef __L3GD20_H
#define __L3GD20_H

#include "main.h"
#include "Quaternion.h"

#define GetValX 0x01
#define GetValY 0x02
#define GetValZ 0x03

#define SPI2_DR_8bit	*(__IO uint8_t*)&(SPI1->DR)


void L3GD20_Init(void);
uint8_t L3GD20_Read(uint8_t ReadAddr);
void L3GD20_Write(uint8_t WriteAddr, uint8_t data);
double L3GD20_GetValues(uint8_t NbrVal);



#endif






