#include "stm32f10x.h"

uint16_t val[2];

int main() { 
	//GPIO configure
	RCC->CFGR |= 1<<16; //enable prescaler div 6
	RCC->APB2ENR |= (1<<9 | 1<<2); //enable clock ADC1 & GPIOA
	GPIOA->CRL &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7); //Configure A1 as in put analog mode
	GPIOA->CRL &= ~(1<<8 | 1<<9 | 1<<10 | 1<<11); //Configure A2 as in put analog mode

	//DMA configure
	RCC->AHBENR |= 1; //DMA1 clock enabled
	DMA1_Channel1->CNDTR |= 2; //Number of data to transfer
	ADC1->CR2 |= 1<<8; //DMA mode enabled
	DMA1_Channel1->CPAR = (uint32_t)&(ADC1->DR); //Peripheral address
	DMA1_Channel1->CMAR = (uint32_t)(val); //Memory address
	DMA1_Channel1->CCR |= 1<<5 | 1<<7; //Enable circular mode and Memory increment mode
	DMA1_Channel1->CCR |= 1<<8 | 1<<10; // Peripheral and Memory size is 16 bits
	DMA1_Channel1->CCR |= 1; //Enable channel DMA (the step must be done in the end of configure DMA)
	
	
	//ADC configure
	ADC1->SQR1 |= 1<<20; //2 conversions
	ADC1->SMPR2 |= (1<<6 | 1<<7 | 1<<8); //Set sampling rate 239.5 cycles on pin A2
	ADC1->SMPR2 |= (1<<3 | 1<<4 | 1<<5); //Set sampling rate 239.5 cycles on pin A1
	ADC1->SQR3 |=  1| 1<< 6; //Set channel 1 and channel 2 in sequence register
	ADC1->CR1 |= 1<<8; //Scan mode enabled 
	ADC1->CR2 |= (1 | 1<<1); //AD on first time and continous mode 
	for(int i=0; i<100; i++);
	ADC1->CR2 |= 1; //AD on second times
	ADC1->CR2 |= 1<<2; //Run calibration
	
	
	while(1) {
	}
}

