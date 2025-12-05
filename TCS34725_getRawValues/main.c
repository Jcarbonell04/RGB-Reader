// Testing Lib - Reading RGB values on SSD1306 (OLED) from TCS34725 (colour sensor)
// Core clock: 64MHz
//
// Requires I2C1 on PB6/PB7 (SCL/SDA)
// I2C Bus pull-ups required.
//

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

TCS34725_t colourSensor;

// SIMPLE COLOUR CLASSIFICATION
const char* GetColourName(uint16_t r, uint16_t g, uint16_t b, uint16_t c)
{
    // very low brightness  dark
    if (c < 200)
        return "DARK";

    // normalize to brightness
    float rn = (float)r / c;
    float gn = (float)g / c;
    float bn = (float)b / c;

    //// dominant channel detection
    if (rn > gn && rn > bn)
        return "RED";
    if (gn > rn && gn > bn)
        return "GREEN";
    if (bn > rn && bn > gn)
        return "BLUE";
    if (rn > bn && gn > bn)
        return "YELLOW";

    // bright and balanced → white
    if (rn > 0.30 && gn > 0.30 && bn > 0.30)
        return "WHITE";

    return "UNKNOWN";
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

    volatile uint16_t r, g, b, c;
    volatile char buff[64];

    while (1)
    {
        // Read raw data
        _TCS34725_GetRawData(&colourSensor, &r, &g, &b, &c);

        // Determine colour name
        const char* colourName = GetColourName(r, g, b, c);

        // Display on OLED
        _SSD1306_Clear();

        sprintf(buff, "R:%5u", r);
        _SSD1306_StringXY(0, 0, buff);

        sprintf(buff, "G:%5u", g);
        _SSD1306_StringXY(0, 1, buff);

        sprintf(buff, "B:%5u", b);
        _SSD1306_StringXY(0, 2, buff);

        sprintf(buff, "C:%5u", c);
        _SSD1306_StringXY(0, 4, buff);

        sprintf(buff, "Detected: %s", colourName);
        _SSD1306_StringXY(0, 6, buff);

        _SSD1306_Render();

        // Print to serial as well
        sprintf(buff, "R:%u G:%u B:%u C:%u (%s)\r\n", r, g, b, c, colourName);
        _USART2_TxString(buff);

        for (volatile int i = 0; i < 500000; ++i); // crude 500ms delay
    }
}

// -----------------------------------------------------
void _OneTimeInits(void)
{
    // Turn on GPIO clock for Port A (needed for USART)
    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

    // Initialize serial at 38400 baud
    _USART2_Init(64E6, 38400);
    _USART2_ClearScreen();
    _USART2_TxStringXY(1, 1, "USART started...");

    // Initialize I2C1 on PB6/PB7
    _USART2_TxStringXY(1, 3, "Starting I2C1 bus...");
    _I2C1_Init_PB67(100);

    // delay for OLED startup
    for (int i = 0; i < 800000; i++);

    _USART2_TxStringXY(1, 4, "I2C1 bus ready...");

    // Initialize OLED
    _SSD1306_DispInit(_SSD1306_OR_DOWN);
    _SSD1306_DisplayOn();
    _SSD1306_Clear();
    _SSD1306_StringXY(0, 0, "OLED Ready");
    _SSD1306_Render();
}


















//// Testing Lib - Reading RGB values on SSD1306 (OLED) from TCS34725 (colour sensor)
//// Core clock: 64MHz
////
//// Requires I2C1 on PB6/PB7 (SCL/SDA)
//// I2C Bus pull-ups required.
////

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
//    // delay for OLED
//    for (int i = 0; i < 800000; i++);
//    _USART2_TxStringXY(1, 4, "I2C1 bus ready...");

//    // Initialize OLED
//    _SSD1306_DispInit(_SSD1306_OR_DOWN);
//    _SSD1306_DisplayOn();
//    _SSD1306_Clear();
//    _SSD1306_StringXY(0, 0, "OLED Ready");
//    _SSD1306_Render();
//}
