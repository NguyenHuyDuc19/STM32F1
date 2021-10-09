#include "stm32f10x.h"
#include "gpio.h"

int main(void)
{
	// Configure GPIO
	Init_GPIO(GPIOB, 9, OUT_HIGH, AF_PP);
	
	// Configure timer
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN; //Clock TIM4 enabled 
	TIM4->CCER |= TIM_CCER_CC4E; // Capture/Compare 4 output enable
	TIM4->CCMR2 |= TIM_CCMR2_OC4M_2 | TIM_CCMR2_OC4M_1; // PWM mode 1
	TIM4->CCMR2 |= TIM_CCMR2_OC4PE; // Output compare 4 preload enable
	TIM4->CR1 |= TIM_CR1_ARPE; // Auto-reload preload enable
	
	/* 
	PWM freq = Fclk/PSC/ARR 
	PWM Duty = CCR4/ARR
	*/
	
	TIM4->PSC = 72;
	TIM4->ARR = 1000;
	TIM4->CCR4 = 250; // Duty cycle is 25%
	
	
	TIM4->EGR |= TIM_EGR_UG; // Update generation
	TIM4->CR1 |= TIM_CR1_CEN; //TIMER channel enable
	
	while(1)
	{
	}
}


