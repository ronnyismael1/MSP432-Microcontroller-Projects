#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#define CRC32_POLY          	0xEDB88320
#define CRC32_INIT          	0xFFFFFFFF

static uint32_t calculateCRC32(uint8_t *data, uint32_t length);
uint32_t compute_simple_checksum(uint8_t *data, uint32_t length);
volatile uint32_t hwCalculatedCRC, swCalculatedCRC, crc;
volatile uint8_t newData[10240];

int main(void)
{
	uint32_t ii, j;
	uint32_t starthw, endhw, durationhw, startsw, endsw, durationsw, start, end,
        	duration;
	// create a large array for comparing checksums
	for (j = 0; j < 10240; j++)
	{
    	newData[j] = j; 	// array of 0, 1, 2, 3, ....
	}

	/* Stop WDT */
	MAP_WDT_A_holdTimer();

	/* Initialize Timer32 */
	MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
	TIMER32_FREE_RUN_MODE);
	MAP_Timer32_startTimer(TIMER32_0_BASE, 0);

	/* Getting the result from the hardware module */
	starthw = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
	// hardware calculations
	MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);
	for (ii = 0; ii < 10240; ii++)
    	MAP_CRC32_set8BitData(newData[ii], CRC32_MODE);
	hwCalculatedCRC = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;
	endhw = MAP_Timer32_getValue(TIMER32_0_BASE); // End timer
	durationhw = (starthw - endhw) / (MAP_CS_getMCLK() / 1000000);
	printf("\nThe hardware checksum is: 0x%x, duration: %d (us)\n",
       	hwCalculatedCRC, durationhw);

	/* Calculating the CRC32 checksum through software */
	startsw = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
	swCalculatedCRC = calculateCRC32((uint8_t*) newData, 10240);
	endsw = MAP_Timer32_getValue(TIMER32_0_BASE); // End timer
	durationsw = (startsw - endsw) / (MAP_CS_getMCLK() / 1000000);
	printf("The software checksum is: 0x%x, duration: %d (us)\n", swCalculatedCRC,
       	durationsw);

	// User created checksum function
	start = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
	crc = compute_simple_checksum((uint8_t*) newData, 10240);
	end = MAP_Timer32_getValue(TIMER32_0_BASE); // End timer
	duration = (start - end) / (MAP_CS_getMCLK() / 1000000);
	printf("The calculated checksum is: 0x%x, duration: %d (us)\n", crc,
       	duration);

	double ratio;
	ratio = (double)durationhw / durationsw;
	printf("The speed up ratio is: %2.5f\n", ratio);
	/* Pause for the debugger */
	__no_operation();
}
//![Simple CRC32 Example]

/* Standard software calculation of CRC32 */
static uint32_t calculateCRC32(uint8_t *data, uint32_t length)
{
	uint32_t ii, jj, byte, crc, mask;
	;
	crc = 0xFFFFFFFF;
	for (ii = 0; ii < length; ii++)
	{
    	byte = data[ii];
    	crc = crc ^ byte;
    	for (jj = 0; jj < 8; jj++)
    	{
        	mask = -(crc & 1);
        	crc = (crc >> 1) ^ (CRC32_POLY & mask);
    	}
	}
	return ~crc;
}
uint32_t compute_simple_checksum(uint8_t *data, uint32_t length)
{
	uint32_t i, total = 0; 	// sum variable
	for (i = 0; i < length; i++)
	{
    	total = total + data[i]; // add all values and invert
	}
	return ~total;
}
