// Desc: Reading RGB values on SSD1306 (OLED) from TCS34725 (colour sensor) + LED
// Core clock: 64MHz
// Note:
// Requires I2C1 on PB6/PB7 (SCL/SDA)
// Author: Jaedyn Carbonell

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

const char* GetColourName(uint8_t r, uint8_t g, uint8_t b, uint16_t c)
{
    // DARK ==> BLACK      
    if (c < 120)
        return "BLACK";

    // convert
    float rf = r / 255.0f;
    float gf = g / 255.0f;
    float bf = b / 255.0f;

    // WHITE
    if (rf > 0.7f && gf > 0.7f && bf > 0.7f)
        return "WHITE";

    // GRAY
    float maxc = rf;
    if (gf > maxc) maxc = gf;
    if (bf > maxc) maxc = bf;

    float minc = rf;
    if (gf < minc) minc = gf;
    if (bf < minc) minc = bf;

    if ((maxc - minc) < 0.10f)
        return "GRAY";

    // YELLOW
    if (rf > 0.4f && gf > 0.4f && bf < 0.3f)
        return "YELLOW";

    // ORANGE      
    if (rf > 0.55f && gf > 0.3f && bf < 0.2f)
        return "ORANGE";

    // PINK      
    if (rf > 0.6f && bf > 0.35f && gf < 0.5f)
        return "PINK";

    // BROWN      
    if (rf > 0.4f && gf > 0.2f && bf < 0.15f)
        return "BROWN";

    // CYAN      
    if (gf > 0.3f && bf > 0.3f)
        return "CYAN";

    // PURPLE      
    if (rf > 0.3f && bf > 0.3f)
        return "PURPLE";
    
    // RED      
    if (rf > gf && rf > bf)
        return "RED";

    // GREEN      
    if (gf > rf && gf > bf)
        return "GREEN";

    // BLUE     
    if (bf > rf && bf > gf)
        return "BLUE";


    return "UNKNOWN";
}

                                                
int main(void)
{
    // ONE-TIME INITS
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
    volatile uint8_t r8, g8, b8;
    volatile char buff[64];

    // MAIN PROGRAM
    while (1)
    {
        _TCS34725_GetRawData(&colourSensor, &r, &g, &b, &c);

        // Avoid divide by zero
        if (c == 0) c = 1;

        r8 = (r * 255) / c;
        g8 = (g * 255) / c;
        b8 = (b * 255) / c;

        if (r8 > 255) r8 = 255;
        if (g8 > 255) g8 = 255;
        if (b8 > 255) b8 = 255;

        const char* colourName = GetColourName(r8, g8, b8, c);

        _SSD1306_Clear();

        // HEADER
        _SSD1306_StringXY(0, 0, "Reading Colour Values");
        _SSD1306_StringXY(0, 1, "~~~~~~~~~~~~~~~~~~~~~");

        // Channel Values
        sprintf(buff, "R:%3u G:%3u B:%3u \r\n", r8, g8, b8);
        _SSD1306_StringXY(0, 2, buff);

        // HEX
        sprintf(buff, "Hex: %02x%02x%02x \r\n", r8, g8, b8);
        _SSD1306_StringXY(0, 4, buff);

        // NAME
        sprintf(buff, "Name: %s", colourName);
        _SSD1306_StringXY(0, 6, buff);

        _SSD1306_StringXY(0, 7, "                 jarb");

        _SSD1306_Render();

        // Print to serial as well
        sprintf(buff, "R:%u G:%u B:%u C:%u (%s)\r\n", r8, g8, b8, c, colourName);
        _USART2_TxString(buff);

        for (volatile int i = 0; i < 500000; ++i); // crude 500ms delay
    }
}

                                                    
void _OneTimeInits(void)
{
    // Turn on GPIO clock for Port A (needed for USART)
    RCC -> IOPENR |= RCC_IOPENR_GPIOAEN;

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