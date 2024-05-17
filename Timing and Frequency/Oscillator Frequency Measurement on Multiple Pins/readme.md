# Oscillator Frequency Measurement on Multiple Pins

This project extends the frequency measurement to three GPIO pins (P4.1, P1.1, and P3.1) using a larger measurement window and averages the results.

## Overview

- **Frequency Measurement:** Measured the oscillation frequencies at P4.1, P1.1, and P3.1.
- **Measurement Window:** Used a 12 ms window to record more oscillation cycles.
- **Average Calculation:** Took 10 measurements for each pin and calculated the average frequency.

## Implementation Details

1. **Pin Configuration:**
   - Configured P4.1, P1.1, and P3.1 as oscillators using the capacitive touch feature.
   - Routed each oscillator output to Timer_A for frequency measurement.

2. **Timer Configuration:**
   - Used Timer_A to count incoming pulses from each oscillator in continuous mode.
   - Set the measurement window to 12 ms for more precise frequency recording.

3. **Frequency Calculation:**
   - Cleared the counter before each measurement and used a 12 ms delay.
   - Took 10 measurements for each pin and calculated the average frequency.
   - Printed the average frequency for each pin to the debug console.

4. **Data Analysis:**
   - Compared the no-touch frequencies of P4.1, P1.1, and P3.1.
   - Investigated any discrepancies by consulting the LaunchPad’s schematic.

## How to Run

- The program measures and prints the average oscillation frequencies of P4.1, P1.1, and P3.1.
- Each frequency value is an average of 10 measurements taken over a 12 ms window.
- Compare the frequencies and investigate any significant differences using the LaunchPad’s schematic.

## Results

- The measured frequencies for the three pins should be similar, especially for P4.1 compared to previous exercises.
- Any significant differences will be explained based on hardware configuration and schematic details.
