// Testing Lib - Reading RGB values on SSD1306 (OLED) from TCS34725 (colour sensor)
// Core clock: 64MHz
//
// Requires I2C1 on PB6/PB7 (SCL/SDA)
// PA8  - TIM1 CH1 R
// PA9  - TIM1 CH2 G
// PA10 - TIM1 CH3 B

#include "stm32g031xx.h"
#include <stdio.h>
#include "gpio.h"
#include "pll.h"
#include "usart.h"
#include "i2c.h"
#include "SSD1306.h"
#include "TCS34725.h"

// function prototypes
void _OneTimeInits(void);
uint16_t ScaleToPWM(uint16_t val, uint16_t clear);

TCS34725_t colourSensor;

const uint16_t PWM_MAX = 63999;

uint16_t ScaleToPWM(uint16_t val, uint16_t clear)
{
    if (clear == 0) return 0;

    float norm = (float)val / (float)clear; 
    float pwm  = norm * PWM_MAX;

    if (pwm < 0)       pwm = 0;
    if (pwm > PWM_MAX) pwm = PWM_MAX;

    return (uint16_t)pwm;
}

// -----------------------------------------------------
int main(void)
{
    _PLL_To_64();
    _OneTimeInits();

    _USART2_TxStringXY(1, 6, "Initializing TCS34725...");

    // Initialize sensor
    if (!_TCS34725_Init(&colourSensor))
    {
        _USART2_TxStringXY(1, 7, "TCS34725 not found!");
        _SSD1306_StringXY(0, 0, "TCS34725 not found!");
        _SSD1306_Render();
        while (1);
    }

    _USART2_TxStringXY(1, 7, "TCS34725 ready!");
    _SSD1306_Clear();
    _SSD1306_StringXY(0, 0, "TCS34725 ready!");
    _SSD1306_Render();

    uint16_t r, g, b, c;
    char buff[64];

    while (1)
    {
        // Read raw sensor data
        _TCS34725_GetRawData(&colourSensor, &r, &g, &b, &c);

        // PWM UPDATE FOR RGB LED 
        TIM1->CCR1 = ScaleToPWM(r, c);   // Red
        TIM1->CCR2 = ScaleToPWM(g, c);   // Green
        TIM1->CCR3 = ScaleToPWM(b, c);   // Blue

        // OLED Display
        _SSD1306_Clear();
        sprintf(buff, "R:%5u", r);
        _SSD1306_StringXY(0, 0, buff);
        sprintf(buff, "G:%5u", g);
        _SSD1306_StringXY(0, 1, buff);
        sprintf(buff, "B:%5u", b);
        _SSD1306_StringXY(0, 2, buff);
        sprintf(buff, "C:%5u", c);
        _SSD1306_StringXY(0, 3, buff);
        _SSD1306_Render();

        // Serial Output
        sprintf(buff, "R:%u G:%u B:%u C:%u\r\n", r, g, b, c);
        _USART2_TxString(buff);

        for (volatile int i = 0; i < 500000; ++i); // ~500ms delay
    }
}

// -----------------------------------------------------
void _OneTimeInits(void)
{
    RCC->APBENR2 |= RCC_APBENR2_SYSCFGEN;
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA11_RMP;
    SYSCFG->CFGR1 |= SYSCFG_CFGR1_PA12_RMP;

    // GPIOA Clock
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    // USART
    _USART2_Init(64E6, 38400);
    _USART2_ClearScreen();
    _USART2_TxStringXY(1, 1, "USART started...");

    // I2C1
    _USART2_TxStringXY(1, 3, "Starting I2C1 bus...");
    _I2C1_Init_PB67(100);
    _USART2_TxStringXY(1, 4, "I2C1 bus ready...");

    // OLED
    _SSD1306_DispInit(_SSD1306_OR_DOWN);
    _SSD1306_DisplayOn();
    _SSD1306_Clear();
    _SSD1306_StringXY(0, 0, "OLED Ready");
    _SSD1306_Render();

    //                 RGB LED PWM (TIM1 CH1/CH2/CH3)

    RCC->APBENR2 |= RCC_APBENR2_TIM1EN; // Enable TIM1

    // PA8 (CH1), PA9 (CH2), PA10 (CH3)
    GPIOA->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk);
    GPIOA->MODER |=  (0x2 << GPIO_MODER_MODE8_Pos) |
                     (0x2 << GPIO_MODER_MODE9_Pos) |
                     (0x2 << GPIO_MODER_MODE10_Pos);

    GPIOA->AFR[1] &= ~(0xF << 0 | 0xF << 4 | 0xF << 8);
    GPIOA->AFR[1] |=  (1 << 0) | (1 << 4) | (1 << 8);  // AF1 TIM1

    // PWM Mode 1 for CH1, CH2, CH3
    TIM1->CCMR1 = (6 << 4) | TIM_CCMR1_OC1PE           // CH1
                | (6 << 12) | TIM_CCMR1_OC2PE;         // CH2

    TIM1->CCMR2 = (6 << 4) | TIM_CCMR2_OC3PE;          // CH3

    TIM1->ARR  = PWM_MAX;
    TIM1->CCR1 = 0;
    TIM1->CCR2 = 0;
    TIM1->CCR3 = 0;

    // Enable outputs
    TIM1->CCER |= TIM_CCER_CC1E | TIM_CCER_CC2E | TIM_CCER_CC3E;

    // Main output enable, TIM1 is advanced timer
    TIM1->BDTR |= TIM_BDTR_MOE;

    TIM1->CR1 |= TIM_CR1_ARPE | TIM_CR1_CEN;
}






//// Testing Lib - Reading RGB values on SSD1306 (OLED) from TCS34725 (colour sensor)
//// Core clock: 64MHz
////
//// Requires I2C1 on PB6/PB7 (SCL/SDA)
//// PA8  - TIM1 CH1 R
//// PA9  - TIM1 CH2 G
//// PA10 - TIM1 CH3 B


//#include "stm32g031xx.h"
//#include <stdio.h>
//#include "gpio.h"
//#include "pll.h"
//#include "usart.h"
//#include "i2c.h"
//#include "SSD1306.h"
//#include "TCS34725.h"

//// function prototypes
//void _OneTimeInits(void);

//TCS34725_t colourSensor;

//// -----------------------------------------------------
//int main(void)
//{
//    _PLL_To_64();
//    _OneTimeInits();

//    _USART2_TxStringXY(1, 6, "Initializing TCS34725...");

//    // Initialize sensor
//    if (!_TCS34725_Init(&colourSensor))
//    {
//        _USART2_TxStringXY(1, 7, "TCS34725 not found!");
//        _SSD1306_StringXY(0, 0, "TCS34725 not found!");
//        _SSD1306_Render();
//        while (1);
//    }

//    _USART2_TxStringXY(1, 7, "TCS34725 ready!");
//    _SSD1306_Clear();
//    _SSD1306_StringXY(0, 0, "TCS34725 ready!");
//    _SSD1306_Render();

//    volatile uint16_t r, g, b, c;
//    volatile char buff[64];

//    while (1)
//    {
//        // Read raw data
//        _TCS34725_GetRawData(&colourSensor, &r, &g, &b, &c);

//        // Display on OLED
//        _SSD1306_Clear();
//        sprintf(buff, "R:%5u", r);
//        _SSD1306_StringXY(0, 0, buff);
//        sprintf(buff, "G:%5u", g);
//        _SSD1306_StringXY(0, 1, buff);
//        sprintf(buff, "B:%5u", b);
//        _SSD1306_StringXY(0, 2, buff);
//        sprintf(buff, "C:%5u", c);
//        _SSD1306_StringXY(0, 3, buff);
//        _SSD1306_Render();

//        // Print to serial as well
//        sprintf(buff, "R:%u G:%u B:%u C:%u\r\n", r, g, b, c);
//        _USART2_TxString(buff);

//        for (volatile int i = 0; i < 500000; ++i); // crude 500ms delay
//    }
//}

//// -----------------------------------------------------
//void _OneTimeInits(void)
//{
//    // Turn on GPIO clock for Port A (needed for USART)
//    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

//    // Initialize serial at 38400 baud
//    _USART2_Init(64E6, 38400);
//    _USART2_ClearScreen();
//    _USART2_TxStringXY(1, 1, "USART started...");

//    // Initialize I2C1 on PB6/PB7
//    _USART2_TxStringXY(1, 3, "Starting I2C1 bus...");
//    _I2C1_Init_PB67(100);
//    _USART2_TxStringXY(1, 4, "I2C1 bus ready...");

//    // Initialize OLED
//    _SSD1306_DispInit(_SSD1306_OR_DOWN);
//    _SSD1306_DisplayOn();
//    _SSD1306_Clear();
//    _SSD1306_StringXY(0, 0, "OLED Ready");
//    _SSD1306_Render();
//}
