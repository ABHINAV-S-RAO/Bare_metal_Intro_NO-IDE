//on board LED is connected to PA5
#define PERIPH_BASE     		(0x40000000UL)
#define AHB1PERIPH_OFFSET 		(0x00020000UL)
#define AHB1PERIPH_BASE			(PERIPH_BASE + AHB1PERIPH_OFFSET)
#define GPIOA_OFFSET 			(0x0UL)//
#define GPIOA_BASE 				(AHB1PERIPH_BASE + GPIOA_OFFSET)
#define RCC_OFFSET 				(0x23800UL)
#define RCC_BASE				(PERIPH_BASE + RCC_OFFSET)
#define RCC_AHB1ENR				(*(volatile unsigned int*)(RCC_BASE + 0x30UL))//AHB1ENR's offset is 0x30 from reference manual
#define GPIOx_MODER_OFFSET		(0x00UL)
#define ODR_OFFSET				(0x14UL)
#define GPIOA_ODR				(*(volatile unsigned int*)(GPIOA_BASE + ODR_OFFSET))
#define GPIOA_MODER				(*(volatile unsigned int*)(GPIOA_BASE + GPIOx_MODER_OFFSET))
#define GPIOAEN					(1U<<0)
#define PIN5					(1U<<5)
#define LED_PIN					PIN5

int main(void)
{
	//Enabling Clock access to GPIOA
	RCC_AHB1ENR |= GPIOAEN;

	//Set PA5 as output pin acc to ref manual
	GPIOA_MODER |= (1U<<10);
	GPIOA_MODER &=~ (1U<<11);

	while(1)
	{
		// Toggle PA5
		GPIOA_ODR ^= LED_PIN;//XOR will toggle the req bit in the given register
		for(int i=0;i<100000;i++)//this is for delay for a visible blink
		{

		}
	}

}
