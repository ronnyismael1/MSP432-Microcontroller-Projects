# Frequency Measurement Using Timer_A

This project measures the frequency of an oscillator using the Timer_A module and detects changes induced by touching the GPIO pin.

## Overview

- **Frequency Measurement:** Utilized Timer_A to measure the frequency of an oscillator at Port 4, Pin 1 (P4.1).
- **Capacitance Detection:** Changed the oscillator frequency by touching the GPIO pin to alter its capacitance.
- **Data Visualization:** Collected frequency data over time and visualized the changes in a graph.

## Implementation Details

1. **Hardware Setup:**
   - Connected P4.1 to the Timer_A module.
   - Used a wire or a metal paper clip to increase the touch surface area for better capacitance change detection.

2. **Timer Configuration:**
   - Configured Timer_A to count incoming pulses from the oscillator in continuous mode.
   - Set the clock source to the inverted external TXCLK and the clock divider to 1 for maximum precision.

3. **Frequency Calculation:**
   - Cleared the counter before each measurement and used a 3 ms measurement window.
   - Calculated the frequency from the counter value and printed it to the debug console.

4. **Measurement Loop:**
   - Ran the measurement in a forever loop, touching the GPIO pin to observe frequency changes.
   - Collected data for about 10 seconds using the Timer32 timer for timing.

5. **Data Analysis:**
   - Took measurements without touching the pin and created a graph to show frequency changes over time.
   - Repeated the measurements with touches to observe small and large frequency changes and visualized the results.

## How to Run

- The project continuously measures and prints the frequency of the oscillator to the debug console.
- Collect data from the console output to create graphs showing frequency changes over time.
