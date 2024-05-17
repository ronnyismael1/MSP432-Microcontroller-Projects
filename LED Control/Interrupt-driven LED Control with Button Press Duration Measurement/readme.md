# Interrupt-driven LED Control with Button Press Duration Measurement

This project extends the interrupt-driven LED control by measuring the duration of the button press using a 32-bit timer and a circular queue for data storage.

## Features

1. **Interrupt-driven LED Control**:
   - Controls the green LED based on the state of push button S1.
   - Green LED off when button is pressed.
   - Green LED on when button is released.

2. **Duration Measurement**:
   - Measures and prints the duration of the button press in milliseconds.
   - Utilizes a 32-bit timer for precise time measurement.

3. **Circular Queue**:
   - Stores records of timestamps and button states.
   - Ensures communication between the ISR and the main control loop without race conditions.

## Key Steps

1. **Setup GPIO and Interrupts**:
   - Configures GPIO interrupts for both high-to-low and low-to-high transitions.
   - Enables interrupts and sets up the ISR to handle button press events.

2. **Implement Circular Queue**:
   - Defines a circular queue to store timestamp and button state records.
   - Uses the queue to pass information from the ISR to the main control loop.

3. **ISR (Interrupt Service Routine)**:
   - Records the timestamp and button state in the circular queue.
   - Manages queue indices and checks for overflow conditions.

4. **Main Control Loop**:
   - Reads records from the queue and calculates the duration of the button press.
   - Prints the duration in milliseconds and the value of `read_index`.
   - Adds a dummy delay to simulate other tasks.

This project demonstrates using interrupts and a circular queue to efficiently measure and process button press durations in a responsive system.