## Emulate UART

This program emulates UART communication by sending a character string bit by bit using software, bypassing the UART controller and directly controlling the GPIO pin.

Key features include:
- Setting up GPIO pin P1.3 as the Tx pin and configuring it as an output.
- Creating UART frames by adding start and stop bits to the data bits.
- Using the delay function `__delay_cycles()` to control the timing for each bit based on the baud rate.
- Defining the baud rate with a constant (`#define BAUD_RATE 1200`) and deriving the timing parameter from it.
- Initializing the Tx pin to its idle state before sending data and ensuring any pre-existing bus transactions are terminated.
- Sending a test string to a terminal program running on a laptop/PC and verifying the output at different baud rates (1200 and 9600).

The program demonstrates basic serial communication concepts by manually controlling the transmission of data bits, allowing a deeper understanding of UART protocol and timing control.