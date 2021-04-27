/** @file sci.c
*   @brief Serial Communication Interface Driver Implementation File
*   @date 20.April.2021
*   @version 1.0
*   This file contains Timers Implementation.
*/
#include"sci.h"

/** @fn void sciInit(void)
*   @brief Initialize uart mode
*
*   Steps to initializes and set UART module
*   1. Set UCSWRST (BIS.B #UCSWRST,&UCAxCTL1)
*   2. Initialize all USCI registers with UCSWRST = 1 (including UCAxCTL1)
*   3. Configure ports.
*   4. Clear UCSWRST via software (BIC.B #UCSWRST,&UCAxCTL1)
*   5. Enable interrupts (optional) via UCAxRXIE and/or UCAxTXIE
*/
void sciInit(void)
{
    /* #1 */
    UCA0CTL1 = UCSWRST;

    /* #2 */
    UCA0CTL1 |= UCSSEL_2; // SMCLK

    /* #3 */
    UCA0CTL0 = 0;
    UCA0CTL0 |= 0;
    // UCA0CTL0 = 0;
    UCA0BR0 = 104;      /*  104 From datasheet table- */
    UCA0BR1 = 0;        /* -selects baudrate =9600,clk = SMCLK*/
    UCA0MCTL = UCBRS_6; /* Modulation value = 6 from datasheet*/
    /* $4 */
    UCA0CTL1 &= ~UCSWRST;

    /* #5 */
    IE2 |= UCA0RXIE;
}

void sciSetPorts(uint8_t _tx, uint8_t _rx)
{
    P1SEL |= _tx | _rx;
    P1SEL2 |= _tx | _rx;
}

/** @fn void sendByte(uint8_t _c)
*   @brief Send Bytes
*   @param[in] _c  - byte to transfer
*
*   Sends a single byte in polling mode, will wait in the
*   routine until the transmit buffer is empty before sending
*   the byte Use sciIsTxReady to check for Tx buffer empty
*   before calling sciSendByte to avoid waiting.
*/
void sendByte(uint8_t _c)
{
    while (!(IFG2 & UCA0TXIFG))
        ;
    UCA0TXBUF = _c;
}

/** @fn vvoid sendString(uint8_t *_str)
*   @brief Send Bytes
*   @param[in] _str  - str to transfer
*
*   Sends a string in polling mode, will wait in the
*   routine until the transmit buffer is empty before sending
*   the byte Use sciIsTxReady to check for Tx buffer empty
*   before calling sciSendByte to avoid waiting.
*/
void sendString(uint8_t *_str)
{
    while (*_str){
        sendByte(*_str);
        _str++;
    }
}

/** @fn void sciSendData(uint8_t *text, uint32_t length)
*   @brief Send Bytes
*   @param[in] text  - data to transfer
*   @param[in] lenght - lenght of string
*
*   Sends a string in polling mode, will wait in the
*   routine until the transmit buffer is empty before sending
*   the byte
*/
void sciSendData(uint8_t *text, uint32_t length)
{
    uint8_t txt = 0;
    uint8_t txt1 = 0;

#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
    text = text + (length - 1);
#endif

    while (length--)
    {
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
        txt = *text--;
#else
        txt = *text++;
#endif

        txt1 = txt;

        txt &= ~(0xF0);
        txt1 &= ~(0x0F);
        txt1 = txt1 >> 4;

        if (txt <= 0x9)
        {
            txt += 0x30;
        }
        else if (txt > 0x9 && txt <= 0xF)
        {
            txt += 0x37;
        }
        else
        {
            txt = 0x30;
        }

        if (txt1 <= 0x9)
        {
            txt1 += 0x30;
        }
        else if ((txt1 > 0x9) && (txt1 <= 0xF))
        {
            txt1 += 0x37;
        }
        else
        {
            txt1 = 0x30;
        }
        sendByte(txt1);
        sendByte(txt);
    };
}
