# LED Control Using Capacitive Touch

This project uses the capacitive touch feature to toggle an LED on and off, simulating a touch-sensitive switch.

## Overview

- **LED Control:** Utilized the capacitive touch feature to control the red LED (LED1) on the LaunchPad.
- **Touch Detection:** Determined touch events based on the frequency change of the oscillator at P4.1.
- **Calibration:** Performed dynamic calibration at startup to determine reliable control parameters.

## Implementation Details

1. **Initial Setup:**
   - Configured P4.1 as a capacitive touch sensor.
   - Initialized the red LED (LED1) to be off at startup.

2. **Calibration:**
   - Assumed no touch on the J1 connector during calibration.
   - Took multiple frequency measurements to establish a baseline frequency.
   - Calculated control parameters dynamically and printed them to the debug console.

3. **Touch Detection:**
   - Continuously measured the oscillator frequency.
   - Determined a touch event based on the frequency falling below a dynamically set threshold.

4. **LED Toggle Logic:**
   - Toggled the state of the LED on each detected touch event.
   - Ensured that holding the touch did not continuously toggle the LED.

5. **Real-Time Response:**
   - Avoided printing measurement results in the control loop to minimize response time.
   - Ensured immediate LED response to touch actions for real-time performance.

## How to Run

- The project initializes with the LED off.
- On touching the J1 connector, the LED toggles between on and off states.
- Calibration parameters are printed at startup; subsequent touch events control the LED without delay.
