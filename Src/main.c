#include <stdint.h>
#include "gpio.h"

int main(void)
{
    bool btnState;

    initGPIOPin();

    while (1)
    {
        btnState = readGPIOPin(0);
        if (btnState == true)
        {
            writeGPIOPinBSSR(ON);
        }
        else
        {
            writeGPIOPinBSSR(OFF);
        }
    }
}

