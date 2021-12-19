#include "stm32f10x.h"

int main(void)
{
	//GPIO config
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //Port C clock enabled
	
	GPIOC->CRH &= ~(1<<22);
	GPIOC->CRH = (1<<21) | (1<<20); //Config PC13 as general purpose output push-pull
	
	while(1)
	{
		GPIOC->ODR ^= 1<<13;
		for(int i=0; i<5000000; i++);
	}
}
