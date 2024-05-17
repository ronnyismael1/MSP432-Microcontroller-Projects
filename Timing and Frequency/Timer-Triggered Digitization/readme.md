## Timer-Triggered Digitization

This program digitizes an analog signal with precise timing controlled by Timer_A, enabling accurate and fast sampling intervals.

Key features include:
- Using Timer_A to trigger ADC sampling at a defined frequency of 100 Hz.
- Setting up Timer_A in Up mode with Compare register 1 (TA0CCR1) to generate pulses for ADC triggering.
- Configuring the ADC to sample the temperature sensor on input channel A22.
- Measuring the sampling frequency with Timer32 to ensure accuracy, displaying the measured frequency on the debug console.
- Updating a global variable with the latest digitized value in the ISR, while the main control loop displays the value at regular intervals.

This program demonstrates precise timing control for ADC sampling using hardware timers, ensuring accurate digitization of analog signals.