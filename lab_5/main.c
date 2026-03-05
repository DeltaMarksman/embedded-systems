// Sample code that prints 430 on the LCD monitor

#include <msp430fr6989.h>

#define redLED     BIT0   // Red at P1.0
#define greenLED   BIT7   // Green at P9.7
#define S1_BUTTON     BIT1
#define S2_BUTTON     BIT2

#define LCD_DIGIT_1  LCDM10   // Leftmost digit
#define LCD_DIGIT_2  LCDM6
#define LCD_DIGIT_3  LCDM4
#define LCD_DIGIT_4  LCDM19
#define LCD_DIGIT_5  LCDM15
#define LCD_DIGIT_6  LCDM8    // Rightmost digit

#define COLON_MEM      LCDM7
#define COLON_BIT      BIT2

#define TIMER_SYMBOL_MEM     LCDM3
#define TIMER_SYMBOL_BIT     BIT3

#define EXCL_MEM       LCDM3
#define EXCL_BIT       BIT0

int hours       = 0;
int minutes     = 0;
int seconds     = 0;
int running     = 1;
int s1_pressed  = 0;
int s2_pressed  = 0;


void Initialize_LCD();

// Digit segment shapes for MSP430FR6989 LCD
const unsigned char LCD_Shapes[10] = {
    0xFC,   // 0
    0x60,   // 1
    0xDB,   // 2
    0xF3,   // 3
    0x67,   // 4
    0xB7,   // 5
    0xBF,   // 6
    0xE4,   // 7
    0xFF,   // 8
    0xF7    // 9
};


void lcd_update_icons() {
    if (running) {
        TIMER_SYMBOL_MEM |= TIMER_SYMBOL_BIT;     // chronometer logo pn
        EXCL_MEM   &= ~EXCL_BIT;      // exclamation off
        COLON_MEM ^= COLON_BIT; // blink colon

    } else {
        TIMER_SYMBOL_MEM &= ~TIMER_SYMBOL_BIT;    // chronometer logo off
        EXCL_MEM   |= EXCL_BIT;       // exclamation on
        COLON_MEM  |= COLON_BIT;     // colon on when stopped
    }
}

// handle overflow and underflow
void time_normalize(void) {
    if (seconds >= 60) {
        seconds = 0;
        minutes++;
    } else if (seconds < 0) {
        seconds = 59;
        minutes--;
    }

    if (minutes >= 60) {
        minutes = 0;
        hours++;
    } else if (minutes < 0) {
        minutes = 59;
        hours--;
    }

    if (hours >= 12) {
        hours = 0;
    } else if (hours < 0) {
        hours = 11;
    }
}

void lcd_write_time(int h, int m, int s) {

    unsigned int d1 = h / 10;
    unsigned int d2 = h % 10;
    unsigned int d3 = m / 10;
    unsigned int d4 = m % 10;
    unsigned int d5 = s / 10;
    unsigned int d6 = s % 10;

    LCD_DIGIT_1 = LCD_Shapes[d1];
    LCD_DIGIT_2 = LCD_Shapes[d2];
    LCD_DIGIT_3 = LCD_Shapes[d3];
    LCD_DIGIT_4 = LCD_Shapes[d4];
    LCD_DIGIT_5 = LCD_Shapes[d5];
    LCD_DIGIT_6 = LCD_Shapes[d6];
}

void lcd_update() {
    lcd_write_time(hours, minutes, seconds);
    lcd_update_icons();
}

void time_increment_one_second(void)
{
    seconds++;
    time_normalize();
}

void time_fast_forward_step(void) {
    time_increment_one_second();
}

void time_rewind_step(void) {
    seconds--;
    time_normalize();
}

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


    int main(void) {

    volatile unsigned int n;

    WDTCTL = WDTPW | WDTHOLD;     // Stop WDT
    PM5CTL0 &= ~LOCKLPM5;         // Enable GPIO pins

    P1DIR |= redLED;              // Red LED output
    P9DIR |= greenLED;            // Green LED output

    P1OUT |= redLED;              // Red ON
    P9OUT &= ~greenLED;           // Green OFF

    P1DIR &= ~(S1_BUTTON | S2_BUTTON);
    P1REN |=  (S1_BUTTON | S2_BUTTON);
    P1OUT |=  (S1_BUTTON | S2_BUTTON);
    P1IES |=  (S1_BUTTON | S2_BUTTON);
    P1IFG &= ~(S1_BUTTON | S2_BUTTON);
    P1IE  |=  (S1_BUTTON | S2_BUTTON);

    // Configure ACLK to the 32 KHz crystal
        config_ACLK_to_32KHz_crystal();



    TA0CCR0 = 32768 - 1;          // 1 second @ 32kHz
    TA0CCTL0 |= CCIE;             // enable interrupt
    TA0CTL = TASSEL_1 | MC_1;     // ACLK, up mode
    __enable_interrupt();

    // Initialize LCD
    Initialize_LCD();
    lcd_update();
    LCDM20 = BIT0; // decimal point


    for (;;) {

        // pause and reset
        if (s1_pressed) {
            s1_pressed = 0;

            // count time s1 is pressed
            unsigned int count = 0;
            while (!(P1IN & S1_BUTTON)) {   // still pressed (active low)
                __delay_cycles(20000);      // debounce
                count++;
                if (count > 150) {          // long press threshold
                    // long press for reset and stop
                    running = 0;
                    hours = minutes = seconds = 0;
                    lcd_update();
                    break;
                }
            }

            if (count <= 150) {
                // short press pause
                running = !running;
                lcd_update();
            }
        }

        // fast forward and rewind
        if (s2_pressed) {
            while (!(P1IN & S2_BUTTON)) {
                if (!(P1IN & S1_BUTTON)) {
                    time_rewind_step();
                } else {
                    time_fast_forward_step();
                }
                lcd_update();

                __delay_cycles(2000);
            }

            s2_pressed = 0;
        }
    }

    return 0;
}

//increment timer
#pragma vector = TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_ISR(void)
{
    if (running) {
        time_increment_one_second();
        lcd_update();
    }
}

#pragma vector = PORT1_VECTOR
__interrupt void PORT1_ISR(void)
{
    if (P1IFG & S1_BUTTON) {       // S1 pressed (falling edge)
        if (s2_pressed == 0)
            s1_pressed = 1;
        __delay_cycles(20000);      // debounce
        P1IFG &= ~S1_BUTTON;
    }

    if (P1IFG & S2_BUTTON) {       // S2 pressed (falling edge)
        s2_pressed = 1;
        __delay_cycles(20000);      // debounce
        P1IFG &= ~S2_BUTTON;
    }
}

//**********************************************************
// Initializes the LCD_C module
// Source: MSP430FR6989 Sample Code
//**********************************************************

void Initialize_LCD() {

    PJSEL0 = BIT4 | BIT5;     // For LFXT

    LCDCPCTL0 = 0xFFD0;
    LCDCPCTL1 = 0xF83F;
    LCDCPCTL2 = 0x00F8;

    // Configure LFXT 32kHz crystal
    CSCTL0_H = CSKEY >> 8;    // Unlock CS registers
    CSCTL4 &= ~LFXTOFF;       // Enable LFXT

    do {
        CSCTL5 &= ~LFXTOFFG;  // Clear LFXT fault flag
        SFRIFG1 &= ~OFIFG;
    } while (SFRIFG1 & OFIFG);

    CSCTL0_H = 0;             // Lock CS registers

    // Initialize LCD_C
    // ACLK, Divider = 1, Pre-divider = 16; 4-pin MUX
    LCDCCTL0 = LCDDIV__1 | LCDPRE__16 | LCD4MUX | LCDLP;

    // VLCD generated internally
    // V2-V4 generated internally, v5 to ground
    // Set VLCD voltage to 2.60v
    // Enable charge pump and select internal reference
    LCDCVCTL = VLCD_1 | VLCDREF_0 | LCDCPEN;

    LCDCCPCTL = LCDCPCLKSYNC; // Clock synchronization enabled
    LCDCMEMCTL = LCDCLRM;     // Clear LCD memory

    LCDCCTL0 |= LCDON;        // Turn LCD on

    return;
}
