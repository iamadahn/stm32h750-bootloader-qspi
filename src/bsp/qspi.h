#ifndef QSPI_H_
#define QSPI_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32h7xx_hal.h"

void qspi_init(QSPI_HandleTypeDef *qspi);

#ifdef __cplusplus
}
#endif

#endif
