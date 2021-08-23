#include "stm32f10x.h"
void gpio_config(void);
void usart_config(void);
void USART3_IRQHandler();
void dma_transceiver(void);
unsigned char chat;

int main() {
	gpio_config();
	usart_config();
	while(1) {
	}
}

void USART3_IRQHandler() {
	if((USART3->SR & (1<<5))) { //check read data register not empty
		
		dma_transceiver();
		
		while(!((USART3->SR >> 6) & 1));
	}
}
void dma_transceiver(void) {
		RCC->AHBENR |= 1; //DMA1 clock enable
	
		DMA1_Channel3->CCR |= 1<<5 | 1<<1; //Circular mode enabled
		DMA1_Channel3->CNDTR = 1; //Number of data to transfer
		DMA1_Channel3->CMAR = (uint32_t)&chat; //Address of memory
		DMA1_Channel3->CPAR = (uint32_t)&(USART3->DR); //Address of peripheral
		DMA1_Channel3->CCR |= 1; //Channel enable
	
		DMA1_Channel2->CCR |= (1<<5 | 1<<4 ); //Circular mode enabled & Read from memory
		DMA1_Channel2->CNDTR = 1; //Number of data transfer
		DMA1_Channel2->CMAR = (uint32_t)&chat; //Address of memory
		DMA1_Channel2->CPAR = (uint32_t)&(USART3->DR); //Address of peripheral
		DMA1_Channel2->CCR |= 1; //Channel enable
}

void gpio_config(void){
	RCC->APB2ENR |= 1<<3; //GPIOB clock enabled
	//USART3_TX PB10
	GPIOB->CRH &= ~(1<<8 | 1<<9 | 1<<10 | 1<<11); //Configure pin PB10 as alternate function output push pull
	GPIOB->CRH |= (1<<8 | 1<<9 | 1<<11);
	//USART3_RX PB11
	GPIOB->CRH &= ~(1<<12 | 1<<13 | 1<<14  | 1<<15); //Configure pin PB11 as input pull up
	GPIOB->CRH |= 1<<15;
}

void usart_config(void) {
	RCC->APB1ENR |= 1<<18; //USART3 clock enabled
	USART3->BRR = 0xEA6; // Baud rate is 9600
	USART3->CR1 |= (1<<2 | 1<<3 | 1<<13); //Receiver enable & Transmitter enable & USART enable
	USART3->CR3 |= (1<<7 | 1<<6); 
	USART3->CR1 |= (1<<5 | 1<<6 | 1<<7); //RXNE interrupt enable & Transmission complete interrupt enable & TXE interrupt enable
	NVIC_EnableIRQ(USART3_IRQn);
}
