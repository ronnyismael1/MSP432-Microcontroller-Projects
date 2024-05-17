# Non-volatile Power-on-reset Counter

This project implements a non-volatile power-on-reset counter on an MSP432 microcontroller. The system tracks the number of times the board is powered on and limits its functionality after a set number of power cycles. 

## Features

1. **Power-on Counter**: Uses flash memory to keep track of the number of power-on cycles.
2. **LED Indicators**:
   - Blinks the green LED at 1 Hz if the power-on count is below the threshold.
   - Turns on the red LED and halts if the power-on count exceeds the threshold.

## Functionality

- Allocates a 32-bit unsigned counter in flash memory to track power cycles.
- Each power-on decrements the counter.
- When the counter expires, the system stops blinking the green LED and turns on the red LED.

## Key Functions

- `FlashCtl_unprotectSector`
- `ROM_FlashCtl_programMemory`
- `__delay_cycles`

## Usage

- The program initially blinks the green LED.
- After the set number of power cycles, it switches to the red LED.
- The counter can be adjusted to handle larger numbers of power cycles by changing a single constant.

## Testing

- Disconnect and reconnect the LaunchPad USB cable to simulate power cycles.
- Verify LED behavior through multiple power-on cycles.
- A video demonstrating the power-on cycles can be accessed remotely.