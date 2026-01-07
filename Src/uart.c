#include "uart.h"

void uartInit(void)
{
    /*Enable clock access to GPIOA*/
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;

    /*Set the mode of PA2 to alternate function mode*/
    GPIOA->MODER &= ~(GPIO_MODER_MODER4);
    GPIOA->MODER |= (GPIO_MODER_MODER5);

    /*Set alternate function type to AF7 (UART2_TX)*/
    

    /*Enable clock access to UART2*/

    /*Configure uart baudrate*/
    
    /*Configure transfer direction*/

    /*Enable UART Module*/

}