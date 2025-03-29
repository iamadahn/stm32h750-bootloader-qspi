#include "bsp.h"
#include "usart.h"
#include "stm32h7xx_hal.h"

static UART_HandleTypeDef serial;

int main(void)
{
    bsp_init();

    usart_init(&serial, USART1);

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}

void SysTick_Handler(void)
{
    HAL_IncTick();
}

void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&serial);
}
