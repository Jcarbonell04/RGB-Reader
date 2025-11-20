// USART Library
//
// Functions perform basic async subset of USART operations
//
// Simon Walker
// Created September 2024, initial build, ported from STM32L476 version
//
// Version 1.0
// 
// May 05 2025 - updated to use GPIO lib for setup of pins
///////////////////////////////////////////////////////////////////////

#include <stdio.h>

#include "gpio.h"
#include "usart.h"

void _USART2_Init (long bus_rate, long baud_rate)
{  
  // turn on GPIO clock for Port A, necessary to operate pins
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;

  // apb peripheral clock enable for USART2 [5.4.15]
  RCC->APBENR1 |= RCC_APBENR1_USART2EN;

  // AF1 for PA2 and PA3 means PA2::USART2_TX, PA3::USART2_RX
  _GPIO_SetPinAlternateFunction(GPIOA, 2, 1);
  _GPIO_SetPinAlternateFunction(GPIOA, 3, 1);

  // turn on the USART, with transmitter and receiver enabled
  USART2->CR1 |= USART_CR1_TE_Msk | USART_CR1_RE_Msk;

  USART2->BRR = ((bus_rate * 10) / baud_rate) / 10;

  // last! turn on the usart (most flags can't be set if it's turned on)
  USART2->CR1 |= USART_CR1_UE_Msk;
}

void _USART2_TxByte (unsigned char data)
{
    // transmit a character over USART2
    while (!(USART2->ISR & USART_ISR_TXE_TXFNF))
    {
    }
    
    USART2->TDR = data;   
}

int _USART2_RxByte (unsigned char * pData)
{
  if (USART2->ISR & USART_ISR_RXNE_RXFNE)
  {
    *pData = USART2->RDR;
    return 1;
  }

  return 0;
}

void _USART2_TxString (char * pStr)
{
  while (*pStr)
  {
    _USART2_TxByte (*pStr);
    pStr++;
  }
}

// use escape sequences to place the cursor at the specified position
// this is the \x1B[Y;XH form with formatted argument replacement (sprintf)
void _USART2_GotoXY (int iCol, int iRow)
{
  char buff [20] = {0};
  (void)sprintf (buff, "\x1b[%d;%dH", iRow, iCol);
  _USART2_TxString (buff);
}

// use sci0_GotoXY and sci0_txStr to place the string
void _USART2_TxStringXY (int iCol, int iRow, char * straddr)
{
  // got the X/Y
  _USART2_GotoXY (iCol, iRow);

  // send out string at the new cursor position
  _USART2_TxString (straddr);
}

// use an escape sequence to clear the terminal
void _USART2_ClearScreen (void)
{
  _USART2_TxString ("\x1b[2J");
}