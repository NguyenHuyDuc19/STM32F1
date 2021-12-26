#include "stm32f10x.h"                  
#include "gpio.h"
#include "systick_delay.h"

void I2C_Init(void);
void I2C_Start(void);
void I2C_Address(uint8_t address);
void I2C_Write(uint8_t data);
void I2C_Stop(void);


int main(void)
{
	I2C_Init();
	
	while(1)
	{
		for(int i = 0; i<4; i++)
		{
			I2C_Start();
			I2C_Address(0x4E); //Address of PCF8574 module
			I2C_Write(1<<i); //Turn on led on pin i
			I2C_Stop();
			delayMs(100);
		}
	}
}

void I2C_Init(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN; //I2C1 clock enabled
	
	GPIO_Init(GPIOB, 6, OUT_HIGH, AF_OD); //PB6 - I2C1_SCL
	GPIO_Init(GPIOB, 7, OUT_HIGH, AF_OD); //PB7 - I2C1_SDA

	//Reset the I2C
	I2C1->CR1 |= I2C_CR1_SWRST;
	I2C1->CR1 &= ~I2C_CR1_SWRST;
	
	//Program the peripheral input clock in I2C_CR2 Register in order to generat correct timings
	I2C1->CR2 |= 36; //Clock input 36Mhz (APB1)
	
	/*
	Take the period of the frequency set into CR2 ex: 1/36Mhz =27,78nS
	Now pick a freq you want the I2C to run at (max 100kHz in standard mode) (400kHz max in Fast mode)
	Then if in standard mode get the period of that frequency ex: 1/100kHz = 10uS
	Then divide by 2 to get the Htime because the duty cycle is 50% so high time and low time is the same so we just divide period by 2 to get 5uS
	Then the CCR register = 5us/27,78nS = 180
	so CCR is 180, this is the MAX time the I2C will wait for a rise time and not necessarily the rise time that it will acheive
	*/
	I2C1->CCR |= 180; //Speed to 100kHz
	
	I2C1->TRISE |= 37; // The formula in datasheet: 1000ns / (CR2 period = 1/36Mhz) = TRISE + 1

	I2C1->CR1 |= I2C_CR1_PE; //Peripheral enable
	
}

void I2C_Start(void)
{
	I2C1->CR1 |= I2C_CR1_START; //Generate start condition
	while(!(I2C1->SR1 & I2C_SR1_SB)); //Wait for start condition generated
}

void I2C_Address(uint8_t address)
{
	I2C1->DR = address; //Send address
	while (!(I2C1->SR1 & (I2C_SR1_ADDR)));  //Wait for ADDR bit to set
	uint8_t temp = I2C1->SR1 | I2C1->SR2; //Read SR1 and SR2 to clear bit ADDR
}


void I2C_Write(uint8_t data)
{
	while(!(I2C1->SR1 & I2C_SR1_TXE)); //Wait for byte transfer complete
	I2C1->DR = data; //Wata to write in that address
}


void I2C_Stop(void)
{
	//Wait for data byte transfer succeeded and data register empty
	while(!((I2C1->SR1 & I2C_SR1_BTF) | (I2C1->SR1 & I2C_SR1_TXE)));
	I2C1->CR1 |= I2C_CR1_STOP; //Stop generation
}
