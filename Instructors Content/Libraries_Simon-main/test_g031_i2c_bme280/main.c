// Basic STM32G031 Demo for MBE280 Basic Operations
// Core clock is 64MHz
//
// This requires the I2C1 config on pins PB6(D0)/PB7(D1) (C and D)
// This demo uses the serial port for information output
// I2C Bus pull-ups are required.
//
// Using Simon's Libraries
//////////////////////////////////////

#include "stm32g031xx.h"
#include "cmsis_gcc.h"

#include <stdio.h>
#include "gpio.h"
#include "pll.h"
#include "usart.h"
#include "i2c.h"
#include "BME280.h"

// global counter from interrupt timer
volatile uint32_t _GlobalCounter = 0;

// perform one-time initializations before the main loop
void _OneTimeInits(void);

int main(void)
{
  // device should be running on HSI / 1 so 16MHz
  // not using external 32kHz XTAL yet
  // kick it up to 64MHz
  _PLL_To_64();

  // perform one-time startups for the program, but at full speed
  _OneTimeInits();  

  // I2C bus scan to see what devices are reporting in
  {
    unsigned char pData[128] = {0};
    _I2C1_BusScan(pData);
    int iLine = 7;
    for (int i = 0; i < 128; ++i)
    {
      if (pData[i])
      {
        char buff[80] = {0};
        (void)sprintf (buff, "Device found at 7-bit address: %2.2X ", i);
        _USART2_TxStringXY(1, iLine++, buff);
        // or print to debug console
        printf("%s\r\n", buff);
      }
    }
  }

  // begin main program loop
  do
  { 
    if (!(_GlobalCounter % 5))
    {
      // show the data from the BME280 sensor
      {
        // 0    1    2     3    4    5     6    7
        // PMSB PLSB PXLSB TMSB TLSB TXLSB HMSB HSLB
        uint32_t rawT = BME280_raw_T ();
        int32_t compT = BME280_compensate_T_int32 (rawT);
      
        uint32_t rawH = BME280_raw_H();
        uint32_t compH = BME280_compensate_H_int64 (rawH);

        uint32_t rawP = BME280_raw_P ();
        uint32_t compP = BME280_compensate_P_int32 (rawP);

        char buff[80] = {0};
      
        (void)sprintf (buff, "\x1b[18;1HTemp = %0.2fC     ", compT / 100.0f);
        _USART2_TxString(buff);

        (void)sprintf (buff, "\x1b[19;1HHumidity = %0.2frH     ", compH / 1024.0f);
        _USART2_TxString(buff);

        (void)sprintf (buff, "\x1b[20;1HPressure = %0.2fhPa     ", (compP + 7735) / 100.0f);
        _USART2_TxString(buff);
      }
    }  
  }
  while (1);
}

void _OneTimeInits(void)
{
  // turn on GPIO clock for Port A - can't do anything without this
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  // bring up the serial port, 38400 BAUD currently on VCOM through programmer
  _USART2_Init(64E6, 38400);
  _USART2_ClearScreen();

  // setup a basic timer
  _USART2_TxStringXY(1, 1, "starting timer interrupt...");  
  RCC->APBENR1 |= RCC_APBENR1_TIM2EN_Msk; // turn on timer 2
  TIM2->CR1 = TIM_CR1_URS_Msk;            // overflow/underflow only
  TIM2->ARR = 6400000;                    // 100ms (current prescale is 1)
  TIM2->DIER |= TIM_DIER_UIE_Msk;
  TIM2->CR1 |= TIM_CR1_CEN_Msk;

  NVIC_EnableIRQ (TIM2_IRQn);
  __enable_irq();
  _USART2_TxStringXY(1, 2, "timer interrupt started...");


  _USART2_TxStringXY(1, 3, "USART started...");

  // I2C Stuff
  _USART2_TxStringXY(1, 4, "starting I2C bus...");
  _I2C1_Init_PB67 (_I2C_SpeedMode_100);
  _USART2_TxStringXY(1, 5, "I2C bus started...");

  // bring up the BME280
  _USART2_TxStringXY(1, 6, "starting BME280...");
  {
    if (BME280_Init())
    {
      _USART2_TxStringXY(1, 6, "unexpected error starting BME280...");
    }
    else
    {
      if (BME280_FetchCompensationValues())
      {
        _USART2_TxStringXY(1, 6, "Unable to load BME280 compensation values!...");        
      }
      else
      {
        if (BME280_SetOversampling(BME280_OS_1x, BME280_OS_1x, BME280_OS_1x, BME280_ModeNormal))
        {
          _USART2_TxStringXY(1, 6, "Unable to set BME280 oversampling rates and operating mode!...");        
        }
      }
    }
  }  
  _USART2_TxStringXY(1, 7, "BME280 init ended...");
}

void TIM2_IRQHandler (void)
{
  // check if read write back is allowed here
  //TIM2->SR &= ~TIM_SR_UIF_Msk;
  TIM2->SR = ~TIM_SR_UIF_Msk; // yep, these are rc_w0 bits

  // clear the pending interrupt request
  NVIC_ClearPendingIRQ (TIM2_IRQn);

  // count this tick
  ++_GlobalCounter;
}
