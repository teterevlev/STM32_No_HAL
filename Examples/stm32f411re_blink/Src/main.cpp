/* The reset and clock control module */
#include <stm32f411xe.h>

uint32_t clocksPerSecond = 7200000;
uint32_t clocksPerMillie = clocksPerSecond/1000;
void delay (uint32_t millis){
	volatile uint32_t count = clocksPerMillie*millis;

	while ( count-- );
}

enum{INPUT, OUTPUT, AF, ANALOG};
enum{PUSHPULL, OPENDRAIN};
enum{LOW, MEDIUM, FAST, VERYFAST};
enum {OFF, ON};
enum {PULLUP=1, PULLDOWN};
class GPIO_Port{
	private:
		GPIO_TypeDef * port;
		uint32_t AHB1ENR_bit;
	public:
		GPIO_Port(GPIO_TypeDef * port, uint32_t AHB1ENR_bit):
			port(port),
			AHB1ENR_bit(AHB1ENR_bit)
		{

		}
		void clock(){
			RCC->AHB1ENR |= AHB1ENR_bit;
		}
		void clock(uint8_t off){
			if (off){
				RCC->AHB1ENR |= AHB1ENR_bit;
			}else{
				RCC->AHB1ENR &= ~AHB1ENR_bit;
			}
		}
		void output(uint8_t pin, uint8_t mode = OUTPUT, uint8_t type = PUSHPULL, uint8_t speed = VERYFAST){
			uint8_t		pin_x2		=	pin * 2;
			uint32_t	mask		=	1 << pin;
			uint32_t	mask_x2		=	0b11 << pin_x2;

			port->MODER				&=	mask_x2;				// clear mode
			port->MODER				|=	mode << pin_x2;			// mode

			port->OTYPER			&=	mask;					// clear type
			port->OTYPER			|=	type << pin;			// type

			port->OSPEEDR			&=	mask_x2;				// clear speed
			port->OSPEEDR			|=	speed << pin_x2;		// speed
		}
		void input(uint8_t pin, uint8_t pullup = OUTPUT){
			uint8_t		pin_x2		=	pin * 2;
			uint32_t	mask		=	1 << pin;
			uint32_t	mask_x2		=	0b11 << pin_x2;

			port->MODER			&=	mask;					// mode = input

			port->PUPDR			&=	mask;					// pullup
			port->PUPDR			|=	mask_x2 << pin_x2;		// pullup
		}
} PortA(GPIOA, RCC_AHB1ENR_GPIOAEN), PortB(GPIOB, RCC_AHB1ENR_GPIOBEN);

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

/* THE END */
