## Emulate UART with User Input

This program emulates UART communication by sending a predefined message whenever the user types a key on the terminal program. Both transmitting and receiving are handled via GPIO pins, bypassing the UART controller.

Key features include:
- Setting up GPIO pin P1.3 as the Tx pin (output) and P1.2 as the Rx pin (input).
- Checking for user input in the main control loop.
- Calling `send_message()` to send the predefined message when a key is pressed.
- Manually controlling the Tx pin to emulate UART transmission.

This program demonstrates handling user input and sending responses using software to emulate UART communication, reinforcing serial communication concepts and GPIO usage.