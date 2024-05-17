#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

int main(void)
{
	/* Stop Watchdog  */
	MAP_WDT_A_holdTimer();

	// Configure Timer_A in continuous mode
	Timer_A_ContinuousModeConfig timer_continuous_obj;
	timer_continuous_obj.clockSource =
	TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK;
	timer_continuous_obj.clockSourceDivider = TIMER_A_CLOCKSOURCE_DIVIDER_1;
	timer_continuous_obj.timerInterruptEnable_TAIE =
	TIMER_A_TAIE_INTERRUPT_DISABLE;
	timer_continuous_obj.timerClear = TIMER_A_DO_CLEAR;
	MAP_Timer_A_configureContinuousMode(TIMER_A2_BASE, &timer_continuous_obj);
	MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

	// some variables for looping
	__delay_cycles(12 * 3000);
	uint32_t cyclecount = MAP_Timer_A_getCounterValue(TIMER_A2_BASE);
	float sum = 0;
	float frequency;

	uint8_t count, outcount;
	for (outcount = 0; outcount < 3; outcount++)
	{
    	if (outcount == 0)
    	{
        	CAPTIO0CTL = 0; // Clear control register
        	CAPTIO0CTL |= 0x0100; // Enable CAPTIO
        	CAPTIO0CTL |= 4 << 4; // Select Port 4. Place port number in Bits 7-4
        	CAPTIO0CTL |= 1 << 1; // Select Pin 1. Place pin number in Bits 3-1
        	printf("Port 4, Pin 1 measurements\n");
    	}
    	else if (outcount == 1)
    	{
        	CAPTIO0CTL = 0; // Clear control register
        	CAPTIO0CTL |= 0x0100; // Enable CAPTIO
        	CAPTIO0CTL |= 1 << 4; // Select Port 1. Place port number in Bits 7-4
        	CAPTIO0CTL |= 1 << 1; // Select Pin 1. Place pin number in Bits 3-1
        	printf("Port 1, Pin 1 measurements\n");
    	}
    	else
    	{
        	CAPTIO0CTL = 0; // Clear control register
        	CAPTIO0CTL |= 0x0100; // Enable CAPTIO
        	CAPTIO0CTL |= 3 << 4; // Select Port 3. Place port number in Bits 7-4
        	CAPTIO0CTL |= 1 << 1; // Select Pin 1. Place pin number in Bits 3-1
        	printf("Port 3, Pin 1 measurements\n");
    	}

    	sum = 0;
    	printf("Frequencies measured: ");
    	for (count = 0; count < 10; count++)
    	{
        	// clear counter
        	timer_continuous_obj.timerClear = TIMER_A_DO_CLEAR;
        	MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

        	CAPTIO0CTL = 0; // Clear control register

        	// Delay for 12 ms
        	__delay_cycles(12 * 3000);

        	// Read counter value
        	cyclecount = MAP_Timer_A_getCounterValue(TIMER_A2_BASE);

        	// Calculate frequency
        	frequency = (cyclecount / 0.012) / (1000000.0);
        	sum = sum + frequency;

        	printf("%.3f MHz ", frequency);
        	// Print frequency to debug console
        	//printf("Frequency = %d/3ms :  %.3f MHz\n", cyclecount, frequency);

    	}
    	printf("\n");
    	sum = sum / 10; //get average
    	printf("Average Frequency: %.4f MHz\n", sum);

    	CAPTIO0CTL &= ~0x0100; // Disable CAPTI0
	}
	while (1)
	{
    	// dummy loop
	}
	return 0;
}
