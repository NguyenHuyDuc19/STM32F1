#include "gpio.h"


void Init_GPIO(GPIO_TypeDef *port, uint8_t pin, uint8_t mode, uint8_t config)
{
	volatile uint32_t *CR;
	if(pin <= 7)
	{
		CR = &(port->CRL);
	}
	else
	{
		CR = &(port->CRH);
		pin -= 8;
	}
	
	//Enable clock 
	if(port == GPIOA)
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	}
	else if(port == GPIOB)
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
	}
	else
	{
		RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	}
	//Mode and configure
	*CR &= ~(1<< (4*pin + 2)); //Reset all bits
	*CR |= (mode << (4*pin)) | (config << (4*pin + 2)); 
}

void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, bool state)
{
	if(state)
	{
		port->ODR |= 1<<pin;
	}
	else
	{
		port->BRR |= 1<<pin;
	}
}

void GPIO_TogglePin(GPIO_TypeDef *port, uint8_t pin)
{
	port->ODR ^= 1<<pin;
}


bool GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin)
{
	if((port->IDR>>pin) & 1)
	{
		return 1;
	}
	else 
	{
		return 0;
	}
}

