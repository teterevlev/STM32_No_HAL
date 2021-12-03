#include <stm32f411xe.h>
uint32_t clocksPerSecond = 72000000;
uint32_t clocksPerMillie = clocksPerSecond/1000;

const uint16_t BAUD_MANTISSA = 19;
const uint16_t BAUD_FRACTION = 8;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}
void transmit(USART_TypeDef * usart	){
	usart->DR = 'a';
	while(!(usart->SR & (1<<USART_SR_TC_Pos)));
	usart->DR = 'b';
	while(!(usart->SR & (1<<USART_SR_TC_Pos)));
}

int main ( void ){


	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | (72 << RCC_PLLCFGR_PLLN_Pos) | (4 << RCC_PLLCFGR_PLLQ_Pos);
	RCC->CR = RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	FLASH->ACR = (2 << FLASH_ACR_LATENCY_Pos);
	RCC->CFGR = (4 << RCC_CFGR_PPRE1_Pos);
	RCC->CFGR |= (2 << RCC_CFGR_SW_Pos);
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;


	GPIOA->MODER |= (1 << GPIO_MODER_MODER5_Pos) | (2 << GPIO_MODER_MODER3_Pos) | (2 << GPIO_MODER_MODER2_Pos);
	GPIOA->OTYPER |= 0 << GPIO_OTYPER_OT5_Pos;
	GPIOA->OSPEEDR |= (3 << GPIO_OSPEEDR_OSPEED5_Pos) | (3 << GPIO_OSPEEDR_OSPEED3_Pos) | (3 << GPIO_OSPEEDR_OSPEED2_Pos);

	GPIOA->AFR[0] = (7<<GPIO_AFRL_AFSEL2_Pos) | (7<<GPIO_AFRL_AFSEL3_Pos);


	USART2->CR1 = USART_CR1_UE | USART_CR1_TE | USART_CR1_RE;
	USART2->BRR = BAUD_FRACTION | (BAUD_MANTISSA << USART_BRR_DIV_Mantissa_Pos);
	for ( ;; ) {
		GPIOA->ODR ^= GPIO_ODR_OD5;
		transmit(USART2);
		delay(10);
	}
}

