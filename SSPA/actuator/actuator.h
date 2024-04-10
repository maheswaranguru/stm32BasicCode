/******************************************************************************
 *                                                                            *
 * File name    :   actuator.h
 * Date         :   30-05-2023
 * Created by   :   Maheswaran G
 * Description  :
 * This file provides functions to support all types od actuators. this contains
 * API for top level leyer.
 *
 * Copyright (C) Sanmina Tech Service
 ******************************************************************************/
#ifndef SRC_ACTUATOR_H_
#define SRC_ACTUATOR_H_

#include "macro.h"
#include "projectconfigurations.h"

//@@@@@@@@@@@@@@@@@@@ PROJECT RELATED DATA @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@L
#define GREEN_LED_Pin GPIO_PIN_12
#define GREEN_LED_GPIO_Port GPIOD
#define RED_LED_Pin GPIO_PIN_14
#define RED_LED_GPIO_Port GPIOD

typedef enum
{
	REDLED,
	GREENLED,
	MAX_PIN
}PinId;

typedef struct GPIO_T
{
  PinId Pinid;
  GPIO_TypeDef* Port;
  GPIO_InitTypeDef Init;
  GPIO_PinState Pinstate;
}GPIO_T;


typedef enum{
	SET_OUT,
	SET_IN,
	GPIO_SET,
	GPIO_RESET,
	GET
}GPIOCommandList_t;



typedef enum
{
    PORT_CHANNEL_A = 0x00,
    PORT_CHANNEL_B = 0x01,
    PORT_CHANNEL_C = 0x02,
    PORT_CHANNEL_D = 0x03,

} TEPortChannel;

typedef enum
{
    PORT_BIT_POS_0 = 0,
    PORT_BIT_POS_1 = 1,
    PORT_BIT_POS_2 = 2,
    PORT_BIT_POS_3 = 3,
    PORT_BIT_POS_4 = 4,
    PORT_BIT_POS_5 = 5,
    PORT_BIT_POS_6 = 6,
    PORT_BIT_POS_7 = 7,
    PORT_BIT_POS_8 = 8,
    PORT_BIT_POS_9 = 9,
    PORT_BIT_POS_10 = 10,
    PORT_BIT_POS_11 = 11,
    PORT_BIT_POS_12 = 12,
    PORT_BIT_POS_13 = 13,
    PORT_BIT_POS_14 = 14,
    PORT_BIT_POS_15 = 15
} TEPortBitPos;

void actuatorTask( void );
void gpioWrite(GPIO_T *mygpio);
void gpioRead(GPIO_T *mygpio);
void gpioToggle(GPIO_T *mygpio);

void gpiooperate(uint8_t u8PortId,uint8_t u8PinBit,GPIOCommandList_t mode);

#endif
