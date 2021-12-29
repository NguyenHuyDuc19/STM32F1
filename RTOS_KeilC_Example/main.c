#include "FreeRTOSConfig.h"             // ARM.FreeRTOS::RTOS:Config
#include "FreeRTOS.h"                   // ARM.FreeRTOS::RTOS:Core
#include "task.h"                       // ARM.FreeRTOS::RTOS:Core
#include "event_groups.h"               // ARM.FreeRTOS::RTOS:Event Groups
#include "timers.h"                     // ARM.FreeRTOS::RTOS:Timers
#include "queue.h"                      // ARM.FreeRTOS::RTOS:Core


#include "stm32f10x.h"                  // Device header
#include "gpio.h"

void first_Task(void *arg)
{
	
	for(;;)
	{
		GPIOC->ODR ^= 1<<13;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}

void second_Task(void *arg)
{
	
	for(;;)
	{
		GPIOA->ODR ^= 1<<2;
		vTaskDelay(pdMS_TO_TICKS(1000));
	}
}
int main(void)
{
	
	//GPIO config
	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN; //Port C clock enabled
	
	GPIOC->CRH &= ~(1<<22);
	GPIOC->CRH = (1<<21) | (1<<20); //Config PC13 as general purpose output push-pull
	    
	GPIO_Init(GPIOA, 2, OUT_HIGH, GP_PP);
	
	xTaskCreate(first_Task, "firstTask", 128, NULL, 1, NULL);
	xTaskCreate(second_Task, "secondTask", 128, NULL, 1, NULL);
	
	vTaskStartScheduler();
}

