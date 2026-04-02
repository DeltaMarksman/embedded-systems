#include <msp430.h> 
#include <easy_lscs.h>

#define FLAGS UCA1IFG // Contains the transmit & receive flags
#define RXFLAG UCRXIFG // Receive flag
#define TXFLAG UCTXIFG // Transmit flag
#define TXBUFFER UCA1TXBUF // Transmit buffer
#define RXBUFFER UCA1RXBUF // Receive buffer
#define true 1
#define false 0

// lab 7 specific defines
#define RN_7    0x7000
#define CT      BITB
#define M_3     0x0600
#define ME      BIT2




void uart_write_char(unsigned char ch){
    // Wait for any ongoing transmission to complete
    while ( (FLAGS & TXFLAG)==0 ) {}

    // Copy the byte to the transmit buffer
    TXBUFFER = ch; // Tx flag goes to 0 and Tx begins!

    return;
}

void uart_newline() {
    uart_write_char('\n');
    uart_write_char('\r');
}

void uart_write_uint16(unsigned int n) {
    unsigned int divisor = 10000;
    unsigned char started = 0;

    if (n == 0) {
        uart_write_char('0');
        return;
    }

    while (divisor > 0) {
        unsigned int digit = n / divisor;
        if (digit > 0 || started) {
            uart_write_char('0' + digit);
            started = 1;
        }
        n %= divisor;
        divisor /= 10;
    }
}

void uart_write_string(char *str) {
    while (*str != '\0') {
        uart_write_char(*str);
        str++;
    }

    //uart_newline();
}

void uart_write_charln(unsigned char ch) {
    uart_write_char(ch);
    uart_newline();
}


// The function returns the byte; if none received, returns null character
unsigned char uart_read_char(void){
    unsigned char temp;
    // Return null character (ASCII=0) if no byte was received
    if( (FLAGS & RXFLAG) == 0)
        return 0;

    // Otherwise, copy the received byte (this clears the flag) and return it
    temp = RXBUFFER;
    return temp;
}


char* uart_read_string()
{
    unsigned int i = 0;
    static char buffer[32];

    while (i < 32) {
        unsigned char c = uart_read_char();

        // no byte received yet
        if (c == 0) {
            continue;
        }

        // end of line
        if (c == '\n' || c == '\r') {
            break;
        }

        buffer[i++] = c;
    }

    // null-terminate
    buffer[i] = '\0';           // null-terminate

    return buffer;
}

// Configure UART to the popular configuration
// 9600 baud, 8-bit data, LSB first, no parity bits, 1 stop bit
// no flow control, oversampling reception
// Clock: SMCLK @ 1 MHz (1,000,000 Hz)
void Initialize_UART(void){
    // Configure pins to UART functionality
    P3SEL1 &= ~(BIT4|BIT5);
    P3SEL0 |= (BIT4|BIT5);

    // Main configuration register
    UCA1CTLW0 = UCSWRST; // Engage reset; change all the fields to zero

    // Most fields in this register, when set to zero, correspond to the
    // popular configuration
    UCA1CTLW0 |= UCSSEL_2; // Set clock to SMCLK

    // Configure the clock dividers and modulators (and enable oversampling)
    UCA1BRW = 6; // divider

    // Modulators: UCBRF = 8 = 1000 --> UCBRF3 (bit #3)
    // UCBRS = 0x20 = 0010 0000 = UCBRS5 (bit #5)
    UCA1MCTLW = UCBRF3 | UCBRS5 | UCOS16;

    // Exit the reset state
    UCA1CTLW0 &= ~UCSWRST;
}

void Initialize_I2C(void) {
    // Configure the MCU in Master mode
    // Configure pins to I2C functionality
    // (UCB1SDA same as P4.0) (UCB1SCL same as P4.1)
    // (P4SEL1=11, P4SEL0=00) (P4DIR=xx)
    P4SEL1 |= (BIT1|BIT0);
    P4SEL0 &= ~(BIT1|BIT0);

    // Enter reset state and set all fields in this register to zero
    UCB1CTLW0 = UCSWRST;

    // Fields that should be nonzero are changed below
    // (Master Mode: UCMST) (I2C mode: UCMODE_3) (Synchronous mode: UCSYNC)
    // (UCSSEL 1:ACLK, 2,3:SMCLK)
    UCB1CTLW0 |= UCMST | UCMODE_3 | UCSYNC | UCSSEL_3;

    // Clock frequency: SMCLK/8 = 1 MHz/8 = 125 KHz
    UCB1BRW = 8;

    // Chip Data Sheet p. 53 (Should be 400 KHz max)
    // Exit the reset mode at the end of the configuration
    UCB1CTLW0 &= ~UCSWRST;
}

////////////////////////////////////////////////////////////////////
///////// Function Headers ///////////////////////////////////
////////////////////////////////////////////////////////////////////
int i2c_read_word(unsigned char, unsigned char, unsigned int*); //
int i2c_write_word(unsigned char, unsigned char, unsigned int); //
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
// Read a word (2 bytes) from I2C (address, register)
int i2c_read_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int * data) {
    unsigned char byte1=0, byte2=0; // Intialize to ensure successful reading
    UCB1I2CSA = i2c_address; // Set address
    UCB1IFG &= ~UCTXIFG0;

    // Transmit a byte (the internal register address)
    UCB1CTLW0 |= UCTR;
    UCB1CTLW0 |= UCTXSTT;

    while((UCB1IFG & UCTXIFG0)==0) {} // Wait for flag to raise

    UCB1TXBUF = i2c_reg; // Write in the TX buffer

    while((UCB1IFG & UCTXIFG0)==0) {} // Buffer copied to shift register; Tx in progress; set Stop bit

    // Repeated Start
    UCB1CTLW0 &= ~UCTR;
    UCB1CTLW0 |= UCTXSTT;

    // Read the first byte
    while((UCB1IFG & UCRXIFG0)==0) {} // Wait for flag to raise

    byte1 = UCB1RXBUF;

    // Assert the Stop signal bit before receiving the last byte
    UCB1CTLW0 |= UCTXSTP;

    // Read the second byte
    while((UCB1IFG & UCRXIFG0)==0) {} // Wait for flag to raise
    byte2 = UCB1RXBUF;
    while((UCB1CTLW0 & UCTXSTP)!=0) {}
    while((UCB1STATW & UCBBUSY)!=0) {}

    *data = (byte1 << 8) | (byte2 & (unsigned int)0x00FF);

    return 0;
}
////////////////////////////////////////////////////////////////////
// Write a word (2 bytes) to I2C (address, register)
int i2c_write_word(unsigned char i2c_address, unsigned char i2c_reg, unsigned int data) {
unsigned char byte1, byte2;
UCB1I2CSA = i2c_address; // Set I2C address
byte1 = (data >> 8) & 0xFF; // MSByte
byte2 = data & 0xFF; // LSByte
UCB1IFG &= ~UCTXIFG0;
// Write 3 bytes
UCB1CTLW0 |= (UCTR | UCTXSTT);
while( (UCB1IFG & UCTXIFG0) == 0) {}
UCB1TXBUF = i2c_reg;
while( (UCB1IFG & UCTXIFG0) == 0) {}
UCB1TXBUF = byte1;
while( (UCB1IFG & UCTXIFG0) == 0) {}
UCB1TXBUF = byte2;
while( (UCB1IFG & UCTXIFG0) == 0) {}
UCB1CTLW0 |= UCTXSTP;
while( (UCB1CTLW0 & UCTXSTP) != 0 ) {}
while((UCB1STATW & UCBBUSY)!=0) {}
return 0;
}





// Variable to keep track of timer
int seconds     = 0;
int minutes     = 0;
int hours       = 12;
int last_lux    = 0;
char *feedback  = "";
int should_print = 0;
void onTimer() {
    // Reading two bytes from register 0x00 on I2C device 0x22
    unsigned int data;



    // Read from register 0x00 to get the result
    // https://www.ti.com/lit/ds/symlink/opt3001.pdf?ts=1775042449037
    i2c_read_word(0x44, 0x00, &data);
    unsigned int lux = data*1.28;

    // Time logic
    seconds += 1;
    minutes += seconds > 59 ? 1 : 0;
    if (seconds >= 60) { should_print = 1; }
    hours   += minutes > 59 ? 1 : 0;

    seconds = seconds > 59 ? seconds - 60 : seconds;
    minutes = minutes > 59 ? minutes - 60 : minutes;
    hours   = hours   > 12 ? hours - 12 : hours;

    if (!should_print) {
        return;
    }
    should_print = 0;

    // Lux logic
    if (abs(lux - last_lux) >= 10)
        feedback = lux > last_lux ? "<high>" : "<low>";
    else
        feedback = "";

    last_lux = lux;


    // Printing TIME
    uart_write_char('0' + hours/10);
    uart_write_char('0' + hours%10);
    uart_write_char(':');
    uart_write_char('0' + minutes/10);
    uart_write_char('0' + minutes%10);

    // Printing lux
    uart_write_char('\t');
    uart_write_uint16(lux);
    uart_write_string("\t lux");

    // Printing feedback
    uart_write_char('\t');
    uart_write_string(feedback);
    uart_newline();
}

void changeTime() {
    uart_write_string("Enter the time...(3 or 4 digits then hit Enter)");
    uart_newline();

    // Set time
    char *input = uart_read_string();
    int digits = input[3] == '\0' ? 3 : 4;

    if (digits == 3) {
        hours   = (input[0]-'0');
        minutes = (input[1]-'0')*10  +  (input[2]-'0');
    } else {
        hours   = (input[0]-'0')*10  +  (input[1]-'0');
        minutes = (input[2]-'0')*10  +  (input[3]-'0');
    }


    uart_write_string("Time is set to ");
    // Printing TIME
    uart_write_char('0' + hours/10);
    uart_write_char('0' + hours%10);
    uart_write_char(':');
    uart_write_char('0' + minutes/10);
    uart_write_char('0' + minutes%10);

    uart_newline();
    uart_newline();
    should_print = 1;
    onTimer();
}

/**
 * main.c
 */
int main(void)
{
WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

    config_ACLK_to_32KHz_crystal();
    Initialize_I2C();
    Initialize_UART();
    config_upmode(1000);
    init_switches();

    // Init LIGHT SENSOR
    i2c_write_word(0x44, 0x01, RN_7 | M_3 | ME); // Omit CT because CT is 0

    // Terminal
    uart_write_string("*** Lux Logger ***");
    uart_newline();
    should_print = 1;
    onTimer();

    // Called per second
    timer_callback(onTimer);
    _enable_interrupts();

    // on button press
    s2_callback(changeTime);

    return 0;
}
