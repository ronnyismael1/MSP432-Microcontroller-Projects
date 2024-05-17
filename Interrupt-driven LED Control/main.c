/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>

int main(void)
{
    /* Stop Watchdog  */
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


    /* Configure P1.1 with interrupt */
    /* Interrupt set-up for the GPIO module */
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, GPIO_PIN1);
    MAP_GPIO_enableInterrupt(GPIO_PORT_P1, GPIO_PIN1);

    /* Set-up for the interrupt controller */
    MAP_Interrupt_enableInterrupt(INT_PORT1);
    MAP_Interrupt_enableMaster();

    /* Edge detecting high-to-low */
    MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION);

    while(1){}
}

/* GPIO ISR */
void PORT1_IRQHandler(void)
{
    uint32_t status;

    status = MAP_GPIO_getEnabledInterruptStatus(GPIO_PORT_P1);
    MAP_GPIO_clearInterruptFlag(GPIO_PORT_P1, status);

    /* Toggling the output on the LED */
    if(status & GPIO_PIN1)
    {
        // Toggle LED state based on the transition type
        static bool ledState = false; // false means LED is off, true means on
        ledState = !ledState; // Toggle the state

        if(ledState) {
            MAP_GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN1); // Turn on the LED
            MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_LOW_TO_HIGH_TRANSITION); // Set to detect release
        } else {
            MAP_GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1); // Turn off the LED
            MAP_GPIO_interruptEdgeSelect(GPIO_PORT_P1, GPIO_PIN1, GPIO_HIGH_TO_LOW_TRANSITION); // Set to detect press
        }
    }
}
