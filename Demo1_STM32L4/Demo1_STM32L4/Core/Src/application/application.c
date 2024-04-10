/*
 * application.c
 *
 *  Created on: 30-May-2023
 *      Author: maheswaran_gurusamy
 */
#include "application.h"
#include "debugCli.h"

void application( void )
{
	while(1)
	{
		debugCliTask();
		//actuatorTask();

		HAL_Delay(100);

	}
}
