/*
 * debugCLi.c
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */
#include<string.h>

#include"debugCLi.h"

#ifndef debugPort
#define debugPort debugPort_BASE
#endif

#ifndef CLI_RING_BUFF_SIZE
#define CLI_RING_BUFF_SIZE 250
#endif


static bool debugConsoleFlushOut( void );
void addToRing( char *strPtr, unsigned int strLeg );


unsigned char debugOutBuffer[ CLI_RING_BUFF_SIZE ] = { 0 };
char tempStrBuff[ 50 ] = { 0 };     //!< To store string of the value to print.
unsigned int writePtr = ZERO;       //!< Write pointer for ring Buffer. Initialize both to zero.
unsigned int readPtr = ZERO;       //!< Read pointer for ring Buffer.
bool txStatus = false;

SemaphoreHandle_t xMutexDebugUart = NULL;   //!< Mutex for debugCLi Api's



char testmsg[]= "\r\nThis is Test Message to check Debug task\r\n";



/*********************************************************************************
 *Name :- debugCliTask
 *Para1:- Task parameter pass by application
 *Return:-N/A
 *Details:-  Main task for Cli module. The intention is mainly for debug and test
 **********************************************************************************/
void debugCliTask(void * pvParameters)
{

    SCI_writeCharArray(debugPort_BASE, testmsg, strlen(testmsg));
    vTaskDelay(1000);

    writePtr = ZERO;
    readPtr = ZERO;
    debugText( "\n\r*********************************************\n\r" );
    debugText( "\n\r**********Phinergy - 4K *****************" );
    debugText( "\n\rApplication Version :      ############" );
    debugText( "\n\r*********************************************\n\r" );

    debugPrint();
    vTaskDelay(1000);


     while(1)
    {
        if( writePtr != readPtr )
        {
            txStatus = debugConsoleFlushOut();
        }
        GPIO_togglePin(LED1);
        vTaskDelay(1000);

    }
}
/*********************************************************************************
 *Name :- debugPrint
 *Para1:- N/A
 *Return:-N/A
 *Details:- Need to consider future use for formated print
 **********************************************************************************/
void debugPrint( void )
{
    const char msg[] = "\r\nHellow LED toggle";
    SCI_writeCharArray(debugPort, (uint16_t*)msg, strlen(msg));
}
/*********************************************************************************
 *Name :- debugText
 *Para1:- debugMsg
 *Return:-N/A
 *Details:- it is to print a given test in console.
 **********************************************************************************/
bool debugText( const char *debugMsg )
{
    char *msg = ( char* ) debugMsg;
    bool returnValue = false;

    if ( pdTRUE == xSemaphoreTake( xMutexDebugUart, portMAX_DELAY ) )
    {
        addToRing( msg, strlen( msg ) );
        returnValue = xSemaphoreGive( xMutexDebugUart );
    }

    return ( returnValue );
}
/*********************************************************************************
 *Name :- debugValue
 *Para1:- debugMsg
 *Para2:- value
 *Para3:- baseValue
 *Return:- PASS / FAIL
 *Details:- This will print a value in console with given base value.
 **********************************************************************************/
bool debugValue( int64_t value, uint8_t baseValue )
{

    if( ZERO >= baseValue )
        baseValue = 10; //!< If the base value is negative or zero or not added , default is 10.

    IntToText( value, baseValue, tempStrBuff );
    debugText( tempStrBuff );

    return 1;
}
/*********************************************************************************
 *Name :- debugTextValue
 *Para1:- debugMsg
 *Para2:- value
 *Para3:- baseValue
 *Return:-N/A
 *Details:- This will print a test along with a value given with base value
 **********************************************************************************/
bool debugTextValue( const char *debugMsg, int64_t value, uint8_t baseValue )
{
    bool returnValue = false;
    char localbuff[ 100 ] = { 0 };

    strcpy( localbuff, debugMsg );

    IntToText( value, baseValue, tempStrBuff );
    strcat( localbuff, tempStrBuff );
    debugText( localbuff );

    return returnValue;
}
/*********************************************************************************
 *Name :- IntToText
 *Para1:- value
 *Para2:- base ( Numerical base )
 *Para3:- str  ( pointer to buffer )
 *Return:-N/A
 *Details:- convert given value to string
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
 *Details:- To reverse the string.
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
            break;      //!<  NEED TO FIND A BETTER WAY TO HANDLE THE UNDERFLOW !
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
 *Details:- Add given stream of data / byte to debugOutBuffer
 **********************************************************************************/
void addToRing( char *strPtr, unsigned int strLeg )
{
    unsigned int remaingBuff = ( CLI_RING_BUFF_SIZE - writePtr ) - 1;

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
 *Name :- debugConsoleFlushOut
 *Para1:- N/A
 *Return:- true = sent successfully ; false = fail to sent
 *Details:-
 **********************************************************************************/
static bool debugConsoleFlushOut( void )
{
    bool returnValue = true;

    if ( writePtr != readPtr )
    {

        if ( writePtr > readPtr )
        {
            SCI_writeCharArray( debugPort, ( uint16_t* ) ( debugOutBuffer + readPtr ), ( uint16_t ) ( writePtr - readPtr ) );
            readPtr = writePtr;
        }
        else
        {
            SCI_writeCharArray( debugPort, ( uint16_t* ) ( debugOutBuffer + readPtr ), ( uint16_t ) ( CLI_RING_BUFF_SIZE - readPtr ) );
           // debugConsoletxCptF = false;
            readPtr = ZERO;
        }
    }

    return returnValue;
}
