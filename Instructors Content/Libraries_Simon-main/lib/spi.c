// SPI implementation

#include "spi.h"
#include "gpio.h"
#include <stdio.h>

void _SPI1_Init (_SPI_Prescale prescale)
{
  // standard pin mappings will be (for full-duplex):
  // PA1 - SCK (AF0)  
  // PA6 - MISO (AF0)
  // PA7 - MOSI (AF0)
  // use whatever GPIO pin(s) you want for *CS or other device signals
  // PA4 in this case will be *CS

  // turn on GPIO clock for Port A, necessary to operate pins
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  const int PIN_SCK = 1;
  const int PIN_SS = 4;
  const int PIN_MISO = 6;
  const int PIN_MOSI = 7;

  _GPIO_PinClear(GPIOA, PIN_SCK); // SCK idle low
  _GPIO_SetPinMode(GPIOA, PIN_SCK, _GPIO_PinMode_Output);
  _GPIO_Set_OutputType(GPIOA, PIN_SCK, _GPIO_OutputType_PushPull);
  _GPIO_SetPinAlternateFunction(GPIOA, PIN_SCK, 0);

  _GPIO_PinClear(GPIOA, PIN_MISO);  
  _GPIO_SetPinMode(GPIOA, PIN_MISO, _GPIO_PinMode_Input);
  _GPIO_SetPinAlternateFunction(GPIOA, PIN_MISO, 0);

  _GPIO_PinClear(GPIOA, PIN_MOSI); // mosi idle low
  _GPIO_SetPinMode(GPIOA, PIN_MOSI, _GPIO_PinMode_Output);
  _GPIO_Set_OutputType(GPIOA, PIN_MOSI, _GPIO_OutputType_PushPull);
  _GPIO_SetPinAlternateFunction(GPIOA, PIN_MOSI, 0);

  // user-defined signals
  _GPIO_PinSet(GPIOA, PIN_SS); // *CS idles high
  _GPIO_SetPinMode(GPIOA, PIN_SS, _GPIO_PinMode_Output);

  // apb peripheral clock enable for SPI1 [5.4.16]
  RCC->APBENR2 |= RCC_APBENR2_SPI1EN;

  // configure SPI
  // no bimode
  // no crc
  // full-duplex
  // MSB first
  // SPI enabled
  // BAUD = user defined prescale
  // master configuration
  // clock idle polarity low
  // clock phase first
  SPI1->CR1 = 0b0000001100000100;
  SPI1->CR1 |= ((unsigned char)(prescale & 0b111)) << 3u;
  SPI1->CR2 = 0x0700;
  SPI1->CR2 |= SPI_CR2_FRXTH;
  SPI1->CR1 |= SPI_CR1_SPE;

  // NOTE: clock measured at 4MHz when div by 16
  // CR2 -> all defaults are good
}

// alter this function to assert your GPIO output connected to *CS on the target device
//  the library default is PA4, but it can be any GPIO pin you've connected to *CS line of the target
void _SPI1_SelectTarget (void)
{
  _GPIO_PinClear(GPIOA, 4); // assert *CS on target
}

void _SPI1_DeselectTarget (void)
{
  _GPIO_PinSet(GPIOA, 4); // unassert *CS on target
}

int _SPI1_Transaction (uint8_t* pTxData, uint8_t* pRxData, int ByteCount)
{
    // assert chip select first
    _SPI1_SelectTarget();

    // 'lil delay for device to wake up (not necessary)
    //  datasheet for MB85 says 10ns for CS setup, and 400us for sleep recovery
    // measured at about 3us under 64MHz CPU clock conditions

    int rxBuffpos = 0;

    // write the data out to the device
    for (int i = 0; i < ByteCount; ++i)
    {      
      while (!(SPI1->SR & SPI_SR_TXE))
        ;;;

      // force 8-bit write
      *(volatile uint8_t*)(&SPI1->DR) = pTxData[i];     
      
      // collect any arriving data while we are transmitting
      //  otherwise, in large transfers, the RX FIFO will fill while we are sending
      if (SPI1->SR & SPI_SR_RXNE)
        pRxData[rxBuffpos++] = SPI1->DR;       
    }

    // while the transaction is still in progress,
    //  and bytes have been received, read them remaining bytes    
    while (SPI1->SR & (SPI_SR_BSY | SPI_SR_RXNE))
    {
      if (SPI1->SR & SPI_SR_RXNE)
        pRxData[rxBuffpos++] = SPI1->DR;
    }
  
    // free the device
    _SPI1_DeselectTarget();

    // return the number of bytes rxed (should be the same as tx)
    return rxBuffpos;
}

