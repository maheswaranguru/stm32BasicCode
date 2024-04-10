/*
 * debugCli.c
 *
 *  Created on: 22-Apr-2023
 *      Author: maheswaran_gurusamy
 *
 */
#include <string.h>
#include <stdlib.h>

#include "debugCli.h"
#include "debugCliWrapper.h"


UART_CONFIG_TYPE debugPort;          //!< There is a LL driver / Generated code of STM32 FW have limitation
// ... We need to keep the variable name for uart configuration structer

#ifdef RTOS
	SemaphoreHandle_t xMutexDebugUart = NULL;
	QueueHandle_t gDebugConsoleQ;
#else
uint8_t xMutexDebugUart = ZERO;
uint8_t gDebugConsoleQ = ZERO;
#endif

void HAL_UART_ErrorCallback( UART_CONFIG_TYPE *huart );

static bool debugConsoleInit( void );
static bool debugConsoleFlushOut( void );

static bool debugConsoletxCptF = true;
volatile static bool uartError_f = false;

//!< @@@@@@@@@@@@@@@@@@@@ RING BUFF CONFIG @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

void addToRing( char *strPtr, unsigned int strLeg );

unsigned char debugOutBuffer[ RING_BUFF_SIZE ] = { 0 };
unsigned int writePtr = ZERO;		//!< Initialize both to zero.
unsigned int readPtr = ZERO;

//!< @@@@@@@@@@@@@@@@@@@@ END RING BUFF @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

char tempBuff[ 50 ] = { 0 };
bool mDebugConInit = false;

//SemaphoreHandle_t xMutexDebugUart = NULL;

//!< @@@@@@@@@@@@@@@@ Receive Command @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

#define MAX_CMD_LENGTH	25

volatile static uint16_t RxByteCnt = ZERO;
volatile static bool cmdRx_t = false;
volatile static uint8_t RxString[ MAX_CMD_LENGTH ] = { 0 };
volatile static uint8_t inputString[ MAX_CMD_LENGTH ] = { 0 };
volatile static uint8_t command[MAX_CMD_LENGTH] = {0};
volatile static bool cmdError = false;

static uint8_t rxByte = ZERO;
bool txStatus = false;

void debugCliTask( void )
{

    if ( true != mDebugConInit )
    {
    	mDebugConInit = debugConsoleInit();
        writePtr = ZERO;
        readPtr = ZERO;
        TASK_DELAY( 100 );
        if ( UART_READY  == GET_UART_STATUS( &debugPort ) )
        {
            debugText( "\n\r********************************************" );
            debugText( "\n\rThis is Base code Generation" );
            debugText( "\n\r############    BASE CODE     ############" );
            debugText( "\n\r*********************************************\n\r" );
        }
        else
        {
            //while(1);
        }
    }

    UART_RECEIVE_IT( &debugPort, &rxByte, 1 );  //!< Enable Receiver for user CMD.

    TASK_LOOP
    {
        if ( ( UART_READY == ((int) GET_UART_STATUS( &debugPort ) & UART_READY ) ) && ( writePtr != readPtr ) )
        {
            txStatus = debugConsoleFlushOut();
        }

        if ( ( true == uartError_f ) || ( NO_ERROR != CHECK_PORT_ERROR( &debugPort ) ) )
        {
            DEINT_PORT( &debugPort );
            debugConsoleInit();

        }
        if ( false == txStatus )
        {
            STOP_DEBUG_PORT( &debugPort );
            txStatus = true;
            writePtr = ZERO;
            readPtr = ZERO;
        }

        if ( true == cmdRx_t )
        {
            cmdRx_t = false;

            debugText( ( char* ) inputString );
        }

        TASK_DELAY(100);

    }
}

///*********************************************************************************
// *Name :- debugConsoleInit
// *Para1:- N/A
// *Return:-N/A
// *Details:-  Initialize the uart used for debug.
// **********************************************************************************/
//static bool debugConsoleInit( void )
//{
//
//    bool returnValue = true;
//    /* USER CODE END USART2_Init  */
//    debugPort.Instance = DEBUG_UART;
//    debugPort.Init.BaudRate = BAUDRATE;
//    debugPort.Init.WordLength = UART_WORDLENGTH_8B;
//    debugPort.Init.StopBits = UART_STOPBITS_1;
//    debugPort.Init.Parity = UART_PARITY_NONE;
//    debugPort.Init.Mode = UART_MODE_TX_RX;
//    debugPort.Init.HwFlowCtl = UART_HWCONTROL_NONE;
//    debugPort.Init.OverSampling = UART_OVERSAMPLING_16;
//
//    if ( HAL_UART_Init( &debugPort ) != HAL_OK )
//    {
//        returnValue = false;
//        // Error_Handler(); 	//!< NEED TO ADD error handling function.
//    }
//    else
//    {
//
//    }
//
//    writePtr = ZERO;
//    readPtr = ZERO;
//    mDebugConInit = returnValue;
//    return ( returnValue );
//
//}
/*********************************************************************************
 *Name :- debugText
 *Para1:- debugMsg
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool debugText( const char *debugMsg )
{
    char *msg = ( char* ) debugMsg;
    bool returnValue = false;

    if ( true == TAKE_SEMAPHORE( xMutexDebugUart, portMAX_DELAY ) )
    {
        if ( mDebugConInit )
        {
            addToRing( msg, strlen( msg ) );
        }

        while ( true != GIVE_SEMAPHORE( xMutexDebugUart ))
            ;
    }

    return ( returnValue );
}
/*********************************************************************************
 *Name :- debugValue
 *Para1:- debugMsg
 *Para2:- value
 *Para3:- baseValue
 *Return:- PASS / FAIL
 *Details:-
 **********************************************************************************/
bool debugValue( int64_t value, uint8_t baseValue )
{

    IntToText( value, baseValue, tempBuff );
    debugText( tempBuff );

    return 1;
}
/*********************************************************************************
 *Name :- debugTextValue
 *Para1:- debugMsg
 *Para2:- value
 *Para3:- baseValue
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool debugTextValue( const char *debugMsg, int64_t value, uint8_t baseValue )
{
    bool returnValue = false;
    char localbuff[ 100 ] = { 0 };

    strcpy( localbuff, debugMsg );

    IntToText( value, baseValue, tempBuff );
    strcat( localbuff, tempBuff );
    debugText( localbuff );

    return returnValue;
}
/*********************************************************************************
 *Name :- IntToText
 *Para1:- value
 *Para2:- base ( Numerical base )
 *Para3:- str  ( pointer to buffer )
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool IntToText( int64_t value, uint8_t base, char *str )
{
    int64_t temp = ZERO;
    uint8_t i = ZERO;
    int64_t division = value;

    if ( ZERO == division )
    {
        *( str + i++ ) = temp | 0x30;
        *( str + i ) = '\0';
    }
    else if ( ZERO != base )
    {
        if ( ZERO > division )
        {
            division = abs( division );
        }
        while ( ZERO != division)
        {
            temp = division % base;
            division /= base;

            if ( ( base == HEX ) && ( temp >= DECIMAL ) )
            {
                *( str + i++ ) = ( temp - DECIMAL ) + 'A';
            }
            else
            {
                *( str + i++ ) = temp | 0x30;
            }
        }
        if ( base == HEX )
        {
            *( str + i++ ) = 'x';
            *( str + i++ ) = '0';
            *( str + i++ ) = ' ';
        }
        else if ( base == BINARY )
        {
            *( str + i++ ) = '.';
            *( str + i++ ) = 'b';
            *( str + i++ ) = ' ';
        }

        if ( ( ZERO > value ) && ( base == DECIMAL ) )
        {
            *( str + i++ ) = '-';
        }

        *( str + i ) = '\0';
        reverseStr( str, i );
    }

    return true;
}
/*********************************************************************************
 *Name :- reverseStr
 *Para1:- pointer to string ( buffer)
 *Para2:- size of the string.
 *Return:-N/A
 *Details:-
 **********************************************************************************/
void reverseStr( char *str, uint8_t size )
{
    uint8_t i = size - 1;
    uint8_t j = ZERO;
    uint8_t temp = i;

    do
    {
        temp = *( str + j );
        *( str + j ) = *( str + i );
        *( str + i ) = temp;
        if ( i == ZERO )
            break;		//!<  NEED TO FIND A BETTER WAY TO HANDLE THE UNDERFLOW !
        i = i - 1;
        j = j + 1;
        temp = ( unsigned char ) i / 2;
    }
    while (i > j);

}
/*********************************************************************************
 *Name :- addToRing
 *Para1:- pointer to string ( buffer)
 *Para2:- size of the string.
 *Return:-N/A
 *Details:-
 **********************************************************************************/
void addToRing( char *strPtr, unsigned int strLeg )
{
    unsigned int remaingBuff = ( RING_BUFF_SIZE - writePtr ) - 1;

    if ( remaingBuff > strLeg )
    {
        memcpy( &debugOutBuffer[ writePtr ], strPtr, strLeg );
        writePtr += strLeg;
    }
    else
    {
        memcpy( &debugOutBuffer[ writePtr ], strPtr, remaingBuff );
        memcpy( debugOutBuffer, ( unsigned char* ) ( strPtr + remaingBuff ), ( strLeg - remaingBuff ) );

        writePtr = ( strLeg - remaingBuff );
    }

}

/*********************************************************************************
 *Name :- addToRing
 *Para1:- last send out pointer ( read pointer )
 *Para2:- Upto which pointer/place ( write pointer ).
 *Return:-N/A
 *Details:-
 **********************************************************************************/
static bool debugConsoleFlushOut( void )
{
    bool returnValue = true;

    if ( ( mDebugConInit ) && ( writePtr != readPtr ) )
    {

        if ( writePtr > readPtr )
        {
            if ( HAL_OK != UART_SENT_IT( &debugPort, ( uint8_t* ) ( debugOutBuffer + readPtr ), ( uint16_t ) ( writePtr - readPtr ) ) )
            {
                returnValue &= false;
            }
            else
            {
                readPtr = writePtr;
            }

        }
        else
        {
            if ( HAL_OK != UART_SENT_IT( &debugPort, ( uint8_t* ) ( debugOutBuffer + readPtr ), ( uint16_t ) ( RING_BUFF_SIZE - readPtr ) ) )
            {
                returnValue &= false;
            }
            else
            {
                debugConsoletxCptF = false;
                readPtr = ZERO;
            }
        }
    }

    return returnValue;
}

/*********************************************************************************
 *Name :- debugTaskGetStatus
 *Para1:- last send out pointer ( read pointer )
 *Para2:- Upto which pointer/place ( write pointer ).
 *Return:-N/A
 *Details:-
 **********************************************************************************/
bool debugTaskGetStatus( void )
{
    return ( debugConsoletxCptF );
}

/***************************************************************************************
 * @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
 * ************************************************************************************/
void HAL_UART_ErrorCallback( UART_CONFIG_TYPE *huart )
{
    //while(1);
    uartError_f = true;
}

void HAL_UART_TxCpltCallback( UART_CONFIG_TYPE *huart )
{
    //debugConsoletxCptF = true;
    return;
}

void HAL_UART_RxCpltCallback( UART_CONFIG_TYPE *huart )  //####### This should be change in Wrapper file !!!!!
{

    RxString[ RxByteCnt ] = rxByte;

    if ( ( '\r' == rxByte ) || ( '\n' == rxByte ) )
    {
        if ( ZERO != RxByteCnt )
        {
            strncpy( ( char* ) inputString, ( char* ) RxString, ( RxByteCnt ) );
            inputString[ RxByteCnt ] = '\0';
            RxByteCnt = ZERO;
            cmdRx_t = true;
        }
        else
        {

        }

    }
    else
    {
        if ( ( MAX_CMD_LENGTH - 1 ) <= RxByteCnt++ )
        {
            UART_RECEIVE_IT( &debugPort, &rxByte, 1 );
            RxByteCnt = ZERO;
        }
    }

    UART_RECEIVE_IT( &debugPort, &rxByte, 1 );

    return;
}

