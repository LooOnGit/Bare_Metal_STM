#include "gpio.h"

void initLed(void)
{
    /*Enable clock for GPIO*/
    RCC->AHB1ENR |= GPIOD_EN;

    /**/


}

void ledOn(void)
{

}

void ledOff(void)
{

}

