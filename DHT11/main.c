#include "stm32f10x.h" 
#include "systick_delay.h"
#include "gpio.h"


void DHT_Run(void);
uint8_t DHT_Read(void);
uint8_t DHT_CheckResponse(void);

float TEM, HUM;


int main(void)
{
	while(1)
	{
		DHT_Run();
		delayMs(1000);
	}
}

uint8_t DHT_Read(void)
{
	uint8_t var;
	for(int i = 0; i < 8; i++)
	{
		while(!GPIO_ReadPin(GPIOB, 9)); //Wait for PB9 high (end of 50ms signal)
		delayUs(40);
		if(GPIO_ReadPin(GPIOB, 9))
		{
			var = (var << 1)|(0x01); //Write 1 to var
			while(GPIO_ReadPin(GPIOB, 9)); //Wait for PB9 low (end of 70ms signal)
		}
		else
		{
			var = (var << 1); //Write 0 to var
		}
	}
	return var;
}

uint8_t DHT_CheckResponse(void)
{
	while(GPIO_ReadPin(GPIOB, 9)); //Wait till the end of 10ms high signal
	delayUs(90);
	if(!GPIO_ReadPin(GPIOB, 9))
	{
		return 0; 
	}
	else
	{
		while(GPIO_ReadPin(GPIOB, 9));//Wait till the end 80ms high signal
		return 1;
	}
}

void DHT_Run(void)
{
	uint8_t HUM1 = 0, HUM2 = 0, TEM1 = 0, TEM2 = 0, SUM = 0;
	//Config GPIO
	GPIO_Init(GPIOB, 9, OUT_HIGH, GP_OD);
	
	//Transmit start signal
	GPIO_WritePin(GPIOB, 9, 0);
	delayMs(20);
	GPIO_WritePin(GPIOB, 9, 1);
	
	if(DHT_CheckResponse())
	{
		HUM1 = DHT_Read();
		HUM2 = DHT_Read();
	
		TEM1 = DHT_Read();
		TEM2 = DHT_Read();

		SUM = DHT_Read();
	}
	if(SUM == HUM1 + HUM2 + TEM1 + TEM2)
	{
		HUM = (float)HUM1 +((float)HUM2/10.0);
		TEM = (float)TEM1 + (float)TEM2/10.0;
	}
}
