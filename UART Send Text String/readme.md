## Send Text String

This program sends a null-terminated character string to the UART controller and displays it on a terminal program running on a laptop/PC.

Key features include:
- Setting up a terminal program (e.g., PuTTY) to communicate with the LaunchPad via UART at 9600 baud.
- Importing and running an example project to ensure hardware and software functionality.
- Adding a function `send_message(char* msg)` to send a string to the UART controller by directly accessing the controller's registers.
- Checking the status register (UCA0STATW) to ensure the controller is ready to receive a character before sending it.
- Testing the function in `main()` by sending a predefined string and verifying the echoing functionality.

This program demonstrates UART communication by sending and receiving data between the LaunchPad and a terminal program, showcasing the use of direct register access for UART transmission.