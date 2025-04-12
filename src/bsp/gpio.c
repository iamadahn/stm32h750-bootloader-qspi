#include "gpio.h"
#include "stm32h7xx_hal.h"

static void gpio_led_init(void);
static void gpio_usart1_init(void);
static void gpio_qspi_init(void);

void gpio_init(void)
{
    gpio_led_init();
    gpio_usart1_init();
    gpio_qspi_init();
}

static void gpio_led_init(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOH_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();

    HAL_GPIO_WritePin(GPIOE, GPIO_PIN_3, GPIO_PIN_RESET);

    GPIO_InitTypeDef gpio_led_config = {0};

    gpio_led_config.Pin = GPIO_PIN_3;
    gpio_led_config.Mode = GPIO_MODE_OUTPUT_PP;
    gpio_led_config.Pull = GPIO_NOPULL;
    gpio_led_config.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOE, &gpio_led_config);
}

static void gpio_usart1_init(void)
{
    __HAL_RCC_GPIOB_CLK_ENABLE();

    GPIO_InitTypeDef gpio_usart1_config = {0};

    gpio_usart1_config.Pin = GPIO_PIN_14|GPIO_PIN_15;
    gpio_usart1_config.Mode = GPIO_MODE_AF_PP;
    gpio_usart1_config.Pull = GPIO_NOPULL;
    gpio_usart1_config.Speed = GPIO_SPEED_FREQ_LOW;
    gpio_usart1_config.Alternate = GPIO_AF4_USART1;
    HAL_GPIO_Init(GPIOB, &gpio_usart1_config);

    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
}

static void gpio_qspi_init(void)
{
    __HAL_RCC_GPIOE_CLK_ENABLE();
    __HAL_RCC_GPIOB_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();

    GPIO_InitTypeDef gpio_qspi_config = {0};

    gpio_qspi_config.Pin = GPIO_PIN_2;
    gpio_qspi_config.Mode = GPIO_MODE_AF_PP;
    gpio_qspi_config.Pull = GPIO_NOPULL;
    gpio_qspi_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_qspi_config.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOE, &gpio_qspi_config);

    gpio_qspi_config.Pin = GPIO_PIN_2;
    gpio_qspi_config.Mode = GPIO_MODE_AF_PP;
    gpio_qspi_config.Pull = GPIO_NOPULL;
    gpio_qspi_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_qspi_config.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOB, &gpio_qspi_config);

    gpio_qspi_config.Pin = GPIO_PIN_11|GPIO_PIN_12|GPIO_PIN_13;
    gpio_qspi_config.Mode = GPIO_MODE_AF_PP;
    gpio_qspi_config.Pull = GPIO_NOPULL;
    gpio_qspi_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_qspi_config.Alternate = GPIO_AF9_QUADSPI;
    HAL_GPIO_Init(GPIOD, &gpio_qspi_config);

    gpio_qspi_config.Pin = GPIO_PIN_6;
    gpio_qspi_config.Mode = GPIO_MODE_AF_PP;
    gpio_qspi_config.Pull = GPIO_NOPULL;
    gpio_qspi_config.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_qspi_config.Alternate = GPIO_AF10_QUADSPI;
    HAL_GPIO_Init(GPIOB, &gpio_qspi_config);
}

