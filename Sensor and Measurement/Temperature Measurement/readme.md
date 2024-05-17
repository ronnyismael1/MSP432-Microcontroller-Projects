## Temperature Measurement

This program converts an analog temperature sensor reading to a digital value and then transforms it into a temperature in Fahrenheit using calibration data stored in the MCU's ROM.

Key features include:
- Utilizing the ADC to digitize the temperature sensor output voltage.
- Accessing calibration data using `SysCtl_getTempCalibrationConstant()` for accurate temperature conversion.
- Calculating temperature based on two calibration points at 30°C and 85°C, with a 1.2 V reference voltage.
- Displaying the temperature in Fahrenheit with two decimal places of precision.
- Demonstrating sensor sensitivity by showing higher temperature readings when the MCU surface is touched.

This program highlights the use of calibration data for accurate temperature measurement and the sensor's responsiveness to environmental changes.