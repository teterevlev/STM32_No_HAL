#ifndef GPIO_Port_H_
#define GPIO_Port_H_
#include <stm32f411xe.h>

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
};

#endif /* GPIO_Port_H_ */
