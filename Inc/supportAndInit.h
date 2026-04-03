#include <stdint.h>
#include <stm32f0xx_hal.h>
#include <stm32f0xx_hal_gpio.h>

void PrepRCCLED(void);
void PrepRCCAndConfigOscillator(void)
void PrepRCCUART(void);
void PrepRCCGPIOAnC(void);
void PrepConfigLED(void);
void PrepConfigUART(void);
void PrepConfigSPI(void);
void AD9833_Write(uint16_t data);
void AD5227_Set_Amplitude(uint8_t target);
void writep(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
void togglep(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);