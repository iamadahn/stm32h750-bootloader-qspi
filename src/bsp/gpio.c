#include "gpio.h"
#include "stm32h7xx_hal.h"

static void gpio_led_init(void);
static void gpio_usart1_init(void);

void gpio_init(void)
{
    gpio_led_init();
    gpio_usart1_init();
}

static void gpio_led_init(void)
{
    GPIO_InitTypeDef gpio_led_config = {0};

    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

    gpio_led_config.Pin = GPIO_PIN_3;
    gpio_led_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_led_config.Pull = GPIO_NOPULL;
    gpio_led_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &gpio_led_config);
}

static void gpio_usart1_init(void)
{
    GPIO_InitTypeDef gpio_usart1_config = {0};
    RCC_PeriphCLKInitTypeDef clock_usart1_config = {0};

    clock_usart1_config.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    clock_usart1_config.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;
    if (HAL_RCCEx_PeriphCLKConfig(&clock_usart1_config) != HAL_OK) {
        while (1);
    }
    __HAL_RCC_USART1_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();

    gpio_usart1_config.Pin = GPIO_PIN_14|GPIO_PIN_15;
    gpio_usart1_config.Mode = GPIO_MODE_AF_PP;
    gpio_usart1_config.Pull = GPIO_NOPULL;
    gpio_usart1_config.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_usart1_config.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOB, &gpio_usart1_config);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

