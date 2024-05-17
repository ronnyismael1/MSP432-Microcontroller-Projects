# Simulate Data Corruption

This project simulates data corruption and examines how it affects checksums using two methods: hardware-accelerated CRC32 and a simple checksum function.

## Features

1. **Checksum Methods**:
   - **Hardware-accelerated CRC32**: Uses DriverLib functions `MAP_CRC32_xxx()`.
   - **Simple Checksum**: Uses `compute_simple_checksum()`.

2. **Data Corruption Simulation**:
   - Reverses Bit 4 of `myData[271]` and `myData[528]` to simulate data corruption.
   - Computes and prints checksums for original, singly corrupted, and doubly corrupted data.

## Key Steps

1. **Original Data**:
   - Compute and print checksums for the original `myData`.

2. **Single Bit Corruption**:
   - Reverse Bit 4 of `myData[271]`.
   - Compute and print checksums.

    ```c
    myData[271] ^= (1 << 4);
    ```

3. **Double Bit Corruption**:
   - Reverse Bit 4 of `myData[528]` (in addition to the previous corruption).
   - Compute and print checksums.

    ```c
    myData[528] ^= (1 << 4);
    ```

4. **Results**:
   - Print checksums for each data set using both methods.
   - Observe how the checksums change with data corruption.

This project highlights the sensitivity of checksum methods to small changes in data, demonstrating their utility in detecting data corruption.