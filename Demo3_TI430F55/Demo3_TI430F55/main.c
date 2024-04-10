#include <msp430.h> 
#include "driverlib.h"
#include "MSP430HAL.h"
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    sysClkConfig();  //!< System clock configuration

    GPIO_setAsOutputPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setAsOutputPin( GPIO_PORT_P4, GPIO_PIN7 );
    GPIO_setOutputHighOnPin( GPIO_PORT_P1, GPIO_PIN0 );
    GPIO_setOutputHighOnPin( GPIO_PORT_P4, GPIO_PIN7 );

    while(1)
    {

    }

	return 0;
}
