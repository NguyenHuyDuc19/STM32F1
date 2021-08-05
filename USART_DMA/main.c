#include "stm32f10x.h"
#include "string.h"

int main() {
	
	//Config USART2
	RCC->APB1ENR |= 1<<17; //Clock USART2 enable
	RCC->APB2ENR |= 1<<2; //Clock PORTA enable
	GPIOA->CRL &= ~(1<<8 | 1<<9 | 1<<10 | 1<<11); //Config PA2 as TX
	GPIOA->CRL |= (1<<8 | 1<<9 | 1<<11);
	GPIOA->CRL &= ~(1<<12 | 1<<13 | 1<<14 | 1<<15); //Config PA3 as RX
	GPIOA->CRL |= (1<<15);
	USART2->BRR = 0xEA6;	//Set baud rate 9600
	USART2->CR1 |= 1<<13 | 1<<3; //Enable USART2 & transmitter
	
	char string[] = "Hello anh Duc";
	uint8_t len = strlen(string);
	
	//Config DMA
	RCC->AHBENR |= 1<<0; //enable clock DMA1 
	USART2->CR3 |= 1<<7; //enable DMA transmit
	DMA1_Channel7->CNDTR = len; //set counter
	//enable Memory increment mode & Circular mode & Data transfer direction & Transfer complete interrupt 
	DMA1_Channel7->CCR |= (1<<7 | 1<<5 | 1<<4 | 1<<1);
	DMA1_Channel7->CMAR = (uint32_t) string; //Address of memory
	DMA1_Channel7->CPAR = (uint32_t)& USART2->DR; //Address of peripheral
	
	while(1) {
		DMA1_Channel7->CCR |= 1; //enable DMA channel 7
		while(!((DMA1->ISR >> 25) & 1)); //Channel 7 transfer complete
		DMA1->IFCR |= 1<<25; //Channel 7 transfer complete clear 
		DMA1_Channel7->CCR &= ~1; //disable DMA channel 7
	}
	
}
