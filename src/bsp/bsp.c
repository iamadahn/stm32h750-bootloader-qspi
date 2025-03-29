#include "bsp.h"
#include "rcc.h"
#include "gpio.h"

#include "stm32h7xx_hal.h"

void bsp_init(void)
{
    HAL_Init();
    rcc_init();
    gpio_init();
}

