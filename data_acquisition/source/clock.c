/*
 * clock.c
 *
 *  Created on: Apr 20, 2021
 *      Author: MarcoAAG
 */

#include<msp430g2553.h>
#include"clock.h"

void clockInit()
{
    //set 1Mhz
    DCOCTL = CALDCO_1MHZ;
    BCSCTL1 = CALBC1_1MHZ;
}


