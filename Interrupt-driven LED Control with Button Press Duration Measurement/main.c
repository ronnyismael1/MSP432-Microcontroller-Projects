/* DriverLib Includes */
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

/* Standard Includes */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#define QUEUE_SIZE 10
struct record {
    uint32_t timestamp;
    uint8_t state;
};
struct record queue[QUEUE_SIZE];
volatile int read_index = 0;
volatile int write_index = 0;

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

    /* Timer32 initialization */
    MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT, TIMER32_FREE_RUN_MODE);
    MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

    printf("\n");

    while(1) {
        __delay_cycles(200 * 3000); // Delay 200 ms at 3 MHz
        if (read_index != write_index) {
            // Process the record
            struct record current_record = queue[read_index];
            static uint32_t last_timestamp = 0;
            if (current_record.state == 0) { // Button was released
                uint32_t duration = (last_timestamp - current_record.timestamp) / (MAP_CS_getMCLK() / 1000);
                printf("Duration: %u ms, Read Index: %d\n", duration, read_index);
            }
            last_timestamp = current_record.timestamp;
            read_index = (read_index + 1) % QUEUE_SIZE;
        }
    }
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
        // ISR print an error message to the debug console
        if (((write_index + 1) % QUEUE_SIZE) == read_index) { // Check if queue is full
            // Queue is full, print an error message
            printf("Error: Queue is full.\n");
            return;
        }

        // Record the timestamp and state
        uint32_t timestamp = MAP_Timer32_getValue(TIMER32_0_BASE);
        uint8_t state = !(MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1)); // Record state as 1 for pressed, 0 for released

        // Store the record in the queue
        queue[write_index].timestamp = timestamp;
        queue[write_index].state = state;
        //printf("timestamp is %u ms\tstate is %u\n", MAP_Timer32_getValue(TIMER32_0_BASE), MAP_GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1));

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
        write_index = (write_index + 1) % QUEUE_SIZE;
    }
}
