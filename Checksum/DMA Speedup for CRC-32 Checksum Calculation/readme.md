# DMA Speedup for CRC-32 Checksum Calculation

This project uses Direct Memory Access (DMA) to transfer data to the CRC-32 accelerator on an MSP432 microcontroller, comparing its performance with a software-based data transfer method.

## Features

1. **DMA Setup**:
   - Configures the DMA controller to transfer data to the CRC-32 accelerator.
   - Uses interrupts to notify the processor upon transfer completion.

2. **Checksum Calculation**:
   - Computes CRC-32 checksum using both DMA and software methods.
   - Compares the performance of both methods.

## Key Steps

1. **DMA Configuration**:
   - Enable the DMA controller and set the control base.
   - Configure DMA channel for 8-bit data transfers, with source address incrementing and destination address fixed.
   - Assign and enable DMA interrupts.

2. **Checksum Calculation Methods**:
   - **Hardware Method**: Transfers data to the CRC-32 accelerator using software.
   - **DMA Method**: Transfers data to the CRC-32 accelerator using DMA.

3. **Performance Measurement**:
   - Measure the execution time for both methods in microseconds (μs) using a 32-bit timer.
   - Calculate and display the speedup of the DMA method over the hardware method.

4. **Checksum Validation**:
   - Ensure both methods produce the same CRC-32 checksum.

5. **Implementation Details**:
   - Use a volatile flag for communication between main and the ISR.
   - Initialize the CRC-32 result register before each transfer.
   - Display the checksums in hexadecimal format.

This project demonstrates the efficiency and speedup achieved by using DMA for data transfers compared to traditional software methods.