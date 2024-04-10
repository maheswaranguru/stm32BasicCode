/*
 * application.c
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#include<string.h>

#include "application.h"
#include "debugCLi.h"
#include "sensor.h"

extern SemaphoreHandle_t xMutexDebugUart;


void sensorTask(void * pvParameters);

__interrupt void INT_debugPort_TX_ISR(void);
__interrupt void INT_debugPort_RX_ISR(void);


const sysTask_t taskList[ MAXIMUM_TASK ] ={

        { (TaskFunction_t) debugCliTask,    "debugconsoleApp",  254,    NULL, 0, 0 },
        { (TaskFunction_t) sensorTask,      "sensorTask",       254,    NULL, 0, 0 },
};

/*********************************************************************************
 *Name :- application
 *Para1:- N/A
 *Return:-N/A
 *Details:- This is the place to create a required task and other tools.
 **********************************************************************************/
void application( void )
{

    uint8_t i = 0;

    for ( i = 0; i< MAXIMUM_TASK; i++ )
    {
        xTaskCreate( taskList[i].vTaskfunPtr, taskList[i].vTaskName, taskList[i].stacksize, taskList[i].VTaskparaeter, taskList[i].VtaskPriority, taskList[i].pxCreatedTask );
    }

    xMutexDebugUart = xSemaphoreCreateMutex();
    if( NULL == xMutexDebugUart )
    {
        // TURN ON ERROR ;
    }
}





__interrupt void INT_debugPort_TX_ISR(void)
{

}
__interrupt void INT_debugPort_RX_ISR(void)
{

}
