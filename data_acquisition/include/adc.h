/** @file adc.h
*   @brief ADC Header File
*   @date 28.April.2021
*   @version 1.0
*
*   This file contains:
*   - Definitions
*
*   which are relevant for the project.
*/

#ifndef INCLUDE_ADC_H_
#define INCLUDE_ADC_H_

#pragma once

#include"includes.h"

void adcInit(void);
uint16_t readSingChanAdc(void);

#endif /* INCLUDE_ADC_H_ */
