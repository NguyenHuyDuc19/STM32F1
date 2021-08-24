#include "stm32f10x.h"

int checkLongClick();
int checkDoubleClick();

int  lcF;
int  dcF;

void gpio_config();

void blinkLed();
void turn_onLed();
void turn_offLed();


int main() 
{	
	gpio_config();
	
	while(1)
	{
		while(GPIOA->IDR & 1) 
		{
			if(checkLongClick())
			{
				dcF = 0;
				lcF = 1;
			}
			else if(checkDoubleClick())
			{
				dcF = 1;
				lcF = 0;
			}
			if(lcF)
			{
				turn_onLed();
			}
			else 
			{
				blinkLed();
			}
		}
	}
}

int checkLongClick() 
{
	int i = 500000;
	while (i)
	{
		if((GPIOA->IDR & 1) == 0) //button not released
		{
			return 0;
		}
		i--;
	}
	return 1;
}


int checkDoubleClick()
{
	for(int i=0; i<1000000; i++);
	int i = 1000000;
	while (i) 
	{
		if(GPIOA->IDR & 1) //push button second time
		{
			return 1;
		}
		i--;
	}	
	return 0;
}



void blinkLed()
{
	while(dcF)
	{
		GPIOA->ODR ^= 1<<1;
		for(int i=0; i<100000; i++)
		{
			if(GPIOA->IDR & 1)
			{
				if(checkLongClick())
				{
					dcF = 0;
					lcF = 1;
				}
				else if(checkDoubleClick())
				{
					dcF = 1;
					lcF = 0;
				}
				if(lcF)
				{
					turn_onLed();
				}
				else 
				{
					blinkLed();
				}
			}
		}
	}
}

void turn_onLed()
{
		GPIOA->ODR |= 1<<1;
}


void gpio_config()
{
	RCC->APB2ENR |= 1<<2; //Port A clock enabled
	
	GPIOA->CRL &= ~(1 | 1<<1 | 1<<2 | 1<<3); //Configure pin A0 as input pull up/pull down
	GPIOA->CRL |= 1<<3;
	
	GPIOA->CRL &= ~(1<<4 | 1<<5 | 1<<6 | 1<<7); //Configure pin A1 as general purpose output push - pull
	GPIOA->CRL |= (1<<4 | 1<<5);
}
