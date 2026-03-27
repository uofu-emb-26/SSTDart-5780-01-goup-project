#include "stm32f0xx_hal.h"

void HardFault_Handler(void) { while (1) {} }

void SysTick_Handler(void) {
    HAL_IncTick();
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

int main(void) {
    HAL_Init();

    while (1) {
    }
}
