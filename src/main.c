#include "bsp.h"
#include "usart.h"
#include "stm32h7xx_hal.h"

int main(void)
{
    bsp_init();

    UART_HandleTypeDef serial = {0};
    usart_init(&serial, USART1);

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}

