#include "stm32f10x.h"

void EXTI0_IRQHandler(void);

int main() {
	//Configure GPIO
	RCC->APB2ENR |= 1 | 1<<2;
	AFIO->EXTICR[0] = 0;
	AFIO->EXTICR[1] = 0;
	AFIO->EXTICR[2] = 0;
	AFIO->EXTICR[3] = 0;
	GPIOA->CRL &= ~(1 | 1<<1 | 1<<2 | 1<<3); //config pin A0 as input with pull up/pull down
	GPIOA->CRL |= (1<<3);
	GPIOA->CRL &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7); //config pin A1 as output push pull
	GPIOA->CRL |= (1<<4 | 1<<5 );
	
	//Configure Interrupt 
	EXTI->IMR |= 1; //Configure the mask bits
	EXTI->RTSR |= 1; //Configure the Trigger Selection bits
	NVIC_EnableIRQ(EXTI0_IRQn); //Configure the enable and mask bits that control the NVIC IRQ channel
	while(1){
	}
}

void EXTI0_IRQHandler(void) {
	if(GPIOA->IDR & 1) {
		GPIOA->ODR |= 1<<1;
		EXTI->PR |= 1; //Clear interrupt pending bit - Interrupt service routine is done one time
	} 
	else {
		GPIOA->ODR &= ~(1<<1);
	}
}
