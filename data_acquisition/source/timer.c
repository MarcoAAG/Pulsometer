/** @file timer.c
*   @brief timer Driver Implementation File
*   @date 20.April.2021
*   @version 1.0
*   This file contains Timers Implementation.
*/

#include"timer.h"


/** @fn void timerAInit(uint32_t _delay)
*   @brief Initialize timer A
*
*   @input _delay [in micro seconds, overflow after 0.25 seg]
*   Set and configure at custom time
*/
void timerAInit(uint32_t _delay)
{
    double period = 7.27; /* period in us*/
    uint16_t timer_ratio = (uint16_t) (_delay / period);

    TA0CTL = TASSEL_2 | ID_3 | MC_1 | TACLR; /* TASSEL_2 -> SMCLK */
                                             /* ID_3 -> Source clock divided by 8 */
                                             /* MC_1 -> Up mode */
                                             /* TACLR -> timer clear */
    TACCR0 = timer_ratio - 1;                /* count of Timer_A */
    CCTL0 = CCIE;                            /* Capture/compare interrupt enable. */
}
