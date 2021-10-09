#include "stm32f10x.h"


void ADC_Configuration(void);
void RTC_Configuration(void);
void Enter_StandbyMode(void);
void GPIO_Configuration(void);
int val =0;
extern void delay(void);


int main(void)
{
		/* Enable PWR and BKP clock */
		RCC-> APB1ENR |= 0x18000000U;
	
		/* Allow access to BKP Domain */
		PWR->CR |= 0x0100U;				

		PWR->CR |= PWR_CR_CWUF;
		PWR->CR |= PWR_CR_CSBF;
		GPIO_Configuration();
		/* Configure RTC clock source and prescaler */
		RTC_Configuration();
	
		/* Wait till RTC Second event occurs */
		RTC->CRL &= ~(0x0001U);
 
		while((RTC->CRL & 0x0001U) == 0){}

    /* Set the RTC Alarm after x seconds ( = x*1s) */
		RTC->CRL |= 0x0010U;										// Enter the Configuration Mode
		RTC->ALRH = 0x0000U;										// Set PRLH value
		RTC->ALRL = RTC->CNTL + 0x0005U;				// Set PRLL value = x
		RTC->CRL &= ~(0x0010U);								  // Exit the Configuration Mode	
		while (((RTC->CRL >> 5) & 0x01) != 1){};

		ADC_Configuration();
		
		while(RTC->CNTL != 0x0003){
			val=ADC1->DR;
			if(val>3000){
				while(1){
				GPIOA->ODR |= 1;
				}
				} else{
					GPIOA->ODR &= ~1;
			}
		}
			
		Enter_StandbyMode();
}
		

void RTC_Configuration(void){

if((PWR->CSR & 0x00000002U) != 0U)
  {/* System resumed from STANDBY mode */
    /* Clear StandBy flag */
    PWR->CSR &= ~(0x00000002U);
    /* Wait for RTC APB registers synchronisation */
    RTC->CRL &= ~(0x0008U);
       while ((RTC->CRL & 0x0008U) == 0U){}    
  }
  else
		{
  /* Check if the StandBy flag is set */

 /* StandBy flag is not set */

		/* Reset Backup Domain */
		//enable reset backup
		RCC->BDCR |=(1<<16);
		//disable reset backup
		RCC->BDCR &=~(1<<16);
		
		
		/* Enable LSE OSC */
		//Reset LSEON and LSEBYP bit before configuring the LSE
		// Reset LSEON bit
		RCC->CSR &= ~(1<<0);

		/* Set LSEON bit */
		RCC->CSR |= 0x01;

		/* Wait till LSE is ready */
		while (((RCC->CSR >> 1) & 0x01) != 1){};

		/* Enable the RTC Clock */
		RCC->BDCR |= 0x0200;	

		// RTCEN: RTC clock enable
		RCC->BDCR |= 0x8000;
			
		/* Wait for RTC APB registers synchronisation */
		//clear bit RSF
		RTC->CRL &=~(1<<3);
		//wait bit RSF = 1
		while (((RTC->CRL >> 3) & 0x01) != 1){};
				
			
		// Poll RTOFF
		while (((RTC->CRL >> 5) & 0x01) != 1);
		//Set the CNF bit to enter configuration mode 
		RTC->CRL |= 0x10;
		
		//Write to onr or more RTC registers
		RTC->PRLH = 0x0000;
		RTC->PRLL = 0x9C3F;
		
				
		RTC->CRH |= (1<<1);
		
		/*Clear the CNF bit to exit configuration mode */
		RTC->CRL &= ~0x10;
		/* Poll RTOFF, wait until its value goes to '1' */
		while (((RTC->CRL >> 5) & 0x01) != 1){};
				
  //}
}
}
	
void GPIO_Configuration(void){
		RCC->APB2ENR |= 1<<2;
		GPIOA->CRL &= 0xFFFFFFF0;
		GPIOA->CRL |= 0x3;
}

void Enter_StandbyMode(void){

	/* Request to enter STANDBY mode (Wake Up flag is cleared) */	
	
  PWR->CR |= 0x0004U;								// Clear Wake-up flag
	SCB->SCR |= 0x04U;								// Set SLEEPDEEP bit of Cortex System Control Register
  PWR->CR |= 0x0002U;								// Select STANDBY mode
	/* Request Wait For Interrupt */
  __WFI();
}





void ADC_Configuration(void){

	RCC->CFGR |= 1<<16;//enable ADC prescale div 6 
	
	RCC->APB2ENR |= 0x200;//enable clock ADC1
	
	//Configure pin A5 as input push pull
	GPIOA->CRL |= GPIO_CRL_CNF5_1;
	GPIOA->CRL &= ~(GPIO_CRL_CNF5_0);
	//Mode5 = 00 => input mode
	
	//configuration sample rate on pin A5 is 111- 239.5 cycles 
	ADC1->SMPR2 |= ADC_SMPR2_SMP5_2 | ADC_SMPR2_SMP5_0 | ADC_SMPR2_SMP5_1;
	

	ADC1->SQR3 |= ADC_SQR3_SQ1_0 | ADC_SQR3_SQ1_2;//set up SQ2 to be channel 5 
	
	//enable the ADC for the first time and set it to continous mode
	ADC1->CR2 |= ADC_CR2_ADON | ADC_CR2_CONT;
	
	delay();
	
	//turn on ADC for the second time to actually turn it on
	ADC1->CR2 |= ADC_CR2_ADON;
	
	delay();
	//run the calibration
	ADC1->CR2 |= ADC_CR2_CAL;
}
