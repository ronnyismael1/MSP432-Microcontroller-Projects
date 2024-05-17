## Software-Triggered Digitization

This program digitizes the temperature sensor's output voltage under software control and displays the converted digital value on the debug console.

Key features include:
- Setting up the voltage reference module (REF_A) to enable the temperature sensor and use a 1.2-V reference voltage.
- Configuring the ADC module (ADC14) to use the main system clock and route the temperature sensor output to a conversion channel.
- Setting the ADC resolution to 14-bit for high-precision digitization.
- Configuring single-sample mode for the ADC and storing the digitized value in ADC_MEM0.
- Setting sample-and-hold time and enabling software-controlled conversion with ADC14.
- Using interrupts to signal the end of digitization and triggering the conversion process manually.

The main loop continually digitizes the sensor output voltage, displays the digitized value and corresponding voltage in mV, and includes a delay to slow down the display for readability.