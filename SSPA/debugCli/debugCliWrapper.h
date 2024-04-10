/******************************************************************************
 *                                                                            *
 * File name    :   debugCli.c
 * Date         :   30-05-2023
 * Created by   :   Maheswaran G
 * Description  :
 * This file provides functions definition(s) for all supporting platform for
 * main Cli API for debug message output.
 *
 * Copyright (C) Sanmina Tech Service
 ******************************************************************************/
#include "debugCli.h"

#ifdef STM32

#define  UART_CONFIG_TYPE		UART_HandleTypeDef

#define DEBUGPORT_OBJ_TYPE		UART_CONFIG_TYPE

#define UART_INIT(a)  			{ HAL_UART_Init( a ) }

#define UART_SENT_IT(a,b,c)		HAL_UART_Transmit_IT(a, b, c)
#define UART_SENT(a,b,c,d)		HAL_UART_Transmit(a, b, c, d)
#define UART_RECEIVE_IT(a,b,c)	HAL_UART_Receive_IT(a, b, c)
#define UART_RECEIVE(a,b,c,d)	HAL_UART_Receive(a, b, c, d)

#define GET_UART_STATUS      	HAL_UART_GetState
#define CHECK_PORT_ERROR(a)     HAL_UART_GetError(a)
#define DEINT_PORT(a)           HAL_UART_DeInit(a)
#define STOP_DEBUG_PORT(a)      HAL_UART_AbortTransmit_IT(a)

#define UART_READY              HAL_UART_STATE_READY
#define DELAY_MS                HAL_Delay
#define NO_ERROR                HAL_UART_ERROR_NONE
//#define HAL_OK

#define HAL_UART_DeInit
#define HAL_UART_GetError
#define HAL_UART_GetState
//#define HAL_UART_Init()			debugConsoleInit()

extern UART_CONFIG_TYPE debugPort;

/*********************************************************************************
 *Name :- debugConsoleInit
 *Para1:- N/A
 *Return:-N/A
 *Details:-  Initialize the uart used for debug.
 **********************************************************************************/
static bool debugConsoleInit( void )
{

    bool returnValue = true;
    /* USER CODE END USART2_Init  */
    debugPort.Instance = DEBUG_UART;
    debugPort.Init.BaudRate = BAUDRATE;
    debugPort.Init.WordLength = UART_WORDLENGTH_8B;
    debugPort.Init.StopBits = UART_STOPBITS_1;
    debugPort.Init.Parity = UART_PARITY_NONE;
    debugPort.Init.Mode = UART_MODE_TX_RX;
    debugPort.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    debugPort.Init.OverSampling = UART_OVERSAMPLING_16;

    if ( HAL_UART_Init( &debugPort ) != HAL_OK )
    {
        returnValue = false;
        // Error_Handler();     //!< NEED TO ADD error handling function.
    }
    else
    {

    }

    return ( returnValue );

}

#endif

#ifdef TI_MSP430
#include "driverlib.h"
#include "MSP430HAL.h"

#define HAL_OK                  ( 1 )
#define UART_READY              ( 1 )           //!< NEED to implement for MSP430

#define UART_INIT(a, b)         HAL_UART_Init(a, b)
#define UART_SENT_IT(a,b,c)     uartHalTxINT(a, b, c)
#define uartHalTx(a,b,c,d)      HAL_UART_Transmit(a, b, c, d)
#define UART_RECEIVE_IT(a,b,c)
#define UART_RECEIVE(a,b,c,d)

#define GET_UART_STATUS(a)      USCI_A_UART_queryStatusFlags(DEBUG_UART, (uint8_t)a)
#define CHECK_PORT_ERROR(a)     ( 1 )
#define DEINT_PORT(a)           USCI_A_UART_disable( DEBUG_UART )
#define STOP_DEBUG_PORT(a)      ( 1 )

#define UART_READY              ( 1 )
#define DELAY_MS                 __delay_cycles
#define NO_ERROR                ( 1 )
#define HAL_OK                  ( 1 )

#define HAL_UART_DeInit
#define HAL_UART_GetError
#define HAL_UART_GetState
#define HAL_UART_Init


USCI_A_UART_initParam uartConfig = { 0 };

/*********************************************************************************
 *Name :- debugConsoleInit
 *Para1:- N/A
 *Return:-N/A
 *Details:-  Initialize the uart used for debug.
 **********************************************************************************/
static bool debugConsoleInit( void )
{

    uartConfig.msborLsbFirst = USCI_A_UART_LSB_FIRST;
    uartConfig.numberofStopBits = USCI_A_UART_ONE_STOP_BIT;
    uartConfig.overSampling = USCI_A_UART_LOW_FREQUENCY_BAUDRATE_GENERATION;
    uartConfig.parity = USCI_A_UART_NO_PARITY;
    uartConfig.selectClockSource = USCI_A_UART_CLOCKSOURCE_SMCLK;
    uartConfig.uartMode = USCI_A_UART_MODE;
    uartConfig.firstModReg = 0;
    uartConfig.secondModReg = 6;
    uartConfig.clockPrescalar = 34;


    GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P4, GPIO_PIN4 + GPIO_PIN5 );

    if (STATUS_FAIL == UART_INIT(DEBUG_UART, &uartConfig))
    {
        while(1);
    }

    USCI_A_UART_enable(DEBUG_UART);
    USCI_A_UART_clearInterrupt( DEBUG_UART, USCI_A_UART_RECEIVE_INTERRUPT );
    USCI_A_UART_enableInterrupt( DEBUG_UART, USCI_A_UART_RECEIVE_INTERRUPT);
    __enable_interrupt();

    return 1;
}

#endif
