#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define COUNTER_FLASH_ADDRESS (0x0003F000)

int main(void) {
    WDT_A_holdTimer();

    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN0); // Red LED
    GPIO_setAsOutputPin(GPIO_PORT_P2, GPIO_PIN1); // Green LED
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN0);
    GPIO_setOutputLowOnPin(GPIO_PORT_P2, GPIO_PIN1);

    if (decrementCounter()) {
        blinkGreenLED();
    } else {
        turnOnRedLED();
    }

    return 0;
}

bool decrementCounter(void) {
    uint32_t counterValue;
    // Read the current counter value
    memcpy(&counterValue, COUNTER_FLASH_ADDRESS, sizeof(counterValue));

    if (counterValue == 0xFFFFFFFF) { // First time running, initialize counter
        counterValue = 0x7; // Allows for 3 power cycles before expiring
    } else if (counterValue != 0) {
        // Decrement the counter by clearing the least significant set bit
        counterValue &= (counterValue - 1);
    } else {
        return false;
    }

    FlashCtl_unprotectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);
    bool result = FlashCtl_programMemory(&counterValue, (void*)COUNTER_FLASH_ADDRESS, sizeof(counterValue));
    FlashCtl_protectSector(FLASH_MAIN_MEMORY_SPACE_BANK1, FLASH_SECTOR31);

    return result && counterValue != 0;
}

void blinkGreenLED(void) {
    while (1) {
        GPIO_toggleOutputOnPin(GPIO_PORT_P2, GPIO_PIN1);
        __delay_cycles(3000000);
    }
}

void turnOnRedLED(void) {
    GPIO_setOutputHighOnPin(GPIO_PORT_P2, GPIO_PIN0);
    while (1) {
        // Infinite loop
    }
}
