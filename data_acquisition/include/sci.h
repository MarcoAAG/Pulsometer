/** @file sci.h
*   @brief Serial Communication Interface Driver Header File
*   @date 20.April.2021
*   @version 1.0
*
*   This file contains:
*   - Definitions
*   - Types
*   - Interface Prototypes
*   .
*   which are relevant for the UART driver.
*/

#ifndef INCLUDE_SCI_H_
#define INCLUDE_SCI_H_

#include"includes.h"

#pragma once

/* SCI Interface Functions */
void sciInit(void);
void sciSetPorts(uint8_t _tx, uint8_t _rx);
void sendByte(uint8_t _c);
void sciSendData(uint8_t *text, uint32_t length);
void sendString(uint8_t *_str);
uint8_t sciRead(void);

#endif /* INCLUDE_SCI_H_ */
