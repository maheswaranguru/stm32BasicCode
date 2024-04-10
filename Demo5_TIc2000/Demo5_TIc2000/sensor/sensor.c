/*
 * sensor.c
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#include"sensor.h"



void sensorTask(void * pvParameters)
{
    static uint16_t counter = 0;

    vTaskDelay(5000);


    while(1)
    {
        debugTextValue(" \r\nMessage From Sensor Task ", counter++, DECIMAL);
        GPIO_togglePin(LED2);
         vTaskDelay(3000);
    }
}
