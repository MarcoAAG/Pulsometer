/*
 *  @main.c
 *  @brief main contents
 *  @version 1.0
 *
 *  Created on: April 20, 2021
 *  Author: MarcoAAG
 *
 *  **Free to use**
 */


#include"includes.h"

//Add custom libraries
#include "clock.h"
#include "timer.h"
#include "sci.h"

#define RX BIT1     //P1.1
#define TX BIT2     //P1.2

uint8_t FlagTimer = 0;

void initMapReg(void);
void protocolInit(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	uint8_t data2send = 0;
	uint8_t i = 0;

	initMapReg();
	clockInit();
	timerAInit((uint32_t)5000);   //Rate in micro seconds
	sciInit();
	sciSetPorts(TX, RX);

    /* enable interrupts */
    _BIS_SR(GIE);

    while(1){

        if(FlagTimer){

            if(i<100){
                data2send += 1;
            }
            if(i>=100 && i<200){
                data2send -= 1;
            }
            if(data2send == 1){
                i = 0;
            }
            i += 1;

            P1OUT ^= BIT6;
            sciSendData((uint8_t *)&data2send, 1);
            sendString("\r\n");
            FlagTimer = 0;
        }

    }

	return 0;
}


/* Interrupt service */
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    FlagTimer = 1;
}
#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCI0RX_ISR(void)
{
    //if(UCA0RXBUF == 49)

}


void initMapReg(void)
{
    P1OUT = 0;
    P1SEL = 0;
    P1SEL2 = 0;

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;
}

void protocolInit(void)
{

}
