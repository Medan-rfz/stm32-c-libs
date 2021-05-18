#ifndef __SPI_H
#define __SPI_H

#include <stdint.h>
#include "stm32f4xx.h" // Connect lib for your stm



uint8_t SPI_TransferByte(SPI_TypeDef *spi, uint8_t byte, int timeout);
uint16_t SPI_TransferWord(SPI_TypeDef *spi, uint16_t word, int timeout);

#endif


























