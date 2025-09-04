#include <stdint.h>
#define __IO volatile
#define UART2EN				(1U<<17)
#define GPIOAEN				(1U<<0)
#define SYS_FREQ			16000000//16Mhz
#define APB1_CLK			SYS_FREQ
#define CR1_TE				(1U<<3)//TE stands for transmitter enable, CR1 stands for Control Register 1
#define CR1_UE				(1U<<13)
#define SR_TXE				(1U<<7)// stands for Status Register--->transmit data register empty
#define UART_BAUDRATE		115200
#define AHB1PERIPH_BASE		(0x40020000UL)
#define GPIOA_BASE          (AHB1PERIPH_BASE + 0x0000UL)
#define GPIOA				((GPIO_TypeDef*)GPIOA_BASE)
#define RCC					((RCC_TypeDef*)RCC_BASE)
#define USART2_BASE			(0x40004400U)
#define USART2      		((USART_TypeDef*)USART2_BASE)
#define RCC_BASE            (0x40023800)
typedef struct
{
  __IO uint32_t SR;         /*!< USART Status register,                   Address offset: 0x00 */
  __IO uint32_t DR;         /*!< USART Data register,                     Address offset: 0x04 */
  __IO uint32_t BRR;        /*!< USART Baud rate register,                Address offset: 0x08 */
  __IO uint32_t CR1;        /*!< USART Control register 1,                Address offset: 0x0C */
  __IO uint32_t CR2;        /*!< USART Control register 2,                Address offset: 0x10 */
  __IO uint32_t CR3;        /*!< USART Control register 3,                Address offset: 0x14 */
  __IO uint32_t GTPR;       /*!< USART Guard time and prescaler register, Address offset: 0x18 */
} USART_TypeDef;
typedef struct
{
  __IO uint32_t MODER;    /*!< GPIO port mode register,               Address offset: 0x00      */
  __IO uint32_t OTYPER;   /*!< GPIO port output type register,        Address offset: 0x04      */
  __IO uint32_t OSPEEDR;  /*!< GPIO port output speed register,       Address offset: 0x08      */
  __IO uint32_t PUPDR;    /*!< GPIO port pull-up/pull-down register,  Address offset: 0x0C      */
  __IO uint32_t IDR;      /*!< GPIO port input data register,         Address offset: 0x10      */
  __IO uint32_t ODR;      /*!< GPIO port output data register,        Address offset: 0x14      */
  __IO uint32_t BSRR;     /*!< GPIO port bit set/reset register,      Address offset: 0x18      */
  __IO uint32_t LCKR;     /*!< GPIO port configuration lock register, Address offset: 0x1C      */
  __IO uint32_t AFR[2];   /*!< GPIO alternate function registers,     Address offset: 0x20-0x24 */
}GPIO_TypeDef;
typedef struct
{
  __IO uint32_t USELESSRN[12];
  __IO uint32_t AHB1ENR;       /*!< RCC AHB1 peripheral clock register,                          Address offset: 0x30 */
  __IO uint32_t AHB2ENR;       /*!< RCC AHB2 peripheral clock register,                          Address offset: 0x34 */
  __IO uint32_t AHB3ENR;       /*!< RCC AHB3 peripheral clock register,                          Address offset: 0x38 */
  uint32_t      RESERVED2;     /*!< Reserved, 0x3C                                                                    */
  __IO uint32_t APB1ENR;       /*!< RCC APB1 peripheral clock enable register,                   Address offset: 0x40 */
  __IO uint32_t APB2ENR;       /*!< RCC APB2 peripheral clock enable register,                   Address offset: 0x44 */

} RCC_TypeDef;

static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate);
static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate);
void uart_print_str(const char *str);


void uart2_write(int ch);
void uart_tx_init(void);
int __io_putchar(int ch)
{
	uart2_write(ch);
	return ch;
}
int main(void)
{
	uart_tx_init();

	while(1){
	uart_print_str("WEC IS GOATED!!\n\r");
	}
	return 0;
}
void uart_print_str(const char *str) {
    while (*str) 
    {              // loop until null terminator
        uart2_write(*str++);    // send each char
    }
}
void uart_tx_init(void)
{
	//******************Configure the UART GPIO pin************************
	/*Enable clock access to GPIOA*/
	RCC->AHB1ENR|=GPIOAEN;
	//Set PA2 to alternate function mode
	GPIOA->MODER|=(1U<<5);
    GPIOA->MODER&=~(1U<<4);
	/* Set PA2 alternate function to UART_TDX (AF07) */
    GPIOA->AFR[0]|=(1U<<8);//AFR is in form of array of 2 bytes. AFR[0] is for the AFRL register , and AFR[1] is for the AFRH register
    GPIOA->AFR[0]|=(1U<<9);
    GPIOA->AFR[0]|=(1U<<10);
    GPIOA->AFR[0]&=~(1U<<11);

	//******************Configure UART Module*****************
	//Enable clock access to UART2
    RCC->APB1ENR|=UART2EN;


	/*Configure UART baudrate*/
    uart_set_baudrate(USART2,APB1_CLK,UART_BAUDRATE);
	// Configure the transfer direction
    USART2->CR1|=CR1_TE;// assigning operator used cuz everything else becomes auto 0 and thats what we want . also default stuff needed so we dont have to congfigure parity or start or stop bit

	  //Enable UART module
    USART2->CR1|=CR1_UE;

}

void uart2_write(int ch)
{
	//Make sure the transmit data register is empty---> USART SR-- status register , bit 7 for
	while(!(USART2->SR & SR_TXE)){}//if both are not same then infinite loop . only when both are same the loop will not be accessed
	//Write to transmit data register
	USART2->DR=(ch & 0xFF);
}





static void uart_set_baudrate(USART_TypeDef *USARTx,uint32_t PeriphClk, uint32_t BaudRate)
{
	USARTx->BRR=compute_uart_bd(PeriphClk,BaudRate);
}

static uint16_t compute_uart_bd(uint32_t PeriphClk,uint32_t BaudRate)
{
	return((PeriphClk+(BaudRate/2U))/BaudRate);//equation for the baudrate value
}
