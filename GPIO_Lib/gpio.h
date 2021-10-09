#ifndef INC_GPIO_H_
#define INC_GPIO_H_


#include "stm32f10x.h"                  
#include <stdint.h>
#include <stdbool.h>



//Mode
#define IN				0
#define OUT_LOW		1
#define	OUT_MD		2
#define	OUT_HIGH	3

//Config
#define	AM				0
#define	FI				1
#define	PU_PD			2	

#define	GP_PP			0
#define	GP_OD			1
#define	AF_PP			2
#define	AF_OD			3


void GPIO_WritePin(GPIO_TypeDef *port, uint8_t pin, bool state);
void GPIO_TogglePin(GPIO_TypeDef *port, uint8_t pin);
bool GPIO_ReadPin(GPIO_TypeDef *port, uint8_t pin);


void Init_GPIO(GPIO_TypeDef *port, uint8_t pin, uint8_t mode, uint8_t config);

#endif

