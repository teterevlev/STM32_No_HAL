#include <stm32f103xb.h>
uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}
int main ( void ){

	RCC->CR |= RCC_CR_HSEON; // hse enable
	while((RCC->CR & RCC_CR_HSERDY) == 0); // wait for HSE ready
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1 | RCC_CFGR_PPRE2_DIV1 | RCC_CFGR_PPRE1_DIV1;
	RCC->CFGR &= ~((RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL)); // pre-clean
	RCC->CFGR |= RCC_CFGR_PLLXTPRE; // feed PLL from HSE (8 MHz)
	RCC->CFGR |= RCC_CFGR_PLLMULL9; // multiply by 9 (8*9=72 MHz)
	RCC->CR |= RCC_CR_PLLON; // run PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	RCC->CFGR &= ~RCC_CFGR_SW; // clean SW0, SW1
	RCC->CFGR |= RCC_CFGR_SW_PLL; // feed by PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;



	GPIOC->CRH &= !(GPIO_CRH_CNF13_0 | GPIO_CRH_CNF13_1);
	GPIOC->CRH |= GPIO_CRH_MODE13_1;


	for ( ;; ) {
		GPIOC->ODR ^= GPIO_ODR_ODR13;
		delay(500);
	}
}


