#include "bsp.h"
#include "usart.h"
#include "qspi.h"
#include "stm32h7xx_hal.h"

static UART_HandleTypeDef serial;
static QSPI_HandleTypeDef qspi;

int main(void)
{
    bsp_init();

    usart_init(&serial, USART1);

    qspi_init(&qspi);

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}


extern "C" {
    void SysTick_Handler(void)
    {
        HAL_IncTick();
    }

    void USART1_IRQHandler(void)
    {
        HAL_UART_IRQHandler(&serial);
    }
}
