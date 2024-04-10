/******************************************************************************
 *                                                                            *
 * File name    :   debugCli.h
 * Date         :   30-05-2023
 * Created by   :   Maheswaran G
 * Description  :
 * This file provides functions of main Cli API for debug message output.
 *
 * Copyright (C) Sanmina Tech Service
 ******************************************************************************/
#ifndef SRC_DEBUGCLI_H_
#define SRC_DEBUGCLI_H_


#include <stdint.h>
#include "projectConfigurations.h"

//!< **************** RTOS related includes *******************
#ifdef RTOS
#include "FreeRTOS.h"
#include "semphr.h"

#include "cmsis_os.h"
#endif

//extern QueueHandle_t gDebugConsoleQ;

void debugCliTask(void);

bool debugTaskGetStatus( void );

bool debugText( const char *debugMsg );
bool debugValue( int64_t value, uint8_t baseValue );
bool debugTextValue( const char *debugMsg, int64_t value, uint8_t baseValue );


bool IntToText(int64_t value, uint8_t base, char * str );
void reverseStr(char *str, uint8_t size);


#endif
