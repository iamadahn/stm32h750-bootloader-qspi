#ifndef USART_H_
#define USART_H_

#include "stm32h7xx_hal.h"

void usart_init(UART_HandleTypeDef *handle, USART_TypeDef *self);

#endif

