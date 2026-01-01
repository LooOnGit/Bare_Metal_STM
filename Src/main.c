#include <stdint.h>
#include "gpio.h"

int main(void)
{
    initGPIOPin();
    while (1)
    {
        writeGPIOPinBSSR(ON);
        for (volatile int i = 0; i < 1000000; i++); // Delay
        writeGPIOPinBSSR(OFF);
        for (volatile int i = 0; i < 1000000; i++); // Delay
    }
}

