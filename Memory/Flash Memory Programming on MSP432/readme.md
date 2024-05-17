# Flash Memory Programming on MSP432

This project demonstrates how to program the flash memory on an MSP432 microcontroller using DriverLib functions. It includes:

1. Initializing and modifying string arrays.
2. Unprotecting flash memory sectors.
3. Programming flash memory to duplicate data between strings.

Key functions used:
- `FlashCtl_unprotectSector`
- `ROM_FlashCtl_programMemory`
- `get_flash_bank_sector`

The program prints string contents before and after modification to verify successful flash memory programming.