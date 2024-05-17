#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>

const eUSCI_UART_ConfigV1 uartConfig = {
EUSCI_A_UART_CLOCKSOURCE_SMCLK,      	// SMCLK Clock Source
    	78,                                 	// BRDIV = 78
    	2,                                   	// UCxBRF = 2
    	0,                                   	// UCxBRS = 0
    	EUSCI_A_UART_NO_PARITY,              	// No Parity
    	EUSCI_A_UART_LSB_FIRST,              	// LSB First
    	EUSCI_A_UART_ONE_STOP_BIT,           	// One stop bit
    	EUSCI_A_UART_MODE,                   	// UART mode
    	EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION,  // Oversampling
    	EUSCI_A_UART_8_BIT_LEN              	// 8 bit data length
    	};

void send_message(char *msg)
{
	// Pointers to access UCA0TXBUF and UCA0STATW registers
	volatile uint16_t *TXBUFptr = &UCA0TXBUF;
	volatile uint16_t *STATWptr = &UCA0STATW;

	// Parse until the message encounters the null character
	while (*msg != '\0')
	{
    	while (*STATWptr & UCBUSY)
    	{
       	// wait for status to not be busy
    	}

    	*TXBUFptr = *msg;   // the character is placed into buffer and sent
    	msg++;          	// select next character
	}
}

int main(void)
{
	/* Halting WDT  */
	MAP_WDT_A_holdTimer();

	/* Selecting P1.2 and P1.3 in UART mode */
	MAP_GPIO_setAsPeripheralModuleFunctionInputPin(
        	GPIO_PORT_P1,
        	GPIO_PIN2 | GPIO_PIN3,
        	GPIO_PRIMARY_MODULE_FUNCTION);

	/* Setting DCO to 12MHz */
	CS_setDCOCenteredFrequency(CS_DCO_FREQUENCY_12);

	//![Simple UART Example]
	/* Configuring UART Module */
	MAP_UART_initModule(EUSCI_A0_BASE, &uartConfig);

	/* Enable UART module */
	MAP_UART_enableModule(EUSCI_A0_BASE);

	/* Enabling interrupts */
	MAP_UART_enableInterrupt(EUSCI_A0_BASE, EUSCI_A_UART_RECEIVE_INTERRUPT);
	MAP_Interrupt_enableInterrupt(INT_EUSCIA0);
	MAP_Interrupt_enableSleepOnIsrExit();
	MAP_Interrupt_enableMaster();
	//![Simple UART Example]

	// test function before the while loop:
	static char something[] = { 0x0d, 0x0a, 0x56, 0x52, 0x4f, 0x4f, 0x4d, 0x4d,
                            	0x4d, 0x4d, 0x4d, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x5f, 0x0d, 0x0a,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x2f, 0x7c, 0x5f,
                            	0x7c, 0x7c, 0x5f, 0x5c, 0x60, 0x2e, 0x5f, 0x5f,
                            	0x0d, 0x0a, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x28, 0x20,
                            	0x20, 0x20, 0x5f, 0x20, 0x20, 0x20, 0x20, 0x5f,
                            	0x20, 0x5f, 0x5c, 0x0d, 0x0a, 0x20, 0x20, 0x20,
                            	0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20, 0x20,
                            	0x20, 0x3d, 0x60, 0x2d, 0x28, 0x5f, 0x29, 0x2d,
                            	0x2d, 0x28, 0x5f, 0x29, 0x2d, 0x27, 0x0d, 0x0a,
                            	0x00 };
	send_message(something);

	while (1)
	{
    	MAP_PCM_gotoLPM0();
	}
}

/* EUSCI A0 UART ISR - Echoes data back to PC host */
void EUSCIA0_IRQHandler(void)
{
	uint32_t status = MAP_UART_getEnabledInterruptStatus(EUSCI_A0_BASE);

	if (status & EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
	{
    	MAP_UART_transmitData(EUSCI_A0_BASE,
    	MAP_UART_receiveData(EUSCI_A0_BASE));
	}

}
