#include <stdint.h>
#include "gpio.h"

int main(void)
{
    initGPIOPin();
    while (1)
    {
        writeGPIOPin(ON);
        for (volatile int i = 0; i < 1000000; i++); // Delay
        writeGPIOPin(OFF);
        for (volatile int i = 0; i < 1000000; i++); // Delay
    }
}

