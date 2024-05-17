#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdio.h>

#define CRC32_POLY          	0xEDB88320
#define CRC32_INIT          	0xFFFFFFFF
static uint32_t calculateCRC32(uint8_t *data, uint32_t length);
uint32_t compute_simple_checksum(uint8_t *data, uint32_t length);
volatile uint32_t hwCalculatedCRC1, hwCalculatedCRC2, hwCalculatedCRC3, crc1,
    	crc2, crc3;
volatile uint8_t newData[10240];

//![Simple CRC32 Example]
int main(void)
{
	uint32_t ii, j;
	// create a large array for comparing checksums
	for (j = 0; j < 10240; j++)
	{
    	newData[j] = j; 	// array of 0, 1, 2, 3, ....
	}

	/* Stop WDT */
	MAP_WDT_A_holdTimer();

	/*	Original values 	*/
	// Hardware for Original
	MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);
	for (ii = 0; ii < 10240; ii++)
    	MAP_CRC32_set8BitData(newData[ii], CRC32_MODE);
	hwCalculatedCRC1 = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;
	printf("\n(original) The hardware checksum is: 0x%x\n", hwCalculatedCRC1);
	crc1 = compute_simple_checksum((uint8_t*) newData, 10240);
	printf("(original) The calculated checksum is: 0x%x\n", crc1);

	/*	1 modified value 	*/
	printf("The original value of newData[271] = 0x%x\n", newData[271]);
	// Reversing Bit 4 of the value in myData[271]
	newData[271] ^= (1 << 3);  // XOR with a bitmask having 1 at Bit 4
	printf("The modified value of newData[271] = 0x%x\n", newData[271]);
	MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);
	for (ii = 0; ii < 10240; ii++)
    	MAP_CRC32_set8BitData(newData[ii], CRC32_MODE);
	hwCalculatedCRC2 = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;
	printf("(1 modified) The hardware checksum is: 0x%x\n", hwCalculatedCRC2);
	crc2 = compute_simple_checksum((uint8_t*) newData, 10240);
	printf("(1 modified) The calculated checksum is: 0x%x\n", crc2);

	/*	2 modified values 	*/
	printf("The original value of newData[528] = 0x%x\n", newData[528]);
	// Reversing Bit 4 of the value in myData[528]
	newData[528] ^= (1 << 3);  // XOR with a bitmask having 1 at Bit 4
	printf("The modified value of newData[528] = 0x%x\n", newData[528]);
	MAP_CRC32_setSeed(CRC32_INIT, CRC32_MODE);
	for (ii = 0; ii < 10240; ii++)
    	MAP_CRC32_set8BitData(newData[ii], CRC32_MODE);
	hwCalculatedCRC3 = MAP_CRC32_getResultReversed(CRC32_MODE) ^ 0xFFFFFFFF;
	printf("(2 modified) The hardware checksum is: 0x%x\n", hwCalculatedCRC3);
	crc3 = compute_simple_checksum((uint8_t*) newData, 10240);
	printf("(2 modified) The calculated checksum is: 0x%x\n", crc3);

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
