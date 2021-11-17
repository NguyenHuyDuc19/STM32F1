#include "stm32f10x.h"

void Flash_unlock(void);
void Flash_erase(volatile uint32_t Startaddr);
void Flash_write(volatile uint32_t Startaddr, uint8_t* BufferWrite, uint32_t Length);
void Flash_read(volatile uint32_t Startaddr, uint8_t* BufferRead, uint32_t Length);

uint8_t  Buffer[8] = {0, 1, 2, 3, 4, 5, 6, 7}; 

int main(void)
{
	uint8_t DataBuffer[8] = {0};
	Flash_erase((uint32_t )0x08001000);
	Flash_write((uint32_t)0x08001000, Buffer, 8);
	Flash_read((uint32_t)0x08001000, DataBuffer, 8);
	while(1)
	{
	}
}

void Flash_unlock(void)
{
	FLASH->KEYR = 0x45670123; //Write the key value
	FLASH->KEYR = 0xCDEF89AB; //Write the key value
}

void Flash_erase(volatile uint32_t Startaddr)
{
	while((FLASH->SR & FLASH_SR_BSY)); //Check BSY bit
	if((FLASH->CR & FLASH_CR_LOCK))
	{
		Flash_unlock();
	}
	
	FLASH->CR |= FLASH_CR_PER;
	FLASH->AR = Startaddr;
	FLASH->CR |= FLASH_CR_STRT;
	
	while((FLASH->SR & FLASH_SR_BSY)); //Check BSY bit
	
	FLASH->CR &= ~FLASH_CR_PER;
	FLASH->CR &= ~FLASH_CR_STRT;
}

void Flash_write(volatile uint32_t Startaddr, uint8_t* BufferWrite, uint32_t Length)
{
	uint32_t count = 0;
	while((FLASH->SR & FLASH_SR_BSY)); //Check BSY bit
	if((FLASH->CR & FLASH_CR_LOCK))
	{
		Flash_unlock();
	}
	
	FLASH->CR |= FLASH_CR_PG;
	
	for(count = 0; count < Length/2; count++)
	{
		*(uint16_t*)(Startaddr + count*2) = *(uint16_t*)(BufferWrite + count*2);
	}
	while((FLASH->SR & FLASH_SR_BSY)); //Check BSY bit
	FLASH->CR &= ~FLASH_CR_PG;
}


void Flash_read(volatile uint32_t Startaddr, uint8_t* BufferRead, uint32_t Length)
{
	uint32_t count = 0;
		for(count = 0; count < Length; count++)
	{
		*(uint8_t*)(BufferRead + count) = *(uint8_t*)(Startaddr + count);
	}
}