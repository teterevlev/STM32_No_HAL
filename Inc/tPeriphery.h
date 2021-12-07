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
		GPIO_Port(GPIO_TypeDef * port, uint32_t AHB1ENR_bit);
		void clock();
		void clock(uint8_t off);
		void output(uint8_t pin, uint8_t mode = OUTPUT, uint8_t type = PUSHPULL, uint8_t speed = VERYFAST);
		void input(uint8_t pin, uint8_t pullup = OUTPUT);
};

#endif /* GPIO_Port_H_ */
