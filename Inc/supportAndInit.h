#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void PrepRCCLED(void);
void PrepRCCAndConfigOscillator(void)
void PrepRCCUART(void);
void PrepConfigLED(void);
void PrepConfigUART(void);
void writep(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void togglep(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);