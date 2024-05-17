## Multitask Programming to Blink LEDs

This program independently controls two LEDs using two tasks. Each task blinks one LED at a specified rate, demonstrating multitask programming with an RTOS.

Key features include:
- Task 1 blinks the red LED (LED1) at a rate of 2 Hz.
- Task 2 blinks the green LED (LED2) at a rate of 0.5 Hz.
- Each task sets up the GPIO port for its respective LED and enters a forever loop to toggle the LED state.
- Device setup is done within each task function, maintaining the modularity of the tasks.
- Timing control is managed using `Task_sleep(time_ms)`, ensuring efficient use of the RTOS for task scheduling.

This program showcases the use of an RTOS for managing multiple tasks, allowing independent and concurrent control of hardware components.