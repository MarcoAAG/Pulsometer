/** @file adc.c
*   @brief ADC 10 bits Driver Implementation File
*   @date 28.April.2021
*   @version 1.0
*   This file contains ADC Implementation.
*/

#include"adc.h"

/** @fn void adcInit(void)
*   @brief initialize adc register
*
*   This function initializes the ADC10 module.
*   For One single channel
*/
void adcInit(void)
{
    ADC10CTL0 &= ~ENC;

    ADC10CTL1 = INCH_0 | SHS_0 | ADC10DIV_0 | ADC10SSEL_0 | CONSEQ_0;

    ADC10AE0 = BIT0;

    ADC10CTL0 = SREF_0 | ADC10SHT_0 | ADC10ON;
}

/** @fn uint16_t readSingChanAdc(void)
*   @brief Read channel
*   @param[out] 10 bits data from sensor
*
*   read data from adc bit0
*/
uint16_t readSingChanAdc(void)
{
    uint16_t data_raw;

    ADC10CTL0 |= ENC | ADC10SC;
    data_raw = ADC10MEM;
    ADC10CTL0 &= ~ENC | ~ADC10SC;
    return data_raw;
}
