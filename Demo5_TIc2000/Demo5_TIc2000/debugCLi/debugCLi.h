/*
 * debugCLi.h
 *
 *  Created on: 10-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#ifndef DEBUGCLI_DEBUGCLI_H_
#define DEBUGCLI_DEBUGCLI_H_

#include "device.h"
#include "board.h"
#include "c2000ware_libraries.h"
#include "c2000_freertos.h"

#include"projectConfiguration.h"

void debugCliTask(void * pvParameters);

void debugPrint( void );

bool debugText( const char *debugMsg );
bool debugValue( int64_t value, uint8_t baseValue );
bool debugTextValue( const char *debugMsg, int64_t value, uint8_t baseValue );


bool IntToText(int64_t value, uint8_t base, char * str );
void reverseStr(char *str, uint8_t size);


#endif /* DEBUGCLI_DEBUGCLI_H_ */
