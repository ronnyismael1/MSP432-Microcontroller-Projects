# DMA Speedup for CRC-32 Checksum Calculation with Variable Block Sizes

This project extends the DMA-based data transfer to the CRC-32 accelerator by measuring performance with different data block sizes on an MSP432 microcontroller.

## Features

1. **DMA Setup**:
   - Configures the DMA controller for data transfer to the CRC-32 accelerator.
   - Uses interrupts to notify the processor upon transfer completion.

2. **Checksum Calculation**:
   - Computes CRC-32 checksum using both DMA and software methods.
   - Measures performance across various data block sizes.

3. **Performance Measurement**:
   - Measures execution time for both methods with different block sizes.
   - Compares the performance of DMA and software methods.

## Key Steps

1. **Global Size Array**:
   - Define an array of different block sizes for measurement: `{2, 4, 16, 32, 64, 128, 256, 786, 1024}`.

2. **Main Loop**:
   - Add a loop in `main()` to iterate over different block sizes.
   - Configure the DMA controller and checksum calculation parameters for each block size.

3. **Checksum Validation**:
   - Ensure both DMA and software methods produce the same checksum for each block size.
   - Display the checksums and execution times for each block size in microseconds (μs).

4. **Implementation Details**:
   - Set up the DMA controller once at the beginning.
   - Use a volatile flag for communication between main and the ISR.
   - Initialize the CRC-32 result register before each transfer.
   - Display the block size, checksums, and execution times in the loop.

## Results

- The project demonstrates the efficiency of DMA for varying data block sizes.
- Both methods produce identical checksums, validating the correctness of the implementation.
- Performance measurements highlight the speedup achieved by using DMA over software-based data transfer methods.