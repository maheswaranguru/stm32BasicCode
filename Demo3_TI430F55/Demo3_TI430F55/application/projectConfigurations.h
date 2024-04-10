/*
 * projectConfig.h
 *
 *  Created on: Jun 22, 2023
 *      Author: maheswaran_gurusamy
 */

#ifndef SRC_APPLICATION_PROJECTCONFIGURATIONS_H_
#define SRC_APPLICATION_PROJECTCONFIGURATIONS_H_


#include <stdbool.h>

#include "macro.h"


#ifdef STM32

#ifdef STM32G070xx
#include "stm32g0xx_hal.h"
#include "stm32g0xx_hal_conf.h"
#endif

#if defined(STM32Fxx) || defined(STM32F427xx) || defined(STM32F407xx) || defined(STM32F411xE)
#include "stm32f4xx_hal.h"
#endif

#if defined (STM32F769xx)
#include "stm32f7xx_hal.h"
#endif

#if defined (STM32L4R9xx)
#include "stm32l4xx_hal.h"
#endif


//@@@@@@@@@@@@@@@@@@@   THIS HAVE TO RESTRUCUTURE MAKE COMMON  @@@@@@@@@@@@@@@@@@@
#define UART_CONFIG_TYPE UART_HandleTypeDef

#ifndef DEBUG_UART
#define RING_BUFF_SIZE  512
#define DEBUG_UART  USART2
#define BAUDRATE    115200
#define DATA_BIT_8 UART_WORDLENGTH_8B
#endif
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#endif		//!< STM32 End Here

#ifdef TI_MSP430

typedef enum
{
  HAL_OK       = 0x00U,
  HAL_ERROR    = 0x01U,
  HAL_BUSY     = 0x02U,
  HAL_TIMEOUT  = 0x03U
} HAL_StatusTypeDef;

//@@@@@@@@@@@@@@@@@@@	THIS HAVE TO RESTRUCUTURE MAKE COMMON  @@@@@@@@@@@@@@@@@@@
#define UART_CONFIG_TYPE uint16_t

#ifndef DEBUG_UART
#define RING_BUFF_SIZE	512
#define DEBUG_UART	USCI_A1_BASE
#define BAUDRATE	115200
#endif
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
#endif

#endif /* SRC_APPLICATION_PROJECTCONFIGURATIONS_H_ */
