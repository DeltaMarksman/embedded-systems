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

void red_led(int value) {
    if (value == 1) 
        P1OUT |= redLED;
    else 
        P1OUT &= ~redLED;
}

void green_led(int value) {
    if (value == 1) 
        P9OUT |= greenLED;
    else 
        P9OUT &= ~greenLED;
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

    int error = 0;

    // Infinite loop
    for(;;) {
        // Check if in error
        if (error == 1) {
            if (!button_is_pressed(BUT2))
                continue;

            TA0CTL &= ~TAIFG;
            TA0CTL |= TACLR;
            green_led(0);
            error = 0;
        }
            

        // Detect when the button is pressed.
        if (button_is_pressed(BUT1)) {
            // Turn on the timer and clear the timer
            TA0CTL = (TA0CTL & ~MC_3) | TACLR | MC_2;

            // Begin while to determine when the button is released
            while (button_is_pressed(BUT1)) {
                // if the timer overflows, enter error state
                if ((TA0CTL & TAIFG) == TAIFG) {
                    error = 1;
                    break;
                }
            }

            // Check if errored
            if (error == 1) {
                green_led(1);
                continue;
            }

            // After button is released, put the timer in up, store the current value in the upto value and reset
            TA0CTL = (TA0CTL & ~MC_3) | MC_1;
            TA0CCR0 = TA0R;
            TA0CTL |= TACLR;
            TA0CTL &= ~TAIFG;

            // Turn on the LED until the flag is turned on
            red_led(1);
            while( (TA0CTL & TAIFG) == 0 ) {}
            red_led(0);

            // turn off the timer and clear the flag
            TA0CTL = (TA0CTL & ~MC_3) | MC_0;
            TA0CTL &= ~TAIFG;
            
        }
    }
}
