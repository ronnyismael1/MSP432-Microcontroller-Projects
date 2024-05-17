# GPIO Oscillator

This project demonstrates the creation and sampling of an oscillator at a GPIO pin using a capacitive-touch feature on an MCU.

## Overview

- **Oscillator Setup:** Configured a GPIO input pin (P4.1) to form an oscillator using the built-in pull-up/pull-down resistor and parasitic capacitance.
- **Data Sampling:** Sampled the state of the oscillator as fast as possible and measured the data collection rate.
- **Output:** Continuously displayed the binary state of the oscillator in the debug console and calculated the sampling rate in Hz.

## Implementation Details

1. **GPIO Configuration:**
   - Enabled the capacitive-touch feature on Port 4, Pin 1.
   - Configured the CAPTIO0CTL control register to set up the oscillator.

2. **State Reading:**
   - Read the binary state of the oscillator using the CAPTIO0CTL control register.
   - Used a forever loop to display the state continuously in the debug console.

3. **Data Collection:**
   - Declared a data array of 100 elements to store the oscillator output.
   - Used a for loop to read the oscillator output and store the data without unnecessary overheads.

4. **Sampling Rate Measurement:**
   - Timed the data collection loop using a Timer32 timer.
   - Calculated the sampling rate in Hz and printed it to the debug console.

## How to Run

- The project outputs a continuous stream of '1's and '0's indicating the oscillator's state.
- After data collection, it displays the sampled bit stream and the calculated sampling rate in Hz.
