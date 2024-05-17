#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ti/sysbios/knl/Semaphore.h>

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Clock.h>

#include <ti/drivers/Board.h>
#define __MSP432P4XX__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define TASKSTACKSIZE   2048
#define BAUD_RATE       1200
#define BIT_TIME        (48000000 / BAUD_RATE)  // System Clock / Baud Rate

Task_Struct task1Struct, task2Struct;
Char task1Stack[TASKSTACKSIZE], task2Stack[TASKSTACKSIZE];


Semaphore_Struct semaStruct;
Semaphore_Handle semaphoreHandle = NULL;

// Function Prototypes
Void task1(UArg arg0, UArg arg1);
Void task2(UArg arg0, UArg arg1);
void sendChar(char c);
void sendString(const char *str);

int main()
{
    Board_init();

    // Construct tasks
    Task_Params taskParams;
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;

    // Task 1
    taskParams.stack = &task1Stack;
    Task_construct(&task1Struct, (Task_FuncPtr)task1, &taskParams, NULL);

    // Task 2
    taskParams.stack = &task2Stack;
    Task_construct(&task2Struct, (Task_FuncPtr)task2, &taskParams, NULL);

    // Construct a semaphore object
    Semaphore_Params semaParams;
    Semaphore_Params_init(&semaParams);                 // Initialize structure with default parameters
    Semaphore_construct(&semaStruct, 0, &semaParams);   // Create an instance of semaphore object
    semaphoreHandle = Semaphore_handle(&semaStruct);

    BIOS_start();
    return(0);
}

Void task1(UArg arg0, UArg arg1)
{
    // Set up GPIO pin for UART Tx and button input
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3); // Using P1.3 for Tx
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN1); // Ensure P1.1 is an input with pull-up

    // Initialize variables
    bool lastState = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0; // Assume button on P1.1
    bool currentState = lastState;
    uint32_t lastTimestamp = Timestamp_get32();
    sendString(lastState ? "S10 " : "S11 ");

    Semaphore_post(semaphoreHandle); // Release semaphore

    while(1)
    {
        Semaphore_pend(semaphoreHandle, BIOS_WAIT_FOREVER); // Wait for semaphore

        Task_sleep(100); // Sleep for 100 ms

        // Debounce check
        bool stableState = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0;
        int i = 0;
        for (i = 0; i < 5; i++) { // Check 5 times every 20ms
            Task_sleep(20);
            if (stableState != (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == 0)) {
                stableState = !stableState; // If state changes during checks, flip and break
                break;
            }
        }

        currentState = stableState;

        // Send state only if changed
        if(currentState != lastState || (Timestamp_get32() - lastTimestamp) >= BIT_TIME)
        {
            sendString(currentState ? "S10 " : "S11 ");
            lastTimestamp = Timestamp_get32();
            lastState = currentState;
        }

        Semaphore_post(semaphoreHandle); // Release semaphore
    }
}

Void task2(UArg arg0, UArg arg1)
{
    // Set up GPIO pin for UART Tx and button input
    GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN3); // Using P1.3 for Tx
    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
    GPIO_setAsInputPinWithPullUpResistor(GPIO_PORT_P1, GPIO_PIN4); // Ensure P1.1 is an input with pull-up

    // Initialize variables
    bool lastState = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0; // Assume button on P1.1
    bool currentState = lastState;
    uint32_t lastTimestamp = Timestamp_get32();
    sendString(lastState ? "S20 " : "S21 ");

    Semaphore_post(semaphoreHandle); // Release semaphore

    while(1)
    {
        Semaphore_pend(semaphoreHandle, BIOS_WAIT_FOREVER); // Wait for semaphore

        Task_sleep(100); // Sleep for 100 ms

        // Debounce check
        bool stableState = GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0;
        int i = 0;
        for (i = 0; i < 5; i++) { // Check 5 times every 20ms
            Task_sleep(20);
            if (stableState != (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN4) == 0)) {
                stableState = !stableState; // If state changes during checks, flip and break
                break;
            }
        }

        currentState = stableState;

        // Send state only if changed
        if(currentState != lastState || (Timestamp_get32() - lastTimestamp) >= BIT_TIME)
        {
            sendString(currentState ? "S20 " : "S21 ");
            lastTimestamp = Timestamp_get32();
            lastState = currentState;
        }

        Semaphore_post(semaphoreHandle); // Release semaphore
    }
}


void sendChar(char c)
{
    int i;
    uint32_t startTime = Timestamp_get32();
    GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3); // Start bit
    while (Timestamp_get32() - startTime < BIT_TIME); // Wait one bit time

    for (i = 0; i < 8; i++) // Send 8 data bits
    {
        if (c & (1 << i))
            GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3);
        else
            GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN3);

        startTime = Timestamp_get32();
        while (Timestamp_get32() - startTime < BIT_TIME); // Wait one bit time
    }

    GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN3); // Stop bit
    startTime = Timestamp_get32();
    while (Timestamp_get32() - startTime < BIT_TIME); // Wait one bit time
}

void sendString(const char *str)
{
    while (*str != '\0')
    {
        sendChar(*str++);
    }
}
