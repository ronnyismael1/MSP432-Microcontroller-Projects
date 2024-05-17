/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    volatile uint32_t ii;

    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);
    printf("%u\n", MAP_CS_getMCLK());

    /* Halting the Watchdog */
    MAP_WDT_A_holdTimer();

    /* Configuring output pins */
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0); // RED
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1); // GREEN
    MAP_GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN2); // BLUE

    /* Configure input pins */
    MAP_GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // S1

    /* Bringing all three LEDs to LOW state */
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0); // RED
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1); // GREEN
    MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2); // BLUE

    uint32_t pressTime, releaseTime, duration;

    while (1)
    {
        /* Delay Loop */
        for(ii=0;ii<5000;ii++){}

        while (MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)){} // Timer won't start until button is pressed
        pressTime = MAP_Timer32_getValue(TIMER32_0_BASE); // Time at press
        MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN2); // Blue LED turns on

        while (!MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)){} // Loop until button is released
        releaseTime = MAP_Timer32_getValue(TIMER32_0_BASE); // Time at release
        MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN2); // Blue LED turns off

        /* Calculate how long button was pressed */
        duration = (pressTime - releaseTime) / (MAP_CS_getMCLK() / 1000);
        printf("S1 was pressed for %u ms.\n", duration);
    }
}
