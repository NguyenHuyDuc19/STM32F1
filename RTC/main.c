#include "stm32f10x.h"


int main() {
	//GPIO configure
	RCC->APB2ENR |= 1<<2; //Enable GPIO clock
	GPIOA->CRL &= ~(1 | 1<<1 | 1<<2 | 1<<3); //Configure pin A15 as output push pull
	GPIOA->CRL |= (1 | 1<<1 );
	
	//RTC configure
	RCC->APB1ENR |= (1<<27 | 1<<28); //Power interface clock enable and Backup interface clock enable
	PWR->CR |= 1<<8; //Disable backup domain write protection.
	RCC->CSR |= 1; // LSI (Internal low-speed oscillator) enable
	while(!((RCC->CSR >> 1) & 1)); //Wait LSI ready
	RCC->BDCR |= 1<<9; //LSI oscillator clock used as RTC clock
	RCC->BDCR |= 1<<15; //RTC clock enable
	while((!(RTC->CRL >> 3) & 1)); //Poll the RSF bit
	while((!(RTC->CRL >> 5) & 1)); //Poll the RTOFF bit
	RTC->CRL |= 1<<4; //Set the CNF bit to enter configuration mode
	RTC->PRLL = 39999; //RTC prescaler value 
	RTC->CRL &= ~(1<<4); //Set the CNF bit to enter configuration mode
	while((!(RTC->CRL >> 5) & 1)); //Poll the RTOFF bit
	
	while(1) {
		if(RTC->CNTL %2 == 1) {
			GPIOA->ODR |= 1;
		}
		else {
			GPIOA->ODR &= ~1;
		}
	}
}

	