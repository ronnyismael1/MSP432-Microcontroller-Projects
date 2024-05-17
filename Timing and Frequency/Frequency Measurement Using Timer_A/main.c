#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define MCLK_FREQUENCY_IN_HZ 3000000
#define DELAY_3MS (MCLK_FREQUENCY_IN_HZ / 1000 * 3)
#define TIMER32_DURATION 10

int main(void) {

    /* Stop WDT */
    WDT_A_holdTimer();

    /* Oscillator setup */
    CAPTIO0CTL = 0;       // Clear control register
    CAPTIO0CTL |= 0x0100; // Enable CAPTIO
    CAPTIO0CTL |= 4 << 4; // Select Port 4. Place port number in Bits 7-4
    CAPTIO0CTL |= 1 << 1; // Select Pin 1. Place pin number in Bits 3-1

    /* Timer_A setup for continuous mode */
    Timer_A_ContinuousModeConfig timer_continuous_obj = {
        TIMER_A_CLOCKSOURCE_INVERTED_EXTERNAL_TXCLK, // Clock source
        TIMER_A_CLOCKSOURCE_DIVIDER_1,               // Clock source divider
        TIMER_A_TAIE_INTERRUPT_DISABLE,              // Timer_A interrupt enable/disable
        TIMER_A_DO_CLEAR                             // Clear TAR
    };

    MAP_Timer_A_configureContinuousMode(TIMER_A2_BASE, &timer_continuous_obj);

    /* Initialize Timer32 module for timing 10 seconds duration */
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_PERIODIC_MODE);
    uint32_t timer32Count = MCLK_FREQUENCY_IN_HZ * TIMER32_DURATION; // 10 seconds count
    Timer32_setCount(TIMER32_0_BASE, timer32Count);
    Timer32_startTimer(TIMER32_0_BASE, true); // true for one-shot mode

    while(Timer32_getValue(TIMER32_0_BASE) != 0) { // Check if the 10 seconds have passed
        MAP_Timer_A_clearTimer(TIMER_A2_BASE); // Clear Timer_A before each measurement
        MAP_Timer_A_startCounter(TIMER_A2_BASE, TIMER_A_CONTINUOUS_MODE);

        /* Delay for measurement window */
        __delay_cycles(DELAY_3MS);

        /* Stop Timer_A to read its value accurately */
        MAP_Timer_A_stopTimer(TIMER_A2_BASE);

        /* Read counter value from Timer_A */
        uint32_t counterValue = MAP_Timer_A_getCounterValue(TIMER_A2_BASE);

        /* Convert counter value to frequency */
        float frequency = (float)counterValue / 0.003; // Counter ticks in 3 ms

        printf("%.2f\n", counterValue, frequency);
        fflush(stdout);
    }

    while(1) {
        // Dummy loop
    }
}
