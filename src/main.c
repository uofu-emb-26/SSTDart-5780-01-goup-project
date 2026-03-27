#include "stm32f0xx_hal.h"

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

    // Enable receive register not empty interrupt
    USART3->CR1 |= USART_CR1_RXNEIE;

    // Route USART3_4 interrupts to USART3_4_IRQn handler
    HAL_NVIC_SetPriority(USART3_4_IRQn, 1, 0);
    HAL_NVIC_EnableIRQ(USART3_4_IRQn);
}

void led_init(void) {
    __HAL_RCC_GPIOC_CLK_ENABLE();

    GPIO_InitTypeDef gpio = {0};
    gpio.Pin   = GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9;
    gpio.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio.Pull  = GPIO_NOPULL;
    gpio.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &gpio);

    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6 | GPIO_PIN_7, GPIO_PIN_SET);
}

int main(void) {
    HAL_Init();
    system_clock_config();
    usart3_init();
    led_init();

    while (1) {
    }
}
