#include <stm32f411xe.h>
#include "tPeriphery.h"

uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}

GPIO_Port PortA(GPIOA, RCC_AHB1ENR_GPIOAEN), PortB(GPIOB, RCC_AHB1ENR_GPIOBEN);

int main ( void ){


	RCC->CR = RCC_CR_PLLON;
	while((RCC->CR & RCC_CR_PLLRDY) == 0); // wait for PLL
	RCC->PLLCFGR = (8 << RCC_PLLCFGR_PLLM_Pos) | (72 << RCC_PLLCFGR_PLLN_Pos);
	FLASH->ACR = (2 << FLASH_ACR_LATENCY_Pos);
	RCC->CFGR = (2 << RCC_CFGR_SW_Pos);
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1); // wait for switch to PLL


	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN;
	PortA.clock();
	PortA.output(5);



	for ( ;; ) {
		GPIOA->ODR ^= GPIO_ODR_OD5;
		delay(100);
	}
}

