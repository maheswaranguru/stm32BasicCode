/*
 * projectConfiguration.h
 *
 *  Created on: 05-Apr-2024
 *      Author: maheswaran_gurusamy
 */

#ifndef APP_APPLICATION_PROJECTCONFIGURATION_H_
#define APP_APPLICATION_PROJECTCONFIGURATION_H_


#ifndef VERSION
#define VERSION "00.00.01"  //!< This is the software version and update as per Sanmina guidelines
#endif


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

#ifndef SET
#define SET 1
#endif

#ifndef CLEAR
#define CLEAR 0
#endif

typedef enum
{
    ZERO,
    ONE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN
}e_number_t;



#define DECIMAL 10
#define HEX     16
#define BINARY  2





#endif /* APP_APPLICATION_PROJECTCONFIGURATION_H_ */
