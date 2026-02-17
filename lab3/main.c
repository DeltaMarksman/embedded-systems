// Flashing the LED with Timer_A, continuous mode, via polling
#include <msp430fr6989.h>
#define redLED BIT0 // Red LED at P1.0
#define greenLED BIT7 // Green LED at P9.7
#define BUT1 BIT1 // Button S1 at P1.1
#define BUT2 BIT2 // Button S2 at P1.2



//**********************************
// Configures ACLK to 32 KHz crystal
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


// Helper functions
int button_is_pressed(int button) {
    return (P1IN & button) == 0;
}

/**
 * main.c
 */
void main(void) {

    WDTCTL = WDTPW | WDTHOLD; // Stop the Watchdog timer
    PM5CTL0 &= ~LOCKLPM5; // Disable GPIO power-on default high-impedance mode


    // Configure the LEDs as output
    P1DIR |= redLED;
    P9DIR |= greenLED;

    // Turn off the LEDs
    P1OUT &= ~redLED; // Turn LED Off
    P9OUT &= ~greenLED; // Turn LED Off


    // Configure buttons
    P1DIR &= ~BUT1; // Direct pin as input
    P1REN |= BUT1;  // Enable built-in resistor
    P1OUT |= BUT1; // Set resistor as pull-up

    P1DIR &= ~BUT2; // Direct pin as input
    P1REN |= BUT2;  // Enable built-in resistor
    P1OUT |= BUT2; // Set resistor as pull-up


    // Configure ACLK to the 32 KHz crystal (function call)
    config_ACLK_to_32KHz_crystal();
    // Configure Timer_A
    // Use ACLK, divide by 1, continuous mode, clear TAR
    TA0CTL = TASSEL_1 | ID_0 | MC_2 | TACLR;

    // Ensure flag is cleared at the start
    TA0CTL &= ~TAIFG;

    // Infinite loop
    for(;;) {
        if (button_is_pressed(BUT2)) {
            P1OUT |= redLED;
        }
    }
}
