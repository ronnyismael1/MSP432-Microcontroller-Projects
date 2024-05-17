#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

/* XDC module Headers */
#include <xdc/std.h>
#include <xdc/runtime/System.h>
#include <xdc/runtime/Timestamp.h>
#include <xdc/runtime/Types.h>

/* BIOS module Headers */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Clock.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/knl/Semaphore.h>

#include <ti/drivers/Board.h>
#define __MSP432P4XX__
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

#define TASKSTACKSIZE   2048

Void clockHandler1(UArg arg);
Void task1(UArg arg0, UArg arg1);
Void task2(UArg arg0, UArg arg1);

Task_Struct task1Struct, task2Struct;
Char task1Stack[TASKSTACKSIZE], task2Stack[TASKSTACKSIZE];

// declare two global semaphores
Semaphore_Struct semaStruct;
Semaphore_Handle semaphoreHandle = NULL;

int main()
{
	// Init drivers
	Board_init();

	srand(time(NULL));  	// Set seed for random number generator

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
	Semaphore_Params_init(&semaParams);             	// Initialize structure with default parameters
	Semaphore_construct(&semaStruct, 0, &semaParams);   // Create an instance of semaphore object
	semaphoreHandle = Semaphore_handle(&semaStruct);

	// Create clock that calls Task_yield() to preempt tasks
	Clock_Params clockParams;
	Clock_Handle myClk0;
	Clock_Params_init(&clockParams);
	clockParams.period = 10;     	// 1 Clock tick is 1 ms by default
	clockParams.startFlag = TRUE;	// Start immediately
	myClk0 = Clock_create((Clock_FuncPtr)clockHandler1, 1, &clockParams, NULL);   // 2nd argument must be greater than 0
	if (myClk0 == NULL)
    	System_abort("Clock0 create failed");

	BIOS_start();	// Jump to the OS and won't return
	return(0);
}

Void task1(UArg arg0, UArg arg1)
{

	// this task sleeps for some time, then signals task2
	// the combined tasks, they blink LED 0.5Hz
	while(1)
	{
    	Task_sleep(1000); // 1000 ms on and 1000 ms off. total period 2s, freq = 0.5hz
    	Semaphore_post(semaphoreHandle); // Send signal to Task 2
	}
}

Void task2(UArg arg0, UArg arg1)
{
	// set up GPIO port for LED
	GPIO_setAsOutputPin(GPIO_PORT_P1, GPIO_PIN0); // LED1 uses P1.0

	while(1)
	{
    	Semaphore_pend(semaphoreHandle, BIOS_WAIT_FOREVER); // when signal from task 1,
    	GPIO_toggleOutputOnPin(GPIO_PORT_P1, GPIO_PIN0); // toggle LED on/off
	}
}

Void clockHandler1(UArg arg)
{
	Task_yield();               	// To force a task switch if needed
}
