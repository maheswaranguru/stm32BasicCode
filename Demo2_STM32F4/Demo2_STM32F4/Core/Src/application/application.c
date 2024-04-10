/*
 * application.c
 *
 *  Created on: Jun 22, 2023
 *      Author: maheswaran_gurusamy
 */

#include "application.h"
#include "freertos.h"
#include "task.h"
#include "queue.h"

#include "debugCli.h"
#include "actuator.h"

#include "projectConfigurations.h"

extern SemaphoreHandle_t xMutexDebugUart;

const sysTask_t taskList[ MAXIMUM_TASK ] ={

		{ (TaskFunction_t) debugCliTask, 	"debugconsoleApp", 256	, NULL, 0, 0 },
		{ (TaskFunction_t) actuatorTask, 	"ButtonSwitchTask", 128, NULL, 0, 0 },
};


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

    /* Start scheduler */
		vTaskStartScheduler();

}
