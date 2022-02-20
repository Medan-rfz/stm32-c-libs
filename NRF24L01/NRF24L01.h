#ifndef __NRF24L01_H
#define __NRF24L01_H

#include <stdbool.h>

#define NRF24L01_ACTIVATE 		0x50
#define NRF24L01_RD_RX_PLOAD 	0x61 // Define RX payload register address
#define NRF24L01_WR_TX_PLOAD 	0xA0 // Define TX payload register address
#define NRF24L01_FLUSH_TX 		0xE1
#define NRF24L01_FLUSH_RX 		0xE2

#define NRF24L01_CONFIG 		0x00 //'Config' register address
#define NRF24L01_EN_AA 			0x01 //'Enable Auto Acknowledgment' register address
#define NRF24L01_EN_RXADDR 		0x02 //'Enabled RX addresses' register address
#define NRF24L01_SETUP_AW 		0x03 //'Setup address width' register address
#define NRF24L01_SETUP_RETR 	0x04 //'Setup Auto. Retrans' register address
#define NRF24L01_RF_CH 			0x05 //'RF channel' register address
#define NRF24L01_RF_SETUP 		0x06 //'RF setup' register address
#define NRF24L01_STATUS 		0x07 //'Status' register address
#define NRF24L01_OBSERVE_TX 	0x08 //'Transmit observe' register
#define NRF24L01_RX_ADDR_P0 	0x0A //'RX address pipe0' register address
#define NRF24L01_RX_ADDR_P1 	0x0B //'RX address pipe1' register address
#define NRF24L01_TX_ADDR 		0x10 //'TX address' register address
#define NRF24L01_RX_PW_P0 		0x11 //'RX payload width, pipe0' register address
#define NRF24L01_RX_PW_P1 		0x12 //'RX payload width, pipe1' register address
#define NRF24L01_FIFO_STATUS	0x17 //'FIFO Status Register' register address
#define NRF24L01_DYNPD 			0x1C
#define NRF24L01_FEATURE 		0x1D

#define NRF24L01_CONFIG_PRIM_RX		(uint8_t)0x01
#define NRF24L01_CONFIG_PWR_UP 		(uint8_t)0x02


#define NRF24L01_STATUS_MAX_RT 		(uint8_t)0x10
#define NRF24L01_STATUS_TX_DS 		(uint8_t)0x20
#define NRF24L01_STATUS_RX_DS 		(uint8_t)0x40

typedef void (*NRF24L01_RxCallback)(uint8_t *pBuf, uint8_t size);

typedef struct
{
	uint8_t SelfAddr[5];
	uint8_t RemoteAddr[5];

	uint8_t FreqCh;
	uint8_t PayloadLen;

	bool TxDone;
	NRF24L01_RxCallback RxCallback;

	SPI_TypeDef *Spi;
	uint8_t PinCE;
	uint8_t PinCSN;
	uint8_t PinIRQ;
	GPIO_TypeDef *Port;
} NRF24L01_TypeDef;


void NRF24L01_Init(NRF24L01_TypeDef *bstruct);
uint8_t NRF24L01_ReadReg(NRF24L01_TypeDef *bstruct, uint8_t reg);
void NRF24L01_WriteReg(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t data);
void NRF24L01_ReadBuf(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t *pBuf, uint8_t size);
void NRF24L01_WriteBuf(NRF24L01_TypeDef *bstruct, uint8_t reg, uint8_t *pBuf, uint8_t size);
int NRF24L01_Transmit(NRF24L01_TypeDef *bstruct, uint8_t *pBuf, uint8_t size);
void NRF24L01_CallbackIRQ(NRF24L01_TypeDef *bstruct);


#endif /* __NRF24L01_H */
