# 32-bit Checksum Speedup

This project evaluates the performance of three methods for computing a 32-bit checksum on a 10KB data block using an MSP432 microcontroller.

## Features

1. **Checksum Methods**:
   - **Hardware-accelerated CRC32**: Uses DriverLib functions `MAP_CRC32_xxx()`.
   - **Pure Software CRC32**: Uses the provided `calculateCRC32()` function.
   - **Simple Checksum**: Uses `compute_simple_checksum()`.

2. **Performance Measurement**:
   - Measures execution time for each method.
   - Calculates speedup of hardware-accelerated CRC32 over the pure software method.

## Key Steps

1. **Data Initialization**:
   - Initialized a 10KB array `myData` with sequential numbers starting from 0.

2. **Time Measurement**:
   - Measured execution time for each checksum method using a 32-bit timer.
   - Printed execution times in microseconds (μs) and computed checksums.

3. **Speedup Calculation**:
   - Computed the speedup of the hardware-accelerated CRC32 over the pure software CRC32.

The results demonstrate the performance differences between hardware-accelerated and software-based checksum computations.