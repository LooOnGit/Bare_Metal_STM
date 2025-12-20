#include <stdint.h>

#define PERIPH_BASE 		(0x40000000UL)							//1: Define base address for peripherals
#define AHB1PERIPH_OFFSET 	(0x00020000UL)							//2. Offset for AHB1 peripheral bus
#define AHB1PERIPH_BASE 	(PERIPH_BASE + AHB1PERIPH_OFFSET) 		//3. Base address for AHB1 peripherals

#define GPIOD_OFFSET 		(0x00000C00UL)							//4. Offset for GPIOD
#define GPIOD_BASE			(AHB1PERIPH_BASE + GPIOD_OFFSET)		//5. Base address for GPIOD

#define RCC_OFFSET			(0x00003800UL)							//6. Offset for RCC
#define RCC_BASE			(AHB1PERIPH_BASE + RCC_OFFSET)			//7. Base address for RCC

#define AHB1EN_R_OFFSET		(0x30UL) 								//8. Offset for AHB1EN register
#define RCC_AHB1EN_R		(*(volatile unsigned int *)(RCC_BASE + AHB1EN_R_OFFSET)) //9. Address off AHB1EN Register

#define MODER_OFFSET		(0x00UL)								//10. Offset for mode register
#define GPIOD_MODER			(*(volatile unsigned int *)(GPIOD_BASE + MODER_OFFSET)) //11. Address of GPIOD mode register
#define ODR_OFFSET			(0x14UL)								//12. Offset for output data register
#define GPIO_ODR			(*(volatile unsigned int *)(GPIOD_BASE + ODR_OFFSET))	//13. Address of GPIOD output data register
#define GPIOD_EN			(1U << 3)								//14. Bit mask for enabling GPIOD (bit 0)
#define PIN_12				(1U << 13)								//15. Bit mask for GPIOD pin 12
#define LED_1				PIN_12									//16. Alias for PIN12 representing LED pin

int main(void)
{
    RCC_AHB1EN_R |= GPIOD_EN;			//18.Enable clock access to GPIOD

    GPIOD_MODER &= ~(3U << (13 * 2));	//19. Clear bit 24 and 25
    GPIOD_MODER |=  (1U << (13 * 2));	//20. Set bit 24 to 1
    while (1)
    {
        GPIO_ODR |= LED_1;   // PD12 ON
    }
}

