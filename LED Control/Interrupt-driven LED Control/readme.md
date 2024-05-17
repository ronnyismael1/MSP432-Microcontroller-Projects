# Interrupt-driven LED Control

This project demonstrates using interrupts on an MSP432 microcontroller to control an LED with a push button.

## Features

- **Interrupt Setup**: Configures GPIO interrupts for push button S1.
- **LED Control**:
  - Turns off the green LED when the button is pressed.
  - Turns on the green LED when the button is released.

## Key Steps

1. **Setup GPIO and Interrupts**:
   - Configure GPIO interrupts on both high-to-low and low-to-high transitions for the push button.
   - Enable and handle GPIO interrupts.

2. **ISR (Interrupt Service Routine)**:
   - Toggles the LED state.
   - Changes interrupt edge selection to detect both button press and release.

The main function sets up the GPIO and interrupts, while the ISR manages LED control, ensuring a responsive system to the button press events.