#include "stm32f10x.h" 

void TIM4_IRQHandler(void);
void delayMs(int ms);


int mytick;

int main()
{
	RCC->APB1ENR |= 1<<2; //TIM4 clock enabled
	
	
	TIM4->PSC = 0; //Set prescaler value
	TIM4->ARR = 72; //Set auto-reload value
	TIM4->CR1 |= 1<<2; //Only counter overflow/underflow generates an update interrupt
	TIM4->DIER |= 1; //Update interrupt enable
	TIM4->EGR |= 1; //Update generation
	
	RCC->APB2ENR |= 1<<2; //Port A clock enabled
	GPIOA->CRL &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7); //Configure pin A1 as general purpose output push - pull
	GPIOA->CRL |= (1<<4 | 1<<5);
	
	NVIC_EnableIRQ(TIM4_IRQn);
	while(1)
	{
		GPIOA->ODR ^= 1<<1;
		delayMs(1000);
	}
}

void TIM4_IRQHandler(void)
{
	mytick++;
	TIM4->SR &= ~(1); //Clear update interrupt flag
	__dsb(0);  // force a wait for the internal write buffer to empty
	//https://developer.arm.com/documentation/ka003795/latest
}

void delayMs(int ms)
{
	TIM4->CR1 |= 1; //Enable timer counter
	mytick = 0;
	while(mytick < (ms*1000));
	TIM4->CR1 &= ~1; //Disable timer counter
}