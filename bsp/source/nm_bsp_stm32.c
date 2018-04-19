/**
 *
 * \file
 *
 * \brief This module contains SAMD21 BSP APIs implementation.
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

#include "nm_bsp.h"
#include "nm_common.h"
#include "nm_bsp_stm32.h"
#include "stm32f4xx_hal.h"

// Used as a data type of ISR function
// registered by nm_bsp_register_isr
static tpfNmBspIsr gpfIsr;

void chip_isr(void)
{
   if (gpfIsr) {
      gpfIsr();
   }
}

/*
 * @fn      init_chip_pins
 * @brief   Initialize reset, chip enable and wake pin (irq?)
 */
static void init_chip_pins(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

    /* Enable GPIO Clock (to be able to program the configuration registers) */
   __HAL_RCC_GPIOA_CLK_ENABLE();
   __HAL_RCC_GPIOC_CLK_ENABLE();

   /*Configure Reset pin Output Level */
   HAL_GPIO_WritePin(GPIOA, WINC_RESET_PIN, GPIO_PIN_RESET);

   // Configure RESET pin as output
   GPIO_InitStruct.Pin   = WINC_RESET_PIN;
   GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
   GPIO_InitStruct.Pull  = GPIO_NOPULL;
   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;

   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//   // Configure CHIP_EN as pull-up
//   GPIO_InitStruct.Pin   = WINC_CHIP_EN_PIN;
//   GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
//   GPIO_InitStruct.Pull  = GPIO_PULLUP;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
//   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

   // Configure IRQ pin as input, pullup
   // GPIO mode is set to detect interrupts on falling edges
   GPIO_InitStruct.Pin  = WINC_IRQ_PIN;
   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
   GPIO_InitStruct.Pull = GPIO_NOPULL;
   HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
//   GPIO_InitStruct.Pin   = WINC_IRQ_PIN;
//   GPIO_InitStruct.Mode  = GPIO_MODE_IT_FALLING;
//   GPIO_InitStruct.Pull  = GPIO_NOPULL;
//   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
//   HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

/*
 * Turn off the WINC board by setting CHIP_EN and RESET_N signals low.
 * Every function call of "nm_bsp_init" should be matched with a call to
 * nm_bsp_deinit. Failure to do so may result in the WINC consuming higher
 * power than expected.
 */
static void deinit_chip_pins(void)
{
   // De-initializes the RESET peripheral register
   // to its default reset values
   HAL_GPIO_DeInit(GPIOA, WINC_RESET_PIN);

   // De-initializes the CHIP_EN peripheral register
   // to its default reset values
//   HAL_GPIO_DeInit(GPIOA, GPIO_InitChipEn.Pin);
}

/*
 * @fn      nm_bsp_init
 * @brief   Initialize BSP
 * @return  0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_init(void)
{
   gpfIsr = NULL;
   
   /* Initialize chip IOs. */
   init_chip_pins();

   /* Perform atwinc chip reset. */
   nm_bsp_reset();

   return M2M_SUCCESS;
}

/**
 * @fn      nm_bsp_deinit
 * @brief   De-iInitialize BSP
 * @return  0 in case of success and -1 in case of failure
 */
sint8 nm_bsp_deinit(void)
{
   // De-initializes the GPIOx peripheral 
   // registers to their default reset values.
   deinit_chip_pins();

   return M2M_SUCCESS;
}

/**
 * @fn      nm_bsp_reset
 * @brief   Reset NMC1500 SoC by setting CHIP_EN and RESET_N signals low,
 *           CHIP_EN high then RESET_N high
 */
void nm_bsp_reset(void)
{
   // Set CHIP_EN and RESET_N pins low
//   HAL_GPIO_WritePin(GPIOA, GPIO_InitChipEn.Pin, GPIO_PIN_RESET);
   HAL_GPIO_WritePin(GPIOA, WINC_RESET_PIN, GPIO_PIN_RESET);
   nm_bsp_sleep(100);

   // Set CHIP_EN high, pause, then set RESET_N high
//   HAL_GPIO_WritePin(GPIOA, GPIO_InitChipEn.Pin, GPIO_PIN_SET);
   nm_bsp_sleep(100);
   HAL_GPIO_WritePin(GPIOA, WINC_RESET_PIN, GPIO_PIN_SET);
   nm_bsp_sleep(100);
}

/*
 * @fn      nm_bsp_sleep
 * @brief   Sleep in units of mSec
 * @param[IN]  u32TimeMsec
 *          Time in milliseconds
 */
void nm_bsp_sleep(uint32 u32TimeMsec)
{
   /*
   Care must be taken when using HAL_Delay(), this function provides accurate delay (in milliseconds)
   based on variable incremented in SysTick ISR. This implies that if HAL_Delay() is called from
   a peripheral ISR process, then the SysTick interrupt must have higher priority (numerically lower)
   than the peripheral interrupt. Otherwise the caller ISR process will be blocked.
   To change the SysTick interrupt priority you have to use HAL_NVIC_SetPriority() function.
   */
 
   // SysTick Timer is configured by default to generate an interrupt each 1 msec.
   // The configuration is done using HAL_SYSTICK_Config() located in HAL_Init().
   while (u32TimeMsec--) {
      HAL_Delay(1);
   }
}

/*
 * @fn      nm_bsp_register_isr
 * @brief   Register interrupt service routine
 * reference to chip_isr() with stm32 isr
 * @param[IN]  pfIsr
 *          Pointer to ISR handler
 */
void nm_bsp_register_isr(tpfNmBspIsr pfIsr)
{
   gpfIsr = pfIsr;

   /*
    * Handler for pins connected to line 10 to 15
    * We are using pin a10 so that's line 10
    * Irq: EXTI15_10_IRQn
    * Handler: EXTI15_10_IRQHandler
    */

   /* EXTI interrupt init*/
   HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
}

/*
 * @fn      nm_bsp_interrupt_ctrl
 * @brief   Enable/Disable interrupts
 * @param[IN]  u8Enable
 *          '0' disable interrupts. '1' enable interrupts
 */
void nm_bsp_interrupt_ctrl(uint8 u8Enable)
{
   if (u8Enable) {
	   HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
   } else {
	   HAL_NVIC_DisableIRQ(EXTI15_10_IRQn);
   }
}
