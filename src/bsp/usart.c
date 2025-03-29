#include "usart.h"

void usart_init(UART_HandleTypeDef *handle, USART_TypeDef *self)
{
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

