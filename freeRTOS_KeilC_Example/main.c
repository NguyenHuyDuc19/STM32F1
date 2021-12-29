#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core

#include "stm32f10x.h"                  // Device header

void my_Task(void *arg)
{
	
	for(;;)
	{
		GPIOC->ODR ^= 1<<13; //Toggle led
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}


int main(void)
{
	
	//GPIO config
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //Port C clock enabled
	GPIOC->CRH &= ~(1<<22);
	GPIOC->CRH = (1<<21) | (1<<20); //Config PC13 as general purpose output push-pull
	
	xTaskCreate(my_Task, "myTask", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();
}

