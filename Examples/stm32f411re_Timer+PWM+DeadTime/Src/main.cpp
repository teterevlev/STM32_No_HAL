#include <stm32f411xe.h>
#include <tPeriphery.h>
uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}

GPIO_Port PortA(GPIOA, RCC_AHB1ENR_GPIOAEN), PortB(GPIOB, RCC_AHB1ENR_GPIOBEN);
int main ( void ){
	AFs.a;

	// clock
	RCC->CR = RCC_CR_PLLON;	// enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | (72 << RCC_PLLCFGR_PLLN_Pos);	// config pll
	FLASH->ACR = (2 << FLASH_ACR_LATENCY_Pos); // setup latency
	RCC->CFGR = (2 << RCC_CFGR_SW_Pos);		// drive by pll
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL

	// clock periphery
	RCC->APB2ENR = RCC_APB2ENR_TIM1EN;
	PortA.clock();
	PortB.clock();

	// gpio
	PortA.output(5);
	PortA.output(9, AF);
	PortB.output(0, AF);


	GPIOA->AFR[0] = 0;
	GPIOA->AFR[1] = (1 << GPIO_AFRH_AFSEL9_Pos);
	GPIOB->AFR[0] = (1 << GPIO_AFRL_AFSEL0_Pos);
	GPIOB->AFR[1] = 0;

	// timer
	TIM1->CCMR1 = (6 << TIM_CCMR1_OC2M_Pos) | TIM_CCMR1_OC2PE; //26624;
	TIM1->CCER = TIM_CCER_CC1E | TIM_CCER_CC1NE | TIM_CCER_CC2E | TIM_CCER_CC2NE ;//85;
	TIM1->PSC = 1;
	TIM1->ARR = 601;
	TIM1->CCR2 = 301;
	TIM1->BDTR = 165 | TIM_BDTR_BKP | TIM_BDTR_MOE;//41125;
	TIM1->DMAR = 1;
	TIM1->CR1 = TIM_CR1_CEN; // enable
	for ( ;; ) {
		GPIOA->ODR ^= GPIO_ODR_OD5;
		delay(100);
	}
}

