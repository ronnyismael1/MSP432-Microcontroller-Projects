## Report Push Button State

This program samples the state of an input push button and displays it on a terminal window via the UART's Tx pin. The state is read every 100 ms and reported only if it changes, saving communication bandwidth.

Key features include:
- Sampling the push button state periodically.
- Sending state updates formatted as "S10 " for pressed and "S11 " for not pressed, using software to create the required bit streams at 1200 baud.
- Using `Task_sleep()` for timing control under an RTOS.
- Utilizing `Timestamp_get32()` from the TI-RTOS to produce consistent bit times, ensuring correct UART protocol timing.
- Initial state sent unconditionally at program startup to inform the connected device of the current state.

The project avoids using the UART controller directly, employing a software-based approach for bit stream generation.