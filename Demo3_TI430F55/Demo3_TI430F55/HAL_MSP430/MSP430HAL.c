/*
 * uartMSP430HAL.c
 *
 *  Created on: 03-Aug-2023
 *      Author: maheswaran_gurusamy
 */
#include<string.h>
#include"MSP430HAL.h"


volatile static uint16_t uartTxBuffSize = 0;
volatile static uint16_t uartTxWriteCmpt = 0;
volatile static uint8_t *uartTxBuffPtr = NULL;


/*********************************************************************************
 *Name :- sysClkConfig
 *Para1:- N/A
 *Return:-N/A
 *Details:-
 **********************************************************************************/
void sysClkConfig( void )
{
    UCS_setExternalClockSource( UCS_XT1_CRYSTAL_FREQUENCY, UCS_XT2_CRYSTAL_FREQUENCY );
    GPIO_setAsPeripheralModuleFunctionInputPin( GPIO_PORT_P5, GPIO_PIN2 + GPIO_PIN3 );
    UCS_turnOnXT2WithTimeout(UCS_XT2_DRIVE_4MHZ_8MHZ, UCS_XT2_TIMEOUT);
    UCS_initClockSignal( UCS_SMCLK, UCS_XT2CLK_SELECT, UCS_CLOCK_DIVIDER_1 );
}




/*********************************************************************************
 *Name :- uartHalTx
 *Para1:- uartBase
 *Para2:- point of the tx string
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool uartHalTx( uint16_t uartBase, uint8_t *data, uint8_t size )
{
    uint8_t lCnt = 0;

    USCI_A_UART_clearInterrupt( USCI_A1_BASE, USCI_A_UART_TRANSMIT_INTERRUPT );
    __disable_interrupt();

    for( lCnt=0; lCnt<size;lCnt++)
    {
        USCI_A_UART_transmitData( USCI_A1_BASE, *(data+lCnt) );
    }

    return 1;
}
/*********************************************************************************
 *Name :- uartHalTxINT
 *Para1:- pointer to string ( buffer)
 *Para2:- size of the string.
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool uartHalTxINT( uint16_t uartBase, uint8_t *data, uint8_t size )
{
    uartTxBuffSize = size;
    uartTxBuffPtr = data;
    uartTxWriteCmpt = 0;
    USCI_A_UART_clearInterrupt( USCI_A1_BASE, USCI_A_UART_TRANSMIT_INTERRUPT );
    USCI_A_UART_enableInterrupt(USCI_A1_BASE, USCI_A_UART_TRANSMIT_INTERRUPT );

    //while( USCI_A_UART_RECEIVE_INTERRUPT == USCI_A_UART_getInterruptStatus(USCI_A1_BASE, USCI_A_UART_RECEIVE_INTERRUPT));
    USCI_A_UART_transmitData( USCI_A1_BASE, *(uartTxBuffPtr + uartTxWriteCmpt++));
    __enable_interrupt();
    return 1;
}
