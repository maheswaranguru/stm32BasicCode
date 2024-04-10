/*
 * application.h
 *
 *  Created on: Jun 22, 2023
 *      Author: maheswaran_gurusamy
 */

#ifndef SRC_APPLICATION_APPLICATION_H_
#define SRC_APPLICATION_APPLICATION_H_

#include "freertos.h"
#include "task.h"

#include "projectConfigurations.h"
#include "debugCli.h"

typedef struct taskpara
{
	TaskFunction_t  vTaskfunPtr;
    char vTaskName[configMAX_TASK_NAME_LEN];
	unsigned short stacksize;
	void * VTaskparaeter;
	UBaseType_t VtaskPriority;
	TaskHandle_t *pxCreatedTask;
}sysTask_t;


typedef enum{
	DEBUGCLI_TASK,
	BUTTON_TASK,
	MAXIMUM_TASK
}sysTaskList_t;

void application( void );


#endif /* SRC_APPLICATION_APPLICATION_H_ */
