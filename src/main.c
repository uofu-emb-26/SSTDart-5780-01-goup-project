#include <stdio.h>
#include "stm32f0xx_hal.h"

#define SWEEP_START_HZ   1000
#define SWEEP_END_HZ     100000
#define SWEEP_STEPS      64

void HardFault_Handler(void) { while (1) {} }

void SysTick_Handler(void) {
    HAL_IncTick();
}

void HAL_MspInit(void) {
    __HAL_RCC_SYSCFG_CLK_ENABLE();
    __HAL_RCC_PWR_CLK_ENABLE();
}

void system_clock_config(void) {
    RCC_OscInitTypeDef oscillator = {0};
    oscillator.OscillatorType      = RCC_OSCILLATORTYPE_HSI;
    oscillator.HSIState            = RCC_HSI_ON;
    oscillator.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
    oscillator.PLL.PLLState        = RCC_PLL_NONE;
    if (HAL_RCC_OscConfig(&oscillator) != HAL_OK) {
        while (1) {}
    }

    RCC_ClkInitTypeDef clock = {0};
    clock.ClockType      = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1;
    clock.SYSCLKSource   = RCC_SYSCLKSOURCE_HSI;
    clock.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    clock.APB1CLKDivider = RCC_HCLK_DIV1;
    if (HAL_RCC_ClockConfig(&clock, FLASH_LATENCY_0) != HAL_OK) {
        while (1) {}
    }
}

UART_HandleTypeDef huart3;

void usart3_init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    // Connect PC10 to TX and PC11 to RX
    //
    // GPIO_MODE_AF_PP and GPIO_AF1_USART3 cause those pins to be connected to
    // the USART3 peripheral
    GPIO_InitTypeDef gpio = {0};
    gpio.Pin       = GPIO_PIN_10 | GPIO_PIN_11;
    gpio.Mode      = GPIO_MODE_AF_PP;
    gpio.Pull      = GPIO_NOPULL;
    gpio.Speed     = GPIO_SPEED_FREQ_LOW;
    gpio.Alternate = GPIO_AF1_USART3;
    HAL_GPIO_Init(GPIOC, &gpio);

    __HAL_RCC_USART3_CLK_ENABLE();

    // Initialize USART3 at 115200 baud, 8 data bits, no parity, 1 stop bit
    huart3.Instance          = USART3;
    huart3.Init.BaudRate     = 115200;
    huart3.Init.WordLength   = UART_WORDLENGTH_8B;
    huart3.Init.StopBits     = UART_STOPBITS_1;
    huart3.Init.Parity       = UART_PARITY_NONE;
    huart3.Init.Mode         = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl    = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);

    // Enable receive interrupt and register the handler with the NVIC
    USART3->CR1 |= USART_CR1_RXNEIE;
    HAL_NVIC_SetPriority(USART3_4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART3_4_IRQn);
}

volatile uint8_t rx_data = 0;
volatile uint8_t rx_flag = 0;

void USART3_4_IRQHandler(void) {
    rx_data = USART3->RDR;
    rx_flag = 1;
}

char receive_char(void) {
    while (!rx_flag);
    rx_flag = 0;
    return rx_data;
}

void transmit_char(char c) {
    while (!(USART3->ISR & USART_ISR_TXE));
    USART3->TDR = c;
}

void transmit_string(const char *str) {
    while (*str != '\0') {
        transmit_char(*str);
        str++;
    }
}

void transmit_uint32(uint32_t freq, uint32_t mag) {
    char buf[32];
    snprintf(buf, sizeof(buf), "%lu %lu\r\n", freq, mag);
    transmit_string(buf);
}

void sweep(void) {
    uint32_t step = (SWEEP_END_HZ - SWEEP_START_HZ) / (SWEEP_STEPS - 1);
    for (int i = 0; i < SWEEP_STEPS; i++) {
        uint32_t frequency = SWEEP_START_HZ + i * step;
        uint32_t magnitude  = i * (4095 / (SWEEP_STEPS - 1));
        transmit_uint32(frequency, magnitude);
    }
    transmit_string("END\r\n");
}

int main(void) {
    HAL_Init();
    system_clock_config();
    usart3_init();

    while (1) {
        switch (receive_char()) {
            case 's':
                sweep();
                break;
            default:
                transmit_string("Error: unknown command\r\n");
                break;
        }
    }
}
