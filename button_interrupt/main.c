#include<stdint.h>
#define ENABLE 1 
#define GPIO_PIN_NO_13 13
#define __IO    volatile
#define NO_OF_PRIORITY_BITS_IMPLEMENTED		4//in case of ST

/*PROCESSOR (ARM CORTEX M4) SPECIFIC HEADER MACRO DEFINITIONS
 */
//ARM Cortex Mx Processor NVIC ISERx register address
#define NVIC_ISER0				((__IO uint32_t*)0xE000E100)
#define NVIC_ISER1				((__IO uint32_t*)0xE000E104)//ISER0 + 4 bytes
#define NVIC_ISER2				((__IO uint32_t*)0xE000E108)
#define NVIC_ISER3				((__IO uint32_t*)0xE000E10C)

#define NVIC_ICER0				((__IO uint32_t*)0xE000E180)
#define NVIC_ICER1				((__IO uint32_t*)0xE000E184)
#define NVIC_ICER2				((__IO uint32_t*)0xE000E188)

#define NVIC_PR_BASE_ADDR		((uint32_t*)0xE000E400)


#define NVIC_IRQ_PRIO0		0
#define NVIC_IRQ_PRIO1		1
#define NVIC_IRQ_PRIO2		2
#define NVIC_IRQ_PRIO3		3
#define NVIC_IRQ_PRIO4		4
#define NVIC_IRQ_PRIO5		5
#define NVIC_IRQ_PRIO6		6
#define NVIC_IRQ_PRIO7		7
#define NVIC_IRQ_PRIO8		8
#define NVIC_IRQ_PRIO9		9
#define NVIC_IRQ_PRIO10	  10
#define NVIC_IRQ_PRIO11		11
#define NVIC_IRQ_PRIO12		12
#define NVIC_IRQ_PRIO13		13
#define NVIC_IRQ_PRIO14		14
#define NVIC_IRQ_PRIO15		15

//some generic macros

#define IRQ_NO_EXTI0		  6
#define IRQ_NO_EXTI1		  7
#define IRQ_NO_EXTI2		  8
#define IRQ_NO_EXTI3		  9
#define IRQ_NO_EXTI4		  10
#define IRQ_NO_EXTI9_5		23
#define IRQ_NO_EXTI15_10	40
#define AHB1PERIPH_BASE		(0x40020000UL)
	
#define GPIOA_BASE            (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOB_BASE            (AHB1PERIPH_BASE + 0x0400UL)
#define GPIOC_BASE            (AHB1PERIPH_BASE + 0x0800UL)
#define SYSCFG_BASE			(0x40013800UL)
#define EXTI_BASE				(0x40013C00UL)
#define GPIOA					((GPIO_TypeDef*)GPIOA_BASE)
#define GPIOC					((GPIO_TypeDef*)GPIOC_BASE)
#define RCC						((RCC_TypeDef*)RCC_BASE)
#define SYSCFG				((SYSCFG_TypeDef*)SYSCFG_BASE)
#define EXTI					((EXTI_TypeDef*)EXTI_BASE)

#define RCC_BASE				(0x40023800UL)	
typedef struct
{
  __IO uint32_t MODER;
  __IO uint32_t OTYPER;
  __IO uint32_t OSPEEDR;
  __IO uint32_t PUPDR;
  __IO uint32_t IDR;
  __IO uint32_t ODR;
  __IO uint32_t BSRR;
  __IO uint32_t LCKR;
  __IO uint32_t AFR[2];
}GPIO_TypeDef;
typedef struct
{
  __IO uint32_t USELESSRN[12];
  __IO uint32_t AHB1ENR;
  __IO uint32_t AHB2ENR;
  __IO uint32_t AHB3ENR;
  uint32_t      RESERVED2;
  __IO uint32_t APB1ENR;
  __IO uint32_t APB2ENR;

} RCC_TypeDef;


typedef struct
{
  __IO uint32_t MEMRMP;
  __IO uint32_t PMC;
  __IO uint32_t EXTICR[4];
  uint32_t      RESERVED[2];
  __IO uint32_t CMPCR;
  uint32_t      RESERVED1[2];
  __IO uint32_t CFGR;
} SYSCFG_TypeDef;


typedef struct
{
  __IO uint32_t IMR;
  __IO uint32_t EMR;
  __IO uint32_t RTSR;
  __IO uint32_t FTSR;
  __IO uint32_t SWIER;
  __IO uint32_t PR;
} EXTI_TypeDef;

//function prototypes
void GPIO_IRQConfig(uint8_t IRQNumber,uint8_t EnorDi);
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority);
void delay(void);

int main(void) 
{
    // Enable clocks for GPIO A (PA5 LED) and GPIO C (PC13 button)
    RCC->AHB1ENR |= (1U << 0);
    RCC->AHB1ENR |= (1U << 2);

    // Set PA5 as LED: output mode, push-pull, low speed
    GPIOA->MODER |= (1U << 10);
    GPIOA->MODER &= ~(1U << 11);
    GPIOA->OTYPER &= ~(1U << 5);

    // Set PC13 as input with pull-up
    GPIOC->MODER &= ~(1U << 26);
    GPIOC->MODER &= ~(1U << 27);
    GPIOC->OSPEEDR |= (1U << 27);      // fast speed
    GPIOC->PUPDR |= (1U << 26);        // pull-up
    GPIOC->PUPDR &= ~(1U << 27);

  
    // Enable SYSCFG clock
    RCC->APB2ENR |= (1U << 14);

    // Map EXTI13 line to Port C
    SYSCFG->EXTICR[3] &= ~(0xF << 4);
    SYSCFG->EXTICR[3] |=  (0x2 << 4);  // 0x2 = Port C

    // Unmask EXTI line 13
    EXTI->IMR |= (1U << 13);

    // Configuring falling edge trigger (button press)
    EXTI->FTSR |= (1U << 13);
    EXTI->RTSR &= ~(1U << 13);//disabling rising edge


    // IRQ configurations
    GPIO_IRQPriorityConfig(40, 15);
    GPIO_IRQConfig(40, ENABLE);

    while(1); // wait for interrupt

    return 0;
}

void delay(void) 
{
    for(uint32_t i = 0; i < 500000/2; i++); // ~200ms delay at 16MHz
}

void EXTI15_10_IRQHandler(void) 
{
    delay(); //

    
    if (EXTI->PR & (1 << GPIO_PIN_NO_13)) 
    {
        EXTI->PR |= (1 << GPIO_PIN_NO_13);   // clear pending bit
        GPIOA->ODR^=(1U<<5);      //toggle LED
    }

}
void GPIO_IRQPriorityConfig(uint8_t IRQNumber,uint8_t IRQPriority)
{
	uint8_t iprx=IRQNumber/4;//finding out the IPR register number
	uint8_t iprx_section=IRQNumber%4;//finding out which section of the register 0,1,2 or 3
	uint8_t shift_amount=(8*iprx_section)+ (8*NO_OF_PRIORITY_BITS_IMPLEMENTED);
	*(NVIC_PR_BASE_ADDR+iprx*4)|=(IRQPriority<<shift_amount);
}
void GPIO_IRQConfig(uint8_t IRQNumber,uint8_t EnorDi)
{

	if(EnorDi==ENABLE)
	{
		if(IRQNumber <= 31)
		{
			//program ISER0 register
			*(NVIC_ISER0)|=(1U<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber<64)
		{
			//program ISER1 register
			*(NVIC_ISER1)|=(1U<<(IRQNumber)%32);
		}
		else if(IRQNumber >=64 && IRQNumber<96)
		{
			//program ISER2 Register
			*(NVIC_ISER2)|=(1U<<(IRQNumber)%32);
		}

	}
	else
	{
		if(IRQNumber<=31)
		{
			//program ICER0 register
			*(NVIC_ICER0)|=(1U<<IRQNumber);
		}
		else if(IRQNumber >=32 && IRQNumber <64)
		{
			//program ICER1 register
			*(NVIC_ICER1)|=(1U<<(IRQNumber)%32);
		}
		else if(IRQNumber >=64 && IRQNumber <96)
		{
			//program ICER2 register
			*(NVIC_ICER2)|=(1U<<(IRQNumber)%32);
		}
	}
}