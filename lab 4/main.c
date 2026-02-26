// Timer_A continuous mode, with interrupts, flashes LEDs
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7


void config_ACLK_to_32KHz_crystal() {
    // By default, ACLK runs on LFMODCLK at 5MHz/128 = 39 KHz
    // Reroute pins to LFXIN/LFXOUT functionality

    PJSEL1 &= ~BIT4;
    PJSEL0 |= BIT4;

    // Wait until the oscillator fault flags remain cleared
    CSCTL0 = CSKEY; // Unlock CS registers

    do {
        CSCTL5 &= ~LFXTOFFG; // Local fault flag
        SFRIFG1 &= ~OFIFG; // Global fault flag
    } while((CSCTL5 & LFXTOFFG) != 0);


    CSCTL0_H = 0; // Lock CS registers
    return;
}



void main(void) {

    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Enable the GPIO pins


    P1DIR |= redLED; // Configure pin as output
    P9DIR |= greenLED; // Configure pin as output
    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off


    // Configure ACLK to the 32 KHz crystal
    config_ACLK_to_32KHz_crystal();


    // Configure Channel 0 for up mode with interrupts
    TA0CCR0 = 32767; //@ 32KHz, 1 second = 2^16
    TA0CCTL0 |= CCIE;
    TA0CCTL0 &= ~CCIFG;

    // Configure Timer_A
    // Use ACLK, divide by 1, continuous mode, TAR cleared
    TA0CTL = TASSEL_1 | ID_0 | MC_1 | TACLR ;


    // Enable the global interrupt bit (call an intrinsic function)
    _enable_interrupts();


    // Infinite loop... the code waits here between interrupts
    for(;;) {}
}

//******* Writing the ISR *******
#pragma vector = TIMER0_A0_VECTOR // Link the ISR to the vector
__interrupt void T0A0_ISR() {
    // Interrupt response goes here
    P1OUT ^= redLED; // toggle LED
    TA0CTL &= ~TAIFG; // clear flag
}
