## Temperature Reporting Program

This program periodically samples the temperature using the MCU's ADC and displays the reading on a remote terminal window. The temperature reading is formatted as "Txx.x " (e.g., "T72.1 ") and sent via the GPIO Tx pin every 5000 ms. 

Key features include:
- Sampling temperature using software-triggered ADC.
- Sending temperature readings in ASCII format with one decimal place.
- Running the sampling task under an RTOS for efficient timing control using `Task_sleep()`.
- Simplified operation without interrupt mechanisms, using a busy-wait loop to check conversion status with `ADC14_isBusy()`.