#include <stdint.h>
#include "gpio.h"
#include "systick.h"
#include "timer.h"

int main(void)
{
    timInit();
    initGPIOPin();

    while (1)
    {
        writeGPIOPinBSSR(ON);
        while (!getUIF()){}
        clearUIF();
        writeGPIOPinBSSR(OFF);
        while (!getUIF()){}
        clearUIF();
    }
}

