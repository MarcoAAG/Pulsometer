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
#include "adc.h"

#define RX BIT1     //P1.1
#define TX BIT2     //P1.2

uint8_t FlagTimer = 0;
//uint8_t FlagRX = 0;
uint8_t Connected = 0;

void initMapReg(void);
void protocolInit(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	uint16_t data2send = 0;
	uint8_t i = 0;

	initMapReg();
	clockInit();
	sciInit();
	sciSetPorts(TX, RX);
	adcInit();

    /* enable interrupts */
    _BIS_SR(GIE);

    protocolInit();
    if(Connected == 1){
        timerAInit((uint32_t)5000);   //Rate in micro seconds
        //P1OUT |= BIT0;
    }


    while(1){

        if(FlagTimer){

            data2send = readSingChanAdc();
            //data2send = 1025;
            P1OUT ^= BIT6;
            sciSendData((uint8_t *)&data2send, 2);
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


void initMapReg(void)
{
    P1OUT = 0;
    P1SEL = 0;
    P1SEL2 = 0;

    P1DIR |= BIT6;
    P1OUT &= ~BIT6;

    //P1DIR |= BIT0;
    //P1OUT &= ~BIT0;
}

void protocolInit(void)
{
    const uint8_t INIT_DATA = '@';
    uint8_t data_receiver;
    //uint8_t tmp = 0;

    sendByte(INIT_DATA);
    sendString("\r\n");

    data_receiver = sciRead();

    while(data_receiver != 'O'){
        sendByte(INIT_DATA);
        sendString("\r\n");
        data_receiver = sciRead();
    }
    Connected = 1;

}
