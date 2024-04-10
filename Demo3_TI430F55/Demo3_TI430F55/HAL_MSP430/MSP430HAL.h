/*
 * uartMSP430HAL.h
 *
 *  Created on: 03-Aug-2023
 *      Author: maheswaran_gurusamy
 */

#ifndef HAL_MSP430_MSP430HAL_H_
#define HAL_MSP430_MSP430HAL_H_

#include "driverlib.h"

#define UCS_XT1_CRYSTAL_FREQUENCY    32768
#define UCS_XT2_CRYSTAL_FREQUENCY    4000000
#define UCS_XT2_TIMEOUT 50000



void sysClkConfig( void );


#endif /* HAL_MSP430_MSP430HAL_H_ */
