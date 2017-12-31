/*
 * \file
 *
 * \brief FindMeSAT
 * spi.h
 *
 * Created: 27.12.2017 18:37:04
 * Author : DF4IAH
 */ 


#ifndef SPI_H_
#define SPI_H_

#include <asf.h>



#if 0
# define USART_SPI_AX												USARTC1
#else
# define SPI_AX														SPIC
#endif


#define C_SPI_AX_BUFFER_LENGTH										256


typedef enum SPI_AX_TRPT_STATE_ENUM {
	SPI_AX_TRPT_STATE_DISABLED										= 0x00,
	SPI_AX_TRPT_STATE_ENA_ADDR,
	SPI_AX_TRPT_STATE_ENA_DATA_WRITE,
	SPI_AX_TRPT_STATE_ENA_DATA_READ,
	SPI_AX_TRPT_STATE_COMPLETE,

	SPI_AX_TRPT_STATE_END											= 0x7f,
	SPI_AX_TRPT_STATE_ERROR											= 0xff,
} SPI_AX_TRPT_STATE_ENUM_t;



bool spi_ax_transport(bool isProgMem, const char* packet);
uint32_t spi_ax_calcFrequency_Mhz2Regs(float f_mhz);
void spi_ax_setFrequency2Regs(uint8_t chan, bool isFreqB);
void spi_ax_selectVcoFreq(bool isFreqB);
void spi_ax_initRegisters_PR1200(void);
void spi_ax_initRegisters_PR1200_Tx(void);
void spi_ax_initRegisters_PR1200_Rx(void);
void spi_ax_initRegisters_PR1200_Rx_WoR(void);
void spi_ax_initRegisters_PR1200_Rx_cont(void);
void spi_ax_initRegisters_PR1200_Rx_cont_SingleParamSet(void);
void spi_ax_initRegisters_AnlogFM(void);

void spi_init(void);
void spi_start(void);


#endif /* SPI_H_ */