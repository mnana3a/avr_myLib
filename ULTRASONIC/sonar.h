#ifndef SONAR_H_
#define SONAR_H_
 
#ifndef F_CPU
    #define F_CPU 16000000UL
#endif
 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
 
// Modify Maximum response time and delay as required
// MAX_RESP_TIME : default: 300
// DELAY_BETWEEN_TESTS : default: 50

#define TRIG_DDR    DDRD            // Trigger Port
#define TRIG_PORT   PORTD
#define TRIG_PIN    PIND
#define TRIG_BIT    PD0             // Trigger Pin
 
#define ECHO_DDR    DDRD            // Echo Port
#define ECHO_PORT   PORTD
#define ECHO_PIN    PIND
#define ECHO_BIT    PD1             // Echo Pin
 
// Speed of sound
// Default: 343 meters per second in dry air at room temperature (~20C)
#define SPEED_OF_SOUND  343
#define MAX_SONAR_RANGE 10          // This is trigger + echo range (in meters) for SR04
#define DELAY_BETWEEN_TESTS 500     // Echo canceling time between sampling. Default: 500us
#define TIMER_MAX 65535             // 65535 for 16 bit timer and 255 for 8 bit timer
 
#define TRIG_ERROR -1
#define ECHO_ERROR -2
 
#define CYCLES_PER_US (F_CPU/1000000)// instructions per microsecond
#define CYCLES_PER_MS (F_CPU/1000)      // instructions per millisecond
// Timeout. Decreasing this decreases measuring distance
// but gives faster sampling
#define SONAR_TIMEOUT ((F_CPU*MAX_SONAR_RANGE)/SPEED_OF_SOUND)
 
#define TRIG_INPUT_MODE() TRIG_DDR &= ~(1<<TRIG_BIT)
#define TRIG_OUTPUT_MODE() TRIG_DDR |= (1<<TRIG_BIT)
#define TRIG_LOW() TRIG_PORT &= ~(1<<TRIG_BIT)
#define TRIG_HIGH() TRIG_PORT |=(1<<TRIG_BIT)
 
#define ECHO_INPUT_MODE() ECHO_DDR &= ~(1<<ECHO_BIT)
#define ECHO_OUTPUT_MODE() ECHO_DDR |= (1<<ECHO_BIT)
#define ECHO_LOW() ECHO_PORT &= ~(1<<ECHO_BIT)
#define ECHO_HIGH() ECHO_PORT |=(1<<ECHO_BIT)
 
#define CONVERT_TO_CM ((10000*2)/SPEED_OF_SOUND)    // or simply 58
 
/** ...- . . .-. --- -... --- -
 * @brief   Initiate Ports for Trigger and Echo pins
 * @param   void
 * @return  none
*/
void init_sonar(void);
 
/**  ...- . . .-. --- -... --- -
 * @brief   Send 10us pulse on Ultrasonic Trigger pin
 * @param   void
 * @return  none
*/
void trigger_sonar(void);
 
/**  ...- . . .-. --- -... --- -
 * @brief   Calculate and store echo time and return distance
 * @param   void
 * @return  unsigned int
 * Usage    int foo = read_sonar();
*/
unsigned int read_sonar(void);
 
#endif /* SONAR_H_ */
