#include "usart.h"

void usart_init(UART_HandleTypeDef *handle, USART_TypeDef *self)
{
    RCC_PeriphCLKInitTypeDef clock_usart1_config = {0};
    clock_usart1_config.PeriphClockSelection = RCC_PERIPHCLK_USART1;
    clock_usart1_config.Usart16ClockSelection = RCC_USART16CLKSOURCE_D2PCLK2;

    if (HAL_RCCEx_PeriphCLKConfig(&clock_usart1_config) != HAL_OK) {
        while (1);
    }

    __HAL_RCC_USART1_CLK_ENABLE();

    handle->Instance = self;
    handle->Init.BaudRate = 115200;
    handle->Init.WordLength = UART_WORDLENGTH_8B;
    handle->Init.StopBits = UART_STOPBITS_1;
    handle->Init.Parity = UART_PARITY_NONE;
    handle->Init.Mode = UART_MODE_TX_RX;
    handle->Init.HwFlowCtl = UART_HWCONTROL_NONE;
    handle->Init.OverSampling = UART_OVERSAMPLING_16;
    handle->Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    handle->Init.ClockPrescaler = UART_PRESCALER_DIV1;
    handle->AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(handle) != HAL_OK) {
        while (1);
    }

    if (HAL_UARTEx_SetTxFifoThreshold(handle, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK) {
        while (1);
    }

    if (HAL_UARTEx_SetRxFifoThreshold(handle, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK) {
        while (1);
    }

    if (HAL_UARTEx_DisableFifoMode(handle) != HAL_OK) {
        while (1);
    }
}

