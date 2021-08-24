#include "stm32f10x.h"

void SysTick_Handler(void);
void delayMs(int ms);

int systick;

int main()
{
	SysTick_Config(SystemCoreClock /1000); //Configure SysTick to generate an interrupt every millisecond
	
	RCC->APB2ENR |= 1<<2; //Port A clock enabled
	GPIOA->CRL &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7); //Configure pin A1 as general purpose output push - pull
	GPIOA->CRL |= (1<<4 | 1<<5);
	
	while(1)
	{
		GPIOA->ODR ^= 1<<1;
		delayMs(1000);
	}
}

void SysTick_Handler(void)
{
	systick++;
}

void delayMs(int ms)
{
	systick = 0;
	while(systick < ms);
}
