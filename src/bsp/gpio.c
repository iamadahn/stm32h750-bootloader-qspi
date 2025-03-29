#include "gpio.h"
#include "stm32h7xx_hal.h"

void gpio_init(void)
{
    GPIO_InitTypeDef led_config = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

    led_config.Pin = GPIO_PIN_3;
    led_config.Mode = GPIO_MODE_OUTPUT_PP;
    led_config.Pull = GPIO_NOPULL;
    led_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &led_config);
}


