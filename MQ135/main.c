#include "stm32f10x.h"
#include "gpio.h"
#include "systick_delay.h"
#include "uart.h"
#include "string.h"
#include <math.h>

uint16_t ulVar;
float CO2_PPM;

float get_CO2_PPM(uint16_t ulVar)
{
	return (6826.0/ulVar - (float)5/3)*113.71 - 3.01;
}

int main(void)
{
	RCC->CFGR |= 1<<16; //enable prescaler div 6
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN; //enable clock ADC1 
	GPIO_Init(GPIOA, 1, IN, AM);
	
	
	//ADC configure
	ADC1->SMPR2 |= (1<<3 | 1<<4 | 1<<5); //Set sampling rate 239.5 cycles on pin A1
	ADC1->SQR3 |=  1; //Set channel 1
	ADC1->CR1 |= ADC_CR1_SCAN; //Scan mode enabled 
	ADC1->CR2 |= (ADC_CR2_ADON | ADC_CR2_CONT); //AD on first time and continous mode 
	delayMs(1);
	ADC1->CR2 |= ADC_CR2_ADON; //AD on second times
	ADC1->CR2 |= ADC_CR2_CAL; //Run calibration
	
	USART_Init(USART1, BR_USART1_9600);
	
	while(1)
	{
		ulVar = ADC1->DR;
		CO2_PPM = get_CO2_PPM(ulVar);
		delayMs(2000);
	}
}

