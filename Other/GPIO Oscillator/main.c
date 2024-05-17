#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define N 100

int main(void) {

    uint32_t startTime, endTime;
    int i;
    float samplingRate;
    unsigned char data[N];

    /* Stop WDT */
    WDT_A_holdTimer();

    /* Initialize Timer32 module for timing */
    Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    Timer32_startTimer(TIMER32_0_BASE, 0);

    /* Oscillator setup */
    CAPTIO0CTL = 0; // Clear control register
    CAPTIO0CTL |= 0x0100; // Enable CAPTIO
    CAPTIO0CTL |= 4 << 4; // Select Port 4. Place port number in Bits 7-4
    CAPTIO0CTL |= 1 << 1; // Select Pin 1. Place pin number in Bits 3-1

    /*for(;;)
    {
        bool state = CAPTIO0CTL & 0x200; // Read the state of the oscillator’s binary state output
        printf("%d ", state);
        fflush(stdout);
    }*/

    startTime = Timer32_getValue(TIMER32_0_BASE); // Start timing

    for (i=0; i<N; i++)
    {
        data[i] = (CAPTIO0CTL & 0x200) ? 1 : 0;;
    }

    endTime = Timer32_getValue(TIMER32_0_BASE); // End timing

    /* Calculate and print the sampling rate */
    uint32_t elapsedTime = startTime - endTime;
    float elapsedTimeSeconds = elapsedTime / (float)MAP_CS_getMCLK();
    samplingRate = N / elapsedTimeSeconds;
    printf("Sampling Rate: %.2f Hz\n", samplingRate);

    for (i=0; i<N; i++)
    {
        printf("%d ", data[i]);
    }
    fflush(stdout);
}
