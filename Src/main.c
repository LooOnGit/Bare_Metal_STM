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

    while (1)
    {
        uartSendString("Hello, UART!\r\n");
        systickMsecDelay(1000);
    }
}

