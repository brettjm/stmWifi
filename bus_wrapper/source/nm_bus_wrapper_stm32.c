/**
 *
 * \file
 *
 * \brief This module contains NMC1000 bus wrapper APIs implementation.
 *
 * Copyright (c) 2016-2017 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include <stdio.h>
#include <stdint.h>
#include "nm_bsp.h"
#include "nm_bsp_stm32.h"
#include "nm_common.h"
#include "nm_bus_wrapper.h"
#include "stm32f4xx_hal.h"		// For GPIO

#define NM_BUS_MAX_TRX_SZ	256

/* GPIO instances for atwinc driver */
static GPIO_InitTypeDef GPIO_InitCS;      // GPIO Struct for CS Pin

/* SPI handler declaration */
SPI_HandleTypeDef SpiHandle;

// Cases for spi transfer status
enum {
	TRANSFER_WAIT,
	TRANSFER_COMPLETE,
	TRANSFER_ERROR
};

tstrNmBusCapabilities egstrNmBusCapabilities =
{
	NM_BUS_MAX_TRX_SZ
};

/* transfer state */
__IO uint32_t wTransferState = TRANSFER_WAIT;

///**
// * @brief  This function is executed in case of error occurrence.
// * @param  None
// * @retval None
// */
//static void Timeout_Error_Handler(void)
//{
//	/* -3- Toggle IO in an infinite loop */
//	while (1)
//	{
//		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
//
//		/* Insert delay 100 ms */
//		HAL_Delay(50);
//	}
//}

static sint8 spi_rw(uint8* pu8Mosi, uint8* pu8Miso, uint16 u16Sz)
{
	uint8 u8Dummy = 0;
	uint8 u8SkipMosi = 0, u8SkipMiso = 0;
	uint8_t txd_data = 0;
	uint8_t rxd_data = 0;

	if(((pu8Miso == NULL) && (pu8Mosi == NULL)) ||(u16Sz == 0)) {
		return M2M_ERR_INVALID_ARG;
	}

	if (!pu8Mosi) {
		pu8Mosi = &u8Dummy;
		u8SkipMosi = 1;
	}
	if(!pu8Miso) {
		pu8Miso = &u8Dummy;
		u8SkipMiso = 1;
	}

	// Write cs pin low to begin spi transfer
	HAL_GPIO_WritePin(GPIOA, WINC_SPI_CS_PIN, GPIO_PIN_RESET);

	while (u16Sz)
	{
		txd_data = *pu8Mosi;
		HAL_SPI_TransmitReceive(&SpiHandle, &txd_data, &rxd_data, 1, 1000);
		*pu8Miso = rxd_data;

//		switch(HAL_SPI_TransmitReceive(&SpiHandle, (uint8_t*)pu8Mosi, (uint8_t *)pu8Miso, u16Sz, 5000))
//		{
//		case HAL_TIMEOUT:
//			/* A Timeout Occurred */
//			/* Call Timeout Handler */
//			Timeout_Error_Handler();
//			break;
//			/* An Error Occurred */
//		case HAL_ERROR:
//			/* Call Timeout Handler */
//			Error_Handler();
//			break;
//		default:
//			break;
//		}

		u16Sz--;
		if (!u8SkipMiso)
			pu8Miso++;
		if (!u8SkipMosi)
			pu8Mosi++;
	}

	// write cs pin high to end spi transfer
	HAL_GPIO_WritePin(GPIOA, WINC_SPI_CS_PIN, GPIO_PIN_SET);

	return M2M_SUCCESS;
}

/*
 *	@fn		nm_bus_init
 *	@brief	Initialize the bus wrapper
 *	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
 */
sint8 nm_bus_init(void *pvinit)
{
	sint8 result = M2M_SUCCESS;

	/* Configure the SPI peripheral */
	SpiHandle.Instance               = SPI3;
	SpiHandle.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;  // try _256
	SpiHandle.Init.Direction         = SPI_DIRECTION_2LINES;
	SpiHandle.Init.CLKPhase          = SPI_PHASE_1EDGE;
	SpiHandle.Init.CLKPolarity       = SPI_POLARITY_LOW;
	SpiHandle.Init.DataSize          = SPI_DATASIZE_8BIT;
	SpiHandle.Init.FirstBit          = SPI_FIRSTBIT_MSB;
	SpiHandle.Init.TIMode            = SPI_TIMODE_DISABLE;
	SpiHandle.Init.CRCCalculation    = SPI_CRCCALCULATION_DISABLE;
	SpiHandle.Init.CRCPolynomial     = 10;
	SpiHandle.Init.NSS               = SPI_NSS_SOFT;
	SpiHandle.Init.Mode              = SPI_MODE_MASTER;

	if(HAL_SPI_Init(&SpiHandle) != HAL_OK)
		Error_Handler();  /* Initialization Error */

	/* Enable GPIO Clock (to be able to program the configuration registers) */
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure CS pin as output
	GPIO_InitCS.Pin   = WINC_SPI_CS_PIN;
	GPIO_InitCS.Mode  = GPIO_MODE_OUTPUT_PP;
	GPIO_InitCS.Pull  = GPIO_NOPULL;
	GPIO_InitCS.Speed = GPIO_SPEED_LOW;

	/* Set SPI CS pin. */
	HAL_GPIO_Init(GPIOA, &GPIO_InitCS);

	/* Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, WINC_SPI_CS_PIN, GPIO_PIN_SET);

	/* Reset WINC1500. */
	nm_bsp_reset();
	nm_bsp_sleep(1);

	return result;
}

/*
 *	@fn		nm_bus_ioctl
 *	@brief	send/receive from the bus
 *	@param[IN]	u8Cmd
 *					IOCTL command for the operation
 *	@param[IN]	pvParameter
 *					Arbitrary parameter depenging on IOCTL
 *	@return	M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
 *	@note	For SPI only, it's important to be able to send/receive at the same time
 */
sint8 nm_bus_ioctl(uint8 u8Cmd, void* pvParameter)
{
	sint8 s8Ret = 0;

	if (u8Cmd == NM_BUS_IOCTL_RW)
	{
		tstrNmSpiRw *pstrParam = (tstrNmSpiRw *)pvParameter;
		s8Ret = spi_rw(pstrParam->pu8InBuf, pstrParam->pu8OutBuf, pstrParam->u16Sz);
	}
	else
	{
		s8Ret = -1;
		M2M_ERR("invalide ioclt cmd\n");
	}

	return s8Ret;
}

/*
 *	@fn		nm_bus_deinit
 *	@brief	De-initialize the bus wrapper
 */
sint8 nm_bus_deinit(void)
{
	HAL_SPI_DeInit(&SpiHandle);

	return 0;
}

/*
 *	@fn			nm_bus_reinit
 *	@brief		re-initialize the bus wrapper
 *	@param [in]	void *config
 *					re-init configuration data
 *	@return		M2M_SUCCESS in case of success and M2M_ERR_BUS_FAIL in case of failure
 */
sint8 nm_bus_reinit(void* config)
{
	return M2M_SUCCESS;
}

