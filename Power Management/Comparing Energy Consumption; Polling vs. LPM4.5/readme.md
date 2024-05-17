## Comparing Energy Consumption: Polling vs. LPM4.5

This exercise compares the energy consumption of two implementations of a task that blinks the red LED (LED1) when the push button S1 is pressed: one using polling and the other using interrupt with low-power mode LPM4.5.

Key steps:
1. **Set Up Project**: Duplicate the empty example project and replace the main file with `Lab6_Ex2_2_LPM45.c`.
2. **Implement Polling Method**: Add code to implement the polling method in the provided code structure.
3. **Run and Measure Energy Consumption**:
   - For the interrupt method (LPM4.5), run the provided code.
   - For the polling method, define `#define USE_POLLING` and implement the necessary code to monitor the state of S1 and blink the LED.
4. **Collect Data**:
   - Build the program and load it to the LaunchPad without running it in debug mode.
   - Start EnergyTrace, set the measurement duration to 10 seconds, and perform a hardware reset.
   - Start data collection, press the push button S1 to blink the LED, and record the energy and mean current values.
   - Repeat the measurements three times for each method.
5. **Analyze Results**:
   - Compute the average energy and mean current for both methods.
   - Calculate the percentage energy saving by using LPM4.5 compared to the polling method.

### Code Implementation for Polling Method
```c
while(1)
{
#ifdef USE_POLLING
    // Implement polling method to monitor S1 and blink LED1
    if (GPIO_getInputPinValue(GPIO_PORT_P1, GPIO_PIN1) == GPIO_INPUT_PIN_LOW) {
        GPIO_setOutputHighOnPin(GPIO_PORT_P1, GPIO_PIN0); // Turn on LED1
        __delay_cycles(3000000); // Delay to keep LED1 on
        GPIO_setOutputLowOnPin(GPIO_PORT_P1, GPIO_PIN0); // Turn off LED1
    }
#else
    MAP_PCM_shutdownDevice(PCM_LPM45);
#endif
}
```

### Steps to Measure Energy Consumption
1. Build and load the program to the LaunchPad.
2. Start EnergyTrace and set the measurement duration to 10 seconds.
3. Perform a hardware reset on the LaunchPad.
4. Start data collection in EnergyTrace and press S1 to blink the LED.
5. Record energy and mean current values.
6. Repeat the measurement three times for each method.
