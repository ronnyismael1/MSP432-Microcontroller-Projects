// Program to digitize the MCU's internal temperature sensor's output voltage
// DriverLib Includes
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

volatile bool adc_done;

int main(void)
{
	// Halt WDT
	WDT_A_holdTimer();
	printf("MCLK: %u\n", MAP_CS_getMCLK());

	// Set reference voltage to 1.2 V and enable temperature sensor
	REF_A_enableReferenceVoltage();
	REF_A_enableTempSensor();
	REF_A_setReferenceVoltage(REF_A_VREF1_2V);

	// Initializing ADC (MCLK/1/1) with temperature sensor routed
	ADC14_enableModule();
	ADC14_initModule(ADC_CLOCKSOURCE_MCLK, ADC_PREDIVIDER_1, ADC_DIVIDER_1,
	ADC_TEMPSENSEMAP);

	// Set resolution
	ADC14_setResolution(ADC_14BIT);

	// Configure ADC Memory for temperature sensor data
	ADC14_configureSingleSampleMode(ADC_MEM0, false);
	ADC14_configureConversionMemory(ADC_MEM0, ADC_VREFPOS_INTBUF_VREFNEG_VSS,
	ADC_INPUT_A22,
                                	false);

	// Configure the sample/hold time
	ADC14_setSampleHoldTime(ADC_PULSE_WIDTH_192, ADC_PULSE_WIDTH_192);

	// Enable sample timer in manual iteration mode and interrupts
	ADC14_enableSampleTimer(ADC_MANUAL_ITERATION);

	// Enable conversion
	ADC14_enableConversion();

	// Enabling Interrupts
	ADC14_enableInterrupt(ADC_INT0);
	Interrupt_enableInterrupt(INT_ADC14);
	Interrupt_enableMaster();

	// Trigger conversion with software
	adc_done = false;
	ADC14_toggleConversionTrigger();

	// Calibration data points for our board
	uint16_t callow = SysCtl_getTempCalibrationConstant(SYSCTL_1_2V_REF,
                                                    	SYSCTL_30_DEGREES_C);
	uint16_t calhigh = SysCtl_getTempCalibrationConstant(SYSCTL_1_2V_REF,
                                                     	SYSCTL_85_DEGREES_C);
	// slope of the calibration values. slope is ~ 55/1453
	float calslope = (float) (85 - 30) / (calhigh - callow);

	//printf("Calibration data for 1.2V, 30C = %d\n", calhigh);
	//printf("Calibration data for 1.2V, 85C = %d\n", callow);
	//printf("Slope is %.5f\n", calslope);

	printf("Value	Volt (mV) 	Temperature (C)\n");
	// main loop, will
	while (1)
	{
    	// Trigger conversion with software
    	adc_done = false;
    	ADC14_toggleConversionTrigger();

    	while (!adc_done)
    	{
        	// wait for adc to finish, from the interrupt handler
    	}

    	// get the adc value and convert it into millivolts
    	uint32_t adc_value = ADC14_getResult(ADC_MEM0);
    	float voltage = adc_value * 0.0732421875; //adc *1200.0 / 16384.0 (in mV) for 14 bit resolution and a 1.2 reference voltage

    	// Print ADC value and corresponding voltage, and now the temperature
    	int32_t tmpcalc = adc_value - callow;
    	// something about using the unsigned integers used gave errors, so placed (adc - callow) into an intermediate step
    	float temp = calslope * (tmpcalc) + 30;
    	printf("%d	%.5f   %.2f\n", adc_value, voltage, temp);

    	// Add delay to slow down the values
    	__delay_cycles(500 * 3000); // Delay for 500 ms
	}
}

// This interrupt happens every time a conversion has completed
void ADC14_IRQHandler(void)
{
	uint64_t status;
	status = ADC14_getEnabledInterruptStatus();
	ADC14_clearInterruptFlag(status);

	if (status & ADC_INT0)
	{
    	adc_done = true;
	}
}
