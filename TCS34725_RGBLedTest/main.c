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

TCS34725_t colourSensor;

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

        // Display on OLED
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

        // Print to serial as well
        sprintf(buff, "R:%u G:%u B:%u C:%u\r\n", r, g, b, c);
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
    _USART2_TxStringXY(1, 4, "I2C1 bus ready...");

    // Initialize OLED
    _SSD1306_DispInit(_SSD1306_OR_DOWN);
    _SSD1306_DisplayOn();
    _SSD1306_Clear();
    _SSD1306_StringXY(0, 0, "OLED Ready");
    _SSD1306_Render();
}


////***********************************************************************************
//// Program: main.c
//// Description: Test program - read RGB values from TCS34725 and display on SSD1306
//// Author: Jaedyn Carbonell
//// Date: October 2025
////***********************************************************************************

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
//void _Delay_ms(unsigned int ms);

//TCS34725_t colorSensor; 

////***********************************************************************************
//// main()
////***********************************************************************************
//int main(void)
//{
//    // Change clock
//    _PLL_To_64();

//    // Initialize all peripherals
//    _OneTimeInits();

//    _TCS34725_Init(&colorSensor);

//    // oeld display init
//    _SSD1306_Clear();
//    _SSD1306_StringXY(0, 0, "TCS34725 Ready");
//    _SSD1306_Render();
//    _USART2_TxStringXY(1, 6, "TCS34725 Ready...");

//    // Main loop
//    while (1)
//    {
//        unsigned short r, g, b, c;
//        char buff[64];

//        // Read raw RGB and clear channel values
//        //_TCS34725_ReadRGB(&r, &g, &b, &c);

//        // Print to USART
//        sprintf(buff, "R:%5u \n G:%5u\n  B:%5u\n  C:%5u", r, g, b, c);
//        _USART2_TxStringXY(1, 8, buff);

//        // Print to OLED
//        _SSD1306_Clear();
//        _SSD1306_StringXY(0, 0, "TCS34725 RGB:");
//        _SSD1306_StringXY(0, 1, buff);
//        _SSD1306_Render();

//        // short delay (around 1 second)
//        _Delay_ms(1000);
//    }
//}

////***********************************************************************************
//// _OneTimeInits()
//// Description: Performs system and peripheral initialization
////***********************************************************************************
//void _OneTimeInits(void)
//{
//    // enable GPIOA clock
//    RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

//    // setup USART2 (38400 baud @ 64MHz)
//    _USART2_Init(64E6, 38400);
//    _USART2_ClearScreen();
//    _USART2_TxStringXY(1, 1, "USART2 Started...");

//    // setup I2C1 on PB6/PB7 @ 100kHz
//    _USART2_TxStringXY(1, 3, "Starting I2C bus...");
//    _I2C1_Init_PB67(_I2C_SpeedMode_100);
//    _USART2_TxStringXY(1, 4, "I2C bus ready...");

//    // initialize OLED
//    _SSD1306_DispInit(_SSD1306_OR_UP);
//    _SSD1306_DisplayOn();
//    _SSD1306_Clear();
//    _SSD1306_StringXY(0, 0, "SSD1306 Ready");
//    _SSD1306_Render();
//}

////***********************************************************************************
//// _Delay_ms()
//// Description: crude software delay (approximate)
////***********************************************************************************
//void _Delay_ms(unsigned int ms)
//{
//    for (unsigned int i = 0; i < ms * 8000; ++i)
//    {
//        __NOP();
//    }
//}


////// Testing Lib - Reading rgb valuse on SSD1306(oled display) from TCS34725(colour sensor)
////// Core clock is 64MHz
//////
////// This requires the I2C1 config on pins PB6/PB7 (d0 and d1)
////// I2C Bus pull-ups are required.
//////
//////////////////////////////////////////

////// define library headers here
////#include "stm32g031xx.h"
////#include <stdio.h>
////#include "gpio.h"
////#include "pll.h"
////#include "usart.h"
////#include "i2c.h"
////#include "SSD1306.h"
////#include "TCS34725.h"

////// perform one-time initializations before the main loop
////void _OneTimeInits(void);

////int main(void)
////{
////  // device should be running on HSI / 1 so 16MHz
////  // not using external 32kHz XTAL yet
////  // kick it up to 64MHz
////  _PLL_To_64();

////  // perform one-time startups for the program, but at full speed
////  _OneTimeInits();  

////  // I2C bus scan to see what devices are reporting in
////  {
////        // moved to main loop
////  }

////  // begin main program loop
////  // variables used in the main loop are created here 
////  do
////  { 
////    // do somethig here
////    unsigned char pData[128] = {0};
////    _I2C1_BusScan(pData);
////    int iLine = 5;
////    for (int i = 0; i < 128; ++i)
////    {
////      if (pData[i])
////      {
////        char buff[80] = {0};
////        (void)sprintf (buff, "Device found at 7-bit address: %2.2X ", i);
////        _USART2_TxStringXY(1, iLine++, buff);
////        // or print to debug console
////        printf("%s\r\n", buff);
////        _SSD1306_StringXY(0, 0, buff);
////        _SSD1306_Render();
////      }
////    }
////  }
////  while (1);
////}

////void _OneTimeInits(void)
////{
////  // turn on GPIO clock for Port A - can't do anything without this
////  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

////  // bring up the serial port, 38400 BAUD currently on VCOM through programmer
////  _USART2_Init(64E6, 38400);
////  _USART2_ClearScreen();

////  _USART2_TxStringXY(1, 1, "USART started...");

////  // I2C Stuff
////  _USART2_TxStringXY(1, 3, "starting I2C bus...");
////  _I2C1_Init_PB67(100);
////  _USART2_TxStringXY(1, 4, "I2C bus started...");

////  // one-time OLED init
////  _SSD1306_DispInit(_SSD1306_OR_UP);
////  _SSD1306_DisplayOn();
////}

