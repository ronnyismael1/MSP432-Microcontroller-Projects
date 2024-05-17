#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

char* string1 = "xyz123";
char string2[] = "xyz123";
const char string3[1024 * 160] = {"xyz123"};

void main(void)
{
    MAP_WDT_A_holdTimer();

    // Print out the strings first
    printf("\n");
    printf("string1: %s\n", string1);
    printf("string2: %s\n", string2);
    printf("string3: %s\n\n", string3);

    // Modified the first two elements in string2
    string2[0] = '0';
    string2[1] = '3';

    // Writing to flash memory
    uint32_t bank_number, sector_mask;

    // Un-protect and program string1
    if(get_flash_bank_sector((uint32_t)string1, &bank_number, &sector_mask)) {
        uint32_t memorySpace = (bank_number == 0) ? FLASH_MAIN_MEMORY_SPACE_BANK0 : FLASH_MAIN_MEMORY_SPACE_BANK1;
        FlashCtl_unprotectSector(memorySpace, sector_mask);
        ROM_FlashCtl_programMemory(string2, string1, 2);
    }

    // Un-protect and program string3
    if(get_flash_bank_sector((uint32_t)string3, &bank_number, &sector_mask)) {
        uint32_t memorySpace = (bank_number == 0) ? FLASH_MAIN_MEMORY_SPACE_BANK0 : FLASH_MAIN_MEMORY_SPACE_BANK1;
        FlashCtl_unprotectSector(memorySpace, sector_mask);
        ROM_FlashCtl_programMemory(string2, (void*)string3, 2);
    }

    // Print out strings again once finished changing the contents
    printf("string1: %s\n", string1);
    printf("string2: %s\n", string2);
    printf("string3: %s\n", string3);
    printf("Bank Number: %X, Sector Mask: %X\n", bank_number, sector_mask);
}

bool get_flash_bank_sector(uint32_t mem_address, uint32_t* bank_number, uint32_t* sector_mask)
{
    // Each bank has 128KB and each sector is 4KB
    const uint32_t sectorSize = 0x1000; // 4KB per sector
    const uint32_t bankSize = 0x20000; // 128KB per bank

    // Calculate bank number and sector within the bank
    *bank_number = mem_address / bankSize;
    uint32_t sector_within_bank = (mem_address % bankSize) / sectorSize;

    // Set the sector mask for only one sector
    *sector_mask = 1 << sector_within_bank;

    return true;
}
