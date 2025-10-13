//***********************************************************************************
// Program: main.c
// Description: TCS34725 Colour Sensor + OLED Display Demo
// MCU: STM32G031 (64 MHz PLL)
// Author: Jaedyn Carbonell
//***********************************************************************************

#include "stm32g031xx.h"
#include <stdio.h>
#include "gpio.h"
#include "pll.h"
#include "usart.h"
#include "i2c.h"
#include "SSD1306.h"
#include "TCS34725.h"

// Function prototypes
void _OneTimeInits(void);
void _TCS34725_GetColorInfo(uint16_t r, uint16_t g, uint16_t b, uint16_t c,
                            uint8_t *R8, uint8_t *G8, uint8_t *B8, char *colorName);
void _Delay_ms(uint32_t ms);

// Global sensor instance
TCS34725_t colorSensor;

//***********************************************************************************
int main(void)
    // Kick MCU to 64 MHz
    {
    _PLL_To_64();

    // Run startup initializations
    _OneTimeInits();

    // Initialize the color sensor (0x29 address)
    if (!_TCS34725_Init(&colorSensor))
    {
        _USART2_TxStringXY(1, 6, "TCS34725 Init Failed!");
        while (1);
    }

    _USART2_TxStringXY(1, 6, "TCS34725 Init OK");

    // Main loop
    while (1)
    {
        uint16_t r, g, b, c;
        uint8_t R8, G8, B8;
        char buff[64], colorName[16];

        // Get raw color readings
        _TCS34725_GetRawData(&colorSensor, &r, &g, &b, &c);

        // Convert to 0–255 and identify color
        _TCS34725_GetColorInfo(r, g, b, c, &R8, &G8, &B8, colorName);

        // Format message
        sprintf(buff, "RGB:%3d,%3d,%3d %s", R8, G8, B8, colorName);

        // Send to serial console
        _USART2_TxStringXY(1, 8, buff);

        // Display on OLED
        _SSD1306_Clear();
        _SSD1306_StringXY(0, 0, "TCS34725 Sensor");
        _SSD1306_StringXY(0, 2, buff);
        _SSD1306_Render();

        _Delay_ms(500);
    }
}

//***********************************************************************************
void _OneTimeInits(void)
{
    // Enable GPIO port clocks
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    // Start USART2 at 38400 baud
    _USART2_Init(64E6, 38400);
    _USART2_ClearScreen();
    _USART2_TxStringXY(1, 1, "USART started...");

    // Initialize I2C on PB6/PB7
    _USART2_TxStringXY(1, 3, "Starting I2C bus...");
    _I2C1_Init_PB67(100);
    _USART2_TxStringXY(1, 4, "I2C bus started...");

    // Initialize OLED
    _SSD1306_DispInit(_SSD1306_OR_UP);
    _SSD1306_DisplayOn();
}

//***********************************************************************************
void _TCS34725_GetColorInfo(uint16_t r, uint16_t g, uint16_t b, uint16_t c,
                            uint8_t *R8, uint8_t *G8, uint8_t *B8, char *colorName)
{
    if (c == 0) c = 1;

    // Normalize to 0–255
    *R8 = (uint8_t)((float)r / c * 255);
    *G8 = (uint8_t)((float)g / c * 255);
    *B8 = (uint8_t)((float)b / c * 255);

    // Clamp
    if (*R8 > 255) *R8 = 255;
    if (*G8 > 255) *G8 = 255;
    if (*B8 > 255) *B8 = 255;

    // Basic color identification
    if (*R8 > 200 && *G8 > 200 && *B8 > 200)
        sprintf(colorName, "White");
    else if (*R8 > 180 && *G8 < 120 && *B8 < 120)
        sprintf(colorName, "Red");
    else if (*G8 > 180 && *R8 < 120 && *B8 < 120)
        sprintf(colorName, "Green");
    else if (*B8 > 180 && *R8 < 120 && *G8 < 120)
        sprintf(colorName, "Blue");
    else if (*R8 > 180 && *G8 > 120 && *B8 < 80)
        sprintf(colorName, "Yellow");
    else if (*R8 > 120 && *G8 > 100 && *B8 < 90)
        sprintf(colorName, "Skin");
    else if (*R8 < 80 && *G8 < 80 && *B8 < 80)
        sprintf(colorName, "Black");
    else
        sprintf(colorName, "Unknown");
}

void _Delay_ms(uint32_t ms)
{
    // crude blocking delay based on 64MHz system clock
    for (uint32_t i = 0; i < ms * 6400; i++)
    {
        __NOP(); // each NOP takes one CPU cycle
    }
}

