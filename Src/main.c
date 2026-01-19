#include <stdio.h>
#include "gpio.h"
#include "systick.h"
#include "timer.h"
#include "uart.h"
#include "adc.h"
#include "exti.h"

int main(void)
{
    /*Initialize GPIO first*/
    initGPIOPin();
    
    /*Initialize UART for debugging*/
    uartInit();

    /*Initialize EXTI for button*/
    pc13ExtiInit();

    /*Turn off LED initially*/
    writeGPIOPin(OFF);

    /*Main loop*/
    while (1)
    {
        /* Application code here */
    }
}

void EXTI0_IRQHandler(void)
{
    if(EXTI->PR & (1U<<0) != 0)
    {
        /*Clear PR flag*/
        EXTI->PR |= (1U<<0);

        /*Do*/
        toggleGPIOPin();
    }

}


