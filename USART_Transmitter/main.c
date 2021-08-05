/*
			1. Enable the USART by writing the UE bit in USART_CR1 register to 1.
			2. Program the M bit in USART_CR1 to define the word length.
			3. Program the number of stop bits in USART_CR2.
			4. Select DMA enable (DMAT) in USART_CR3 if Multi buffer Communication is to take
			place. Configure the DMA register as explained in multibuffer communication.
			5. Select the desired baud rate using the USART_BRR register.
			6. Set the TE bit in USART_CR1 to send an idle frame as first transmission.
			7. Write the data to send in the USART_DR register (this clears the TXE bit). Repeat this
			for each data to be transmitted in case of single buffer.
			8. After writing the last data into the USART_DR register, wait until TC=1. This indicates
			that the transmission of the last frame is complete. This is required for instance when
			the USART is disabled or enters the Halt mode to avoid corrupting the last
			transmission
*/

#include "stm32f10x.h"                  // Device header


char chat = 'D';

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
	
	
	USART1->CR1 |= USART_CR1_RXNEIE | USART_CR1_TXEIE;
	
	NVIC_EnableIRQ(USART1_IRQn);
	
	while(1){

		}
	
}
void USART1_IRQHandler(void){
	//check if we are here because of RXNE interrupt
	if(USART1->SR & USART_SR_RXNE) //if RX is not empty
	{
		char temp = USART1->DR; //fetch the data received
		USART1->DR = temp; //send it back out
		while (!(USART1->SR & USART_SR_TC));
	}
	
	//check if we are here because of TXEIE interrupt
	if(USART1->SR & USART_SR_TXE){ //if RX is not empty
		
		//handle transmit completion here
		
	}
	
}