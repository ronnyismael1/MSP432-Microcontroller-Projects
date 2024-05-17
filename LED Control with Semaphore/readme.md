## LED Control with Semaphore

This program controls an LED using two tasks with semaphore synchronization. One task handles the timing for LED state changes, while the other task turns the LED on or off.

Key features include:
- Task 1 controls the frequency of LED blinking at 0.5 Hz, posting to a semaphore to signal state changes.
- Task 2 sets up the GPIO port for the LED and changes the LED state when the semaphore is available.
- The semaphore ensures proper task synchronization, preventing simultaneous access to shared resources.
- Timing control is managed using `Task_sleep()`, avoiding hardware timers and custom delay functions.

The tasks operate under an RTOS, demonstrating the use of semaphores for efficient task coordination and resource management.