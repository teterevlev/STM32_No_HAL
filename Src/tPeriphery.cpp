/*
 * tPeriphery.cpp
 *
 *  Created on: 7 дек. 2021 г.
 *      Author: odama
 */

#include "tPeriphery.h"

GPIO_Port::GPIO_Port(GPIO_TypeDef * port, uint32_t AHB1ENR_bit):
	port(port),
	AHB1ENR_bit(AHB1ENR_bit) {

	}
void GPIO_Port::clock(){
	RCC->AHB1ENR |= AHB1ENR_bit;
}
void GPIO_Port::clock(uint8_t off){
	if (off){
		RCC->AHB1ENR |= AHB1ENR_bit;
	}else{
		RCC->AHB1ENR &= ~AHB1ENR_bit;
	}
}
void GPIO_Port::output(uint8_t pin, uint8_t mode, uint8_t type, uint8_t speed){
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
void GPIO_Port::input(uint8_t pin, uint8_t pullup){
	uint8_t		pin_x2		=	pin * 2;
	uint32_t	mask		=	1 << pin;
	uint32_t	mask_x2		=	0b11 << pin_x2;

	port->MODER			&=	mask;					// mode = input

	port->PUPDR			&=	mask;					// pullup
	port->PUPDR			|=	mask_x2 << pin_x2;		// pullup
}
