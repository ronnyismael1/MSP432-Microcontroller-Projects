# SRAM Function Performance on MSP432

This project investigates the performance of running a function in SRAM versus flash memory on an MSP432 microcontroller.

## Features

1. **Function Placement in SRAM**: Uses the `#pragma CODE_SECTION` directive to place a function in SRAM.
2. **Performance Comparison**: Measures and compares the execution times of identical functions in SRAM and flash memory.
3. **Clock Frequency Adjustment**: Evaluates performance at both 3 MHz and 48 MHz system clock frequencies.

## Key Steps

- Defined two identical functions: one in SRAM and one in flash memory.
- Measured execution times using a 32-bit timer.
- Printed execution times and starting addresses.
- Adjusted system clock frequency from 3 MHz to 48 MHz for additional data.

The results demonstrate the performance differences between executing code in SRAM and flash memory, as well as the impact of different clock frequencies.