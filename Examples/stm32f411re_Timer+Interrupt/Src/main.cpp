#include <stm32f411xe.h>
uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
uint32_t events = 0;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}
int main ( void ){
	// clock
	RCC->CR = RCC_CR_PLLON;	// enable PLL
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | (72 << RCC_PLLCFGR_PLLN_Pos);	// config pll
	FLASH->ACR = (2 << FLASH_ACR_LATENCY_Pos); // setup latency
	RCC->CFGR = (2 << RCC_CFGR_SW_Pos);		// drive by pll
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL

	// clock periphery
	RCC->APB2ENR = RCC_APB2ENR_TIM1EN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOBEN;

	// gpio
	GPIOB->MODER |= (2 << GPIO_MODER_MODER0_Pos);		// alternative function
	GPIOB->OTYPER |= 0 << GPIO_OTYPER_OT0_Pos;			// pushpull
	GPIOB->OSPEEDR |= 3 << GPIO_OSPEEDR_OSPEED0_Pos;	// fast as hell
	GPIOA->MODER |=
			(1 << GPIO_MODER_MODER5_Pos) |				// output
			(2 << GPIO_MODER_MODER9_Pos);				// alternative function
	GPIOA->OTYPER = 0;									// pushpull

	GPIOA->OSPEEDR =
			(3 << GPIO_OSPEEDR_OSPEED5_Pos) |
			(3 << GPIO_OSPEEDR_OSPEED9_Pos);			// fast as hell

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
	TIM1->DIER |= TIM_DIER_UIE;
	TIM1->CR1 = TIM_CR1_CEN; // enable
	NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);


	for ( ;; ) {
		GPIOA->ODR ^= GPIO_ODR_OD5;
		delay((events>>8)%1024);
	}
}

