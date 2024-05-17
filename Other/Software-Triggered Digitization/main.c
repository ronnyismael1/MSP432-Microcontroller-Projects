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
                                	ADC_INPUT_A22, false);

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



	printf("Value	Voltage (mV)\n");
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
    	float voltage = adc_value * 0.0732421875; //adc *1200 / 16384 (in mV) for 14 bit resolution and a 1.2 reference voltage

    	// Print ADC value and corresponding voltage2
    	printf("%d	%.5f\n", adc_value, voltage);

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
