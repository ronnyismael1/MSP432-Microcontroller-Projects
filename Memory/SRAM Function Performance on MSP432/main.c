#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
uint32_t startflash, endflash, durationflash, startsram, endsram, durationsram;

#pragma CODE_SECTION(sram_function, ".TI.ramfunc")
void sram_function()
{
	// store these variables into the CPU register
	register int i;
	register int count = 100000;
	for (i = 0; i < count; i++)
	{
    	// no data access
	}
}
void flash_function()
{
	// store these variables into the CPU register
	register int i;
	register int count = 100000;
	for (i = 0; i < count; i++)
	{
    	// no data access
	}
}
int main(void)
{
	MAP_WDT_A_holdTimer();  /* Stop Watchdog  */
	MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
	TIMER32_FREE_RUN_MODE);
	MAP_Timer32_startTimer(TIMER32_0_BASE, 0);
	printf("Lab 4, Exercise 3.1:  SRAM/Flash @ 3MHz\n");

	/* SRAM function */
	startsram = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
	sram_function();
	endsram = MAP_Timer32_getValue(TIMER32_0_BASE);  // end time
	durationsram = (startsram - endsram) / (MAP_CS_getMCLK() / 1000000); // difference in us
	printf("Address of SRAM function: 0x%08p\n", sram_function);
	printf("SRAM function duration: %d (us)\n", durationsram);

	/* Flash function */
	startflash = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
	flash_function();
	endflash = MAP_Timer32_getValue(TIMER32_0_BASE);  // end time
	durationflash = (startflash - endflash) / (MAP_CS_getMCLK() / 1000000); // difference in us
	printf("Address of Flash function: 0x%08p\n", flash_function);
	printf("Flash function duration: %d (us)\n", durationflash);

	double ratio = (double) durationsram / durationflash;
	printf("The Speed up ratio of SRAM/Flash: %2.5f\n", ratio);
}
