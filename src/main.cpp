#include "bsp.h"
#include "usart.h"
#include "qspi.h"
#include "stm32h7xx_hal.h"
#include "drivers/w25q/w25q.h"

#define VECT_TAB_OFFSET      0x00000000
#define APPLICATION_ADDRESS  0x90000000

typedef  void (*func_ptr)(void);
func_ptr jump_to_app;

static UART_HandleTypeDef serial;
static QSPI_HandleTypeDef qspi;

int main(void)
{
    bsp_init();

    usart_init(&serial, USART1);

    qspi_init(&qspi);

    auto flash = W25Q::Flash(&qspi);
    flash.init();

    for (uint8_t i = 0; i < 10; i++) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(100);
    }

    flash.enable_memory_map();

	SysTick->CTRL = 0;
	SysTick->LOAD = 0;
	SysTick->VAL = 0;

	for(uint8_t i = 0; i < 8; i++)
	{
		NVIC->ICER[i]=0xFFFFFFFF;
		NVIC->ICPR[i]=0xFFFFFFFF;
	}

	__set_CONTROL(0);
	__disable_irq();
	__set_PRIMASK(1);

	HAL_GPIO_DeInit(GPIOE, GPIO_PIN_3);

	jump_to_app = (func_ptr) (*(__IO uint32_t*)(APPLICATION_ADDRESS + 4));
	__set_MSP(*(__IO uint32_t*) APPLICATION_ADDRESS);
	jump_to_app();

    while (true) {
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_3);
        HAL_Delay(500);
    }
}


extern "C" void SysTick_Handler(void)
{
    HAL_IncTick();
}

extern "C" void USART1_IRQHandler(void)
{
    HAL_UART_IRQHandler(&serial);
}
