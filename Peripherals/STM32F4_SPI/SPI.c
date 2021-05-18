#include "SPI.h"
#include "Delay.h"
	
//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
inline uint8_t SPI_TransferByte(SPI_TypeDef *spi, uint8_t byte, int timeout)
{
	__IO int _timeout = timeout;

	while((spi->SR & SPI_SR_TXE) != SPI_SR_TXE)
	{
		DelayUs(1);
		if(--_timeout <= 0) return 0;
	}

	*(__IO uint8_t*)&spi->DR = byte;

	_timeout = timeout;
	while((spi->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
	{
		DelayUs(1);
		if(--_timeout <= 0) return 0;
	}

	return (*(__IO uint8_t*)&spi->DR);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//
inline uint16_t SPI_TransferWord(SPI_TypeDef *spi, uint16_t word, int timeout)
{
	__IO int _timeout = timeout;

	while((spi->SR & SPI_SR_TXE) != SPI_SR_TXE)
	{
		DelayUs(1);
		if(--_timeout <= 0) return 0;
	}

	*(__IO uint16_t*)&spi->DR = word;

	_timeout = timeout;
	while((spi->SR & SPI_SR_RXNE) != SPI_SR_RXNE)
	{
		DelayUs(1);
		if(--_timeout <= 0) return 0;
	}

	return (*(__IO uint16_t*)&spi->DR);
}

//-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*-*//



























