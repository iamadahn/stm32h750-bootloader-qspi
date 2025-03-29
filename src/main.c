#include "bsp.h"
#include "stm32h7xx_hal.h"

int main(void)
{
    bsp_init();
    while (1) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}

