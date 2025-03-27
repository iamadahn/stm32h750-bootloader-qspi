#include "stm32h7xx_hal.h"

void rcc_init(void)
{
    RCC_OscInitTypeDef osc_config = {0};

    RCC_ClkInitTypeDef clk_config = {0};

    HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

    __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

    while (!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

    HAL_PWR_EnableBkUpAccess();

    __HAL_RCC_LSEDRIVE_CONFIG(RCC_LSEDRIVE_LOW);

    osc_config.OscillatorType = RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
    osc_config.HSEState = RCC_HSE_ON;
    osc_config.LSEState = RCC_LSE_ON;

    osc_config.PLL.PLLState = RCC_PLL_ON;
    osc_config.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc_config.PLL.PLLM = 5;
    osc_config.PLL.PLLN = 192;
    osc_config.PLL.PLLP = 2;
    osc_config.PLL.PLLQ = 2;
    osc_config.PLL.PLLR = 2;
    osc_config.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
    osc_config.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
    osc_config.PLL.PLLFRACN = 0;

    if (HAL_RCC_OscConfig(&osc_config) != HAL_OK) {
        while (1);
    }

    clk_config.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                           |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
                           |RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
    clk_config.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk_config.SYSCLKDivider = RCC_SYSCLK_DIV1;
    clk_config.AHBCLKDivider = RCC_HCLK_DIV2;
    clk_config.APB3CLKDivider = RCC_APB3_DIV2;
    clk_config.APB1CLKDivider = RCC_APB1_DIV2;
    clk_config.APB2CLKDivider = RCC_APB2_DIV2;
    clk_config.APB4CLKDivider = RCC_APB4_DIV2;

    if (HAL_RCC_ClockConfig(&clk_config, FLASH_LATENCY_4) != HAL_OK) {
        while (1);
    }
}

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

int main(void)
{
    HAL_Init();

    rcc_init();

    gpio_init();

    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}
