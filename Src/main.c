#include <stdint.h>
#include "gpio.h"
#include "systick.h"

int main(void)
{
    bool btnState;

    initGPIOPin();

    while (1)
    {
        writeGPIOPinBSSR(ON);
        systickMsecDelay(1000);
        writeGPIOPinBSSR(OFF);
        systickMsecDelay(1000);
    }
}

