# Time Measurement for Button Press Duration

This project measures the duration of a push button press using a high-precision 32-bit counter/timer on an MSP432 microcontroller.

## Features

1. **Time Measurement Setup**:
   - Configures and starts a 32-bit timer to measure the duration of button presses.

2. **Button Press Duration Calculation**:
   - Computes and displays the duration in milliseconds of how long the button is pressed.

## Key Steps

1. **Timer Initialization**:
   - Set up the 32-bit timer in the main function.
   - Start the timer to begin counting down from 0xFFFFFFFF.
   - Print the system clock frequency for reference.

2. **Button Press Duration Calculation**:
   - Use the timer to record the counter value when the button is pressed and released.
   - Calculate the duration of the button press using the difference in counter values and the system clock frequency.
   - Display the calculated duration in milliseconds using `printf`.

## Implementation

1. **Setup Timer in Main**:
   - Initialize and start the 32-bit timer at the beginning of the main function.
   - Use `MAP_CS_getMCLK()` to get and print the system clock frequency.

2. **Record Counter Values**:
   - Record the counter value when the button is pressed.
   - Record the counter value when the button is released.

3. **Calculate and Display Duration**:
   - Calculate the duration of the button press in milliseconds.
   - Display the duration using `printf` after the button is released.

## Results

- The project demonstrates accurate time measurement of button press duration using a 32-bit timer.
- The duration of each button press is displayed in milliseconds, providing a clear indication of the press length.

This project showcases the use of a high-precision timer to measure and display the duration of a button press on an MSP432 microcontroller.