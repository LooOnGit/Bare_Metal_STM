#include <stdio.h>
#include "gpio.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"

int main(void)
{
    /*Initialize systick timer*/
    uartInit();

    pa1ADCInit();
    startConversion();

    while (1)
    {
        uint32_t adcValue = adcRead();
        // uartSendString("Hello, UART!\r\n");
        // systickMsecDelay(1000);
    }
}

