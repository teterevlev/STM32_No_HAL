#include <stm32f411xe.h>
uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}
int main ( void ){


	RCC->CR = RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | (72 << RCC_PLLCFGR_PLLN_Pos);
	FLASH->ACR = (2 << FLASH_ACR_LATENCY_Pos);
	RCC->CFGR = (2 << RCC_CFGR_SW_Pos);
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;


	GPIOA->MODER |= 1 << GPIO_MODER_MODER5_Pos;
	GPIOA->OTYPER |= 0 << GPIO_OTYPER_OT5_Pos;
	GPIOA->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED5_Pos;

	SysTick_Config(144000);
	for ( ;; ) {
		//GPIOA->ODR ^= GPIO_ODR_OD5;
		delay(100);
	}
}
extern "C" void SysTick_Handler(void){
	GPIOA->ODR ^= GPIO_ODR_OD5;
}
