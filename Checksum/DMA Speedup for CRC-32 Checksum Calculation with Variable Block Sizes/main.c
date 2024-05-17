#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#define CRC32_SEED          	0xFFFFFFFF
#define CRC32_INIT          	0xFFFFFFFF

/* Statics */
static volatile uint32_t crcSignature;

/* DMA Control Table */
#if defined(__TI_COMPILER_VERSION__)
#pragma DATA_ALIGN(controlTable, 1024)
#elif defined(__IAR_SYSTEMS_ICC__)
#pragma data_alignment=1024
#elif defined(__GNUC__)
__attribute__ ((aligned (1024)))
#elif defined(__CC_ARM)
__align(1024)
#endif

/*  Variables  */
uint8_t controlTable[1024];
uint32_t hwCalculatedCRC;
//extern uint8_t data_array[];
volatile uint8_t data_array[1024];
uint32_t starthw, endhw, durationhw, startdma, enddma, durationdma;
volatile int dma_done;      	// flag

// Part 3.2 new variables
int size_array[] = { 2, 4, 16, 32, 64, 128, 256, 786, 1024 };
void initDMA(int block_size);
void performDMA(int block_size);
void calculateChecksum(int *data, int size);

int main(void)
{
	/* Initialize Timer32 */
	MAP_Timer32_initModule(TIMER32_0_BASE, TIMER32_PRESCALER_1, TIMER32_32BIT,
	TIMER32_FREE_RUN_MODE);
	MAP_Timer32_startTimer(TIMER32_0_BASE, 0);
	uint32_t ii, dataiterator;

	for (dataiterator = 0; dataiterator < 9; dataiterator++)
	{
    	printf("\nDoing calculations for data block size: %d\n",
           	size_array[dataiterator]);

    	/*       	Hardware Method         	*/

    	starthw = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
    	MAP_CRC32_setSeed(CRC32_SEED, CRC32_MODE);

    	for (ii = 0; ii < size_array[dataiterator]; ii++)
    	{
        	MAP_CRC32_set8BitData(data_array[ii], CRC32_MODE);
    	}
    	// In our original code, we used getResultReversed, and xor with 0xFFFFFFFF
    	// But instead we will use same function as what accelerator uses, to get same result
    	hwCalculatedCRC = MAP_CRC32_getResult(CRC32_MODE);
    	endhw = MAP_Timer32_getValue(TIMER32_0_BASE);  // End timer
    	durationhw = (starthw - endhw) / (MAP_CS_getMCLK() / 1000000); // difference in us
    	printf("The hardware checksum is: 0x%x,  duration: %d (us)\n",
           	hwCalculatedCRC, durationhw);

    	/*       	DMAMethod         	*/

    	/* Halting Watchdog */
    	MAP_WDT_A_holdTimer();

    	/* Configuring DMA module */
    	startdma = MAP_Timer32_getValue(TIMER32_0_BASE); // start time
    	MAP_DMA_enableModule();
    	MAP_DMA_setControlBase(controlTable);

    	/* Setting Control Indexes. In this case we will set the source of the
     	* DMA transfer to our random data array and the destination to the
     	* CRC32 data in register address*/
    	MAP_DMA_setChannelControl(UDMA_PRI_SELECT,
    	UDMA_SIZE_8 | UDMA_SRC_INC_8 | UDMA_DST_INC_NONE | UDMA_ARB_1024);
    	MAP_DMA_setChannelTransfer(UDMA_PRI_SELECT,
    	UDMA_MODE_AUTO,
                               	data_array, (void*) (&CRC32->DI32), size_array[dataiterator]);

    	/* Assigning/Enabling Interrupts */
    	MAP_DMA_assignInterrupt(DMA_INT1, 0);
    	MAP_Interrupt_enableInterrupt(INT_DMA_INT1);
    	MAP_Interrupt_enableMaster();

    	/* Enabling DMA Channel 0 */
    	MAP_DMA_enableChannel(0);

    	/* Forcing a software transfer on DMA Channel 0 */
    	MAP_CRC32_setSeed(CRC32_SEED, CRC32_MODE);  // added this line
    	MAP_DMA_requestSoftwareTransfer(0);

    	//Before main() triggers the DMA transfer, it sets the dma_done flag to 0 and enters a while loop to wait for its state to change.
    	dma_done = 0;

    	// Wait and idle, until the interrupt service routine completes and finishes the service
    	while (!dma_done)
    	{
        	//MAP_PCM_gotoLPM0();
    	}

    	enddma = MAP_Timer32_getValue(TIMER32_0_BASE);  // End timer
    	durationdma = (startdma - enddma) / (MAP_CS_getMCLK() / 1000000); // difference in us
    	printf("The DMA checksum is: 0x%x, duration: %d (us)\n", crcSignature,
           	durationdma);
    	double ratio;
    	ratio = (double) durationdma / durationhw;
    	printf("The Speed up ratio of DMA/hardware: %2.5f\n", ratio);

	}
}

/* Completion interrupt for DMA */
void DMA_INT1_IRQHandler(void)
{
	MAP_DMA_disableChannel(0);
	crcSignature = MAP_CRC32_getResult(CRC32_MODE);
	dma_done = 1;
}
