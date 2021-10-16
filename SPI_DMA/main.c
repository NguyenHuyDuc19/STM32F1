#include "stm32f10x.h"                  // Device header
#include "gpio.h"
#include "systick_delay.h"                  
#include "string.h"

char buff[] = "Hello Duc";

int main(void)
{
	int len = strlen(buff);
	
	//GPIO Configure
	GPIO_Init(GPIOA, 4, OUT_HIGH, GP_PP); 	//PA4 - SPI1_NSS
	GPIO_Init(GPIOA, 5, OUT_HIGH, AF_PP); 	//PA5 - SPI1_SCK
	GPIO_Init(GPIOA, 6, IN, PU_PD); 				//PA6 - SPI1_MISO
	GPIO_Init(GPIOA, 7, OUT_HIGH, AF_PP);		//PA7 - SPI1_MOSI
	GPIO_WritePin(GPIOA, 4, 1); 
  
	delayMs(5000);
	
	//SPI1 Configure
	RCC->APB2ENR |= RCC_APB2ENR_SPI1EN; //SPI1 clock enable
	SPI1->CR1 |= SPI_CR1_BR_2	//Baund rate: fPCLK/32
						| SPI_CR1_SSM //Software slave management
						| SPI_CR1_SSI //Internal slave select
						| SPI_CR1_MSTR //Master configuration
						| SPI_CR1_SPE; //Peripheral enabled
	SPI1->CR2 |= SPI_CR2_TXDMAEN ; //Tx buffer DMA enable				
	
	GPIO_WritePin(GPIOA, 4, 0); //Start communication
	
	//DMA1 Configure
	RCC->AHBENR |= RCC_AHBENR_DMA1EN; //DMA1 clock enable
	DMA1_Channel3->CCR |= DMA_CCR3_MINC //Memory increment mode
										 | DMA_CCR3_DIR; //Read from memory
	DMA1_Channel3->CNDTR = len + 2; //Number of data to transfer
	DMA1_Channel3->CMAR = (uint32_t)(buff); //Address of memory
	DMA1_Channel3->CPAR = (uint32_t)&(SPI1->DR); //Address of peripheral
	DMA1_Channel3->CCR |= DMA_CCR3_EN; //DMA1 channel3 enabled
	
	while(!(DMA1->ISR & DMA_ISR_TCIF3)); //A transfer complete event occurred on channel 3

	GPIO_WritePin(GPIOA, 4, 1); //Stop communication
	
	while(1)
	{
		
	}
	
}
