/*
 * board_init.c - Implementation of functions to init board.
 * Copyright (C) 2013 Milan Babel <babel@inf.fu-berlin.de>
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include "cpu.h"
#include "irq.h"
#include "board.h"
#include "msp430.h"
#include "debug.h"
#include "uart_stdio.h"

static void msp_launchpad_ports_init(void)
{
    // Port 1: LED
    P1SEL0 = 0x00;    // Port1 Select: 00000000 = 0x00
    P1OUT = 0x00;    // Port1 Output: 00000000 = 0x00
    P1DIR = 0x01;    // Port1 Direction: 00000001 = 0x01

    // Port 2: Nothing
    P2SEL0 = 0x00;    // Port2 Select: 00000100 = 0x04
    P2OUT = 0x00;    // Port2 Output: 00000000 = 0x00
    P2DIR = 0x00;    // Port2 Direction: 11111111 = 0xFF


    // Port 3: Nothing
    P3SEL0 = 0x00;    // Port3 Select: 11111110 = 0xFE
    P3OUT = 0x00;    // Port3 Output: 00000000 = 0x00
    P3DIR = 0x00;    // Port3 Direction: 11111111 = 0xFF


    // Port 4: LED
    P4SEL0 = 0x00;    // Port4 Select: 00000000 = 0x00
    P4OUT = 0x00;    // Port4 Output: 00000000 = 0x00
    P4DIR = 0x40;    // Port4 Direction: 01000000 = 0x40

    // Port J:

    // enable propagation of Port changes to hardware
    PM5CTL0 &= ~LOCKLPM5;
}

/*---------------------------------------------------------------------------*/
void msp430_init_dco(void)
{
  /*---------------------- use internal DCO @ 16MHz -------------------------*/

  // Stop watchdog
  WDTCTL = WDTPW + WDTHOLD;

  // Enable FRAM waits (FRAM does not support clocks > 8MHz)
  FRCTL0 = FWPW;
  FRCTL0_L = 0x10;
  FRCTL0_H = 0xff;

  // Enable DCO @ 16MHz
  CSCTL0_H = CSKEY >> 8;
  CSCTL1 = DCORSEL | DCOFSEL_4;
  CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;
  CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
  CSCTL0_H = 0;
}

void board_init(void)
{
    msp430_cpu_init();
    msp_launchpad_ports_init();
    msp430_init_dco();

    /* initialize STDIO over UART */
    uart_stdio_init();
}
