#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

// Timing delay is (1/baud rate) in microseconds
// for 1200 baud, delay = 1/1200 = 833 us, 0.833us x 3000 mhz = 2500
// for 9600 baud, delay = 1/9600 = 104.2 us, 0.1042us x 3000 mhz = 312.5
#define TIMING_DELAY 300    	// 312 was giving slightly strange results so 300 is used

void send_message(const char *message)
{
	__delay_cycles(100 * 3000); 	// delay for 100 ms, terminate existing bus transitions

	while (*message != '\0')    	// Each character in message is a frame to be sent
	{
    	P1->OUT &= ~BIT3;       	// start frame bit logic low
    	__delay_cycles(TIMING_DELAY);
    	int i;
    	for (i = 0; i < 8; i++) 	// data frame loop
    	{
        	if ((*message >> i) & 1)
        	{
            	P1->OUT |= BIT3;	// logic high
        	}
        	else
        	{
            	P1->OUT &= ~BIT3;   // logic low
        	}
        	__delay_cycles(TIMING_DELAY);
    	}
    	// no parity bit because it is disabled
    	P1->OUT |= BIT3;       	// Stop bit logic high
    	__delay_cycles(TIMING_DELAY);
    	message++;              	// iterate through next char
	}
}
int main(void)
{
	/* Halting WDT  */
	MAP_WDT_A_holdTimer();

	// Set up the transmit pins
	P1->SEL0 &= ~(BIT2 | BIT3 );              	// prepare ports
	P1->SEL1 &= ~(BIT2 | BIT3 );              	// set P1.2 and P1.3 as GPIO
	P1->DIR |= BIT3;        	// set P1.3 (Tx) as output
	P1->OUT |= BIT3;        	// set Tx to idle state (logic high)
	// set up receive pins
	P1->SEL0 &= ~BIT2;      	// Set P1.2 as GPIO
	P1->SEL1 &= ~BIT2;
	P1->DIR &= ~BIT2;       	// Set P1.2 (Rx) as input

	static char something[] = { 0x0d, 0x0a, 0x56, 0x52, 0x4f, 0x4f, 0x4d, 0x4d,
                            	0x4d, 0x4d, 0x4d, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x0d, 0x0a,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x7c, 0x5f,
                            	0x7c, 0x7c, 0x5f, 0x5c, 0x60, 0x2e, 0x5f, 0x5f,
                            	0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20,
                            	0x20, 0x20, 0x5f, 0x20, 0x20, 0x20, 0x20, 0x5f,
                            	0x20, 0x5f, 0x5c, 0x0d, 0x0a, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x3d, 0x60, 0x2d, 0x28, 0x5f, 0x29, 0x2d,
                            	0x2d, 0x28, 0x5f, 0x29, 0x2d, 0x27, 0x0d, 0x0a,
                            	0x00 };
	//send_message("Hello, World!\n");
	send_message(something);
	while (1)
	{
    	if (!(P1->IN & BIT2 ))
    	{
        	send_message(something);	// send when an input is received
    	}
	}
}
