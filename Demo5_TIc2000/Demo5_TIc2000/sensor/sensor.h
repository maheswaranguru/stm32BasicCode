/*
 * sensor.h
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#ifndef SENSOR_SENSOR_H_
#define SENSOR_SENSOR_H_
#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include "c2000_freertos.h"

#include"projectConfiguration.h"
#include"debugCLi.h"

void sensorTask(void * pvParameters);


#endif /* SENSOR_SENSOR_H_ */
