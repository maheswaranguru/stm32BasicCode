/*
 * application.h
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#ifndef APPLICATION_APPLICATION_H_
#define APPLICATION_APPLICATION_H_

#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include "c2000_freertos.h"

#include"projectConfiguration.h"

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
    SENSOR_TASK,
    MAXIMUM_TASK
}sysTaskList_t;


void application( void );



#endif /* APPLICATION_APPLICATION_H_ */
