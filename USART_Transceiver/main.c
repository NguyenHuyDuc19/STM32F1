#include "stm32f10x.h"     

void USART1_IRQHandler(void);

int main(){
	
	//Enable the USART clock
	RCC->APB2ENR |= 1<<14;
	
	//Enable the USART1 
	USART1->CR1 |= 1<<13;
	
	//Enable port A
	RCC->APB2ENR |= 1<<2;
	
	//Config pin A9 alternate function push pull  - TX
	GPIOA->CRH &= ~(1<<7 | 1<<6 | 1<<5 | 1<<4);
	GPIOA->CRH |= (1<<4 | 1<<5 | 1<<7);
	
	//Config pin A10 input pull up - pull down
	GPIOA->CRH &= ~(1<<8 | 1<<9 | 1<<10 | 1<<11);
	GPIOA->CRH |= (1<<11);
	
	//Select the desired baud rate - 9600
	USART1->BRR = 0x1D4C;
	
	//Set the RE & TE bit USART_CR1
	USART1->CR1 |= 1<<2 | 1<<3;
	
	//Enable TXE interrupt & Transmission complete interrupt
	USART1->CR1 |= 1<<6 | 1<<7;
	
	//Enable interrupt request
	NVIC_EnableIRQ(USART1_IRQn);
	
	while(1){
	}
	
}


void USART1_IRQHandler(void){
	
	if(USART1->SR & (1<<5)) { //check read data register not empty 
			char tmp = USART1->DR;
			USART1->DR = tmp;
			while(!((USART1->SR>>6) & 1));
	}		
	
	if(USART2->SR &(1<<7)){ //check transmit data register empty 
		
	}
}
