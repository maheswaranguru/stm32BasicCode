/******************************************************************************
 *                                                                            *
 * File name    :   actuator.c
 * Date         :   30-05-2023
 * Created by   :   Maheswaran G
 * Description  :
 * This file provides functions to support all types od actuators. this contains
 * API for top level leyer.
 *
 * Copyright (C) Sanmina Tech Service
 ******************************************************************************/

#include <stdint.h>
#include <stdint.h>

#include "actuator.h"
#include"debugCli.h"
#include "actuatorWrapper.h"



GPIO_T mygpio[MAX_PIN] = {
		{REDLED,RED_LED_GPIO_Port, RED_LED_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW,GPIO_PIN_RESET},
		{GREENLED,GREEN_LED_GPIO_Port, GREEN_LED_Pin, GPIO_MODE_OUTPUT_PP, GPIO_NOPULL, GPIO_SPEED_FREQ_LOW,GPIO_PIN_RESET},

};
void actuatorTask( void )
{
	 //	      gpio_init();


	 TASK_LOOP
		{

		 //debugText("\n\rHello");


		 //          gpioToggle(&mygpio[REDLED]);
		 //          HAL_Delay (500);
		 //          gpioToggle(&mygpio[GREENLED]);
		 //          HAL_Delay (500);


  		TASK_DELAY(1000);
  	}
}


void gpioWrite(GPIO_T *mygpio)
{
	HAL_GPIO_WritePin((void *)mygpio->Port, mygpio->Init.Pin, mygpio->Pinstate);


}


void gpioToggle(GPIO_T *mygpio)
{
	HAL_GPIO_TogglePin(mygpio->Port, mygpio->Init.Pin);

}


void gpiooperate(uint8_t u8PortId,uint8_t u8PinBit,GPIOCommandList_t mode)
{

	 GPIO_PinState bPinStatus = GPIO_PIN_RESET;

	 if((PORT_CHANNEL_D >= u8PortId) && (PORT_BIT_POS_15 >= u8PinBit))
	        {

		 	 	switch(mode)
		 	 	{
		 	 	case SET_OUT:
		 	 	{
		 	 		debugText("\r\n Gpio direction output set.");
		 	 	}break;

		 	 	case SET_IN:
		 	 	{
		 	 		debugText("\r\n Gpio direction input set.");
		 	 	}break;

		 	 	case GPIO_SET:
		 	 	{
		 	 		if ((u8PortId == PORT_CHANNEL_D) && (u8PinBit == PORT_BIT_POS_12))
		 	 			{
		 	 				mygpio[GREENLED].Pinstate = GPIO_PIN_SET;
		 	 				gpioWrite(&mygpio[GREENLED]);
		 	 			}
		 	 		else if ((u8PortId == PORT_CHANNEL_D)&& (u8PinBit == PORT_BIT_POS_14))
		 	 			{
		 	 				mygpio[REDLED].Pinstate = GPIO_PIN_SET;
		 	 				gpioWrite(&mygpio[REDLED]);
		 	 			}
		 	 		debugText("\r\n Gpio Output High set.");

		 	 	}break;

		 	 	case GPIO_RESET:
		 	 	{
		 	 		if((u8PortId == PORT_CHANNEL_D) && (u8PinBit == PORT_BIT_POS_12) )
		 	 		{
		 	 			mygpio[GREENLED].Pinstate = GPIO_PIN_SET;
		 	 			gpioWrite(&mygpio[GREENLED]);
		 	 		}
		 	 		else if ((u8PortId == PORT_CHANNEL_D)&& (u8PinBit == PORT_BIT_POS_14))
		 	 		{
		 	 			mygpio[REDLED].Pinstate = GPIO_PIN_RESET;
		 	 			gpioWrite(&mygpio[REDLED]);
		 	 		}

		 	 		debugText("\r\n Gpio Output Low set.");
		 	 	}break;
		 	 	case GET:
		 	 	{
		 	 		bPinStatus = HAL_GPIO_ReadPin(GPIOD, GREEN_LED_Pin);

		 	 		                if(GPIO_PIN_SET == bPinStatus)
		 	 		                {
		 	 		                    debugText( "\r\n Gpio input is high.");
		 	 		                }
		 	 		                else
		 	 		                {
		 	 		                    debugText( "\r\n Gpio input is low.");
		 	 		                }
		 	 	}break;
		 	 	}

	        }
	        else
	        {
	            debugText( "\r\n Invalid Port Or Pin");
	        }


}

void gpio_init()
{
	  GPIO_InitTypeDef GPIO_InitStruct = {0};
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	  __HAL_RCC_GPIOH_CLK_ENABLE();
	  __HAL_RCC_GPIOA_CLK_ENABLE();
	  __HAL_RCC_GPIOD_CLK_ENABLE();

	  /*Configure GPIO pin Output Level */
	  HAL_GPIO_WritePin(GPIOD, GREEN_LED_Pin|RED_LED_Pin, GPIO_PIN_RESET);

	  /*Configure GPIO pins : GREEN_LED_Pin RED_LED_Pin */
	  GPIO_InitStruct.Pin = GREEN_LED_Pin|RED_LED_Pin;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

void actuatorConfig( uint8_t actNo )
{

	(void) actNo;
	return;
}
