## Control LEDs with Push Buttons

This program controls two LEDs using two different push buttons, with each button-LED pair managed by an independent task. The left push button (S1) controls the red LED (LED1), and the right push button (S2) controls the green LED (LED2).

Key features include:
- Two tasks, each in an infinite control loop, managing the state of one LED based on the corresponding push button.
- Polling method to read the push button state and turn the LED on or off accordingly.
- Device setup is done within each task function, ensuring modularity and separation of concerns.
- Tasks share processor time under an RTOS, each getting a 10 ms time slice for execution.

The program demonstrates effective task management and resource sharing using an RTOS, allowing responsive control of LEDs based on push button inputs.