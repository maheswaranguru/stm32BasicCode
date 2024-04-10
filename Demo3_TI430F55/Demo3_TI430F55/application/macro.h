/*
 * macro.h
 *
 *  Created on: 14-Jul-2023
 *      Author: maheswaran_gurusamy
 */

#ifndef SRC_APPLICATION_MACRO_H_
#define SRC_APPLICATION_MACRO_H_

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef HIGH
#define HIGH 1
#endif

#ifndef LOW
#define LOW 0
#endif

//#ifndef SET
//#define SET 1
//#endif

#ifndef CLEAR
#define CLEAR 0
#endif

#ifndef ZERO
#define ZERO 0
#endif

#define DECIMAL 10
#define HEX     16
#define BINARY  2

#ifndef RTOS
	#define TASK_LOOP
	#define TAKE_SEMAPHORE(a, b) (1)
	#define GIVE_SEMAPHORE(a) (1)
	#define TASK_DELAY(a)
#else
	#include "FreeRTOS.h"
	#include "task.h"
	#include "queue.h"
	#define TASK_LOOP for(;;)

	#define TAKE_SEMAPHORE(a, b) xSemaphoreTake(a, b)
	#define GIVE_SEMAPHORE(a) xSemaphoreGive(a)
	#define TASK_DELAY(a)	vTaskDelay(a)

#ifndef portMAX_DELAY
#define portMAX_DELAY  0xff
#endif

#endif

#endif /* SRC_APPLICATION_MACRO_H_ */
