#include "qspi.h"

void qspi_init(QSPI_HandleTypeDef *qspi)
{
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_QSPI;
    PeriphClkInitStruct.QspiClockSelection = RCC_QSPICLKSOURCE_D1HCLK;

    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK) {
        while (1);
    }

    __HAL_RCC_QSPI_CLK_ENABLE();

    qspi->Instance = QUADSPI;
    qspi->Init.ClockPrescaler = 2;
    qspi->Init.FifoThreshold = 4;
    qspi->Init.SampleShifting = QSPI_SAMPLE_SHIFTING_HALFCYCLE;
    qspi->Init.FlashSize = 1;
    qspi->Init.ChipSelectHighTime = QSPI_CS_HIGH_TIME_5_CYCLE;
    qspi->Init.ClockMode = QSPI_CLOCK_MODE_0;
    qspi->Init.FlashID = QSPI_FLASH_ID_1;
    qspi->Init.DualFlash = QSPI_DUALFLASH_DISABLE;

    if (HAL_QSPI_Init(qspi) != HAL_OK) {
        while (1);
    }
}
