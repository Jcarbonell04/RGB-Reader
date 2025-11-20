// GPIO Library
//
// Functions perform basic GPIO operations and configurations
//
// Simon Walker
// Created September 2024, initial build, ported from STM32L476 version
//
// Version 1.0
// 
///////////////////////////////////////////////////////////////////////

#include "gpio.h"

void inline _GPIO_PinToggle (GPIO_TypeDef * pPort, int PinNumber)
{
  if (PinNumber < 0 || PinNumber > 15)
    return;

  pPort->ODR ^= (1u << PinNumber);
}

void inline _GPIO_PinSet (GPIO_TypeDef * pPort, int PinNumber)
{
  if (PinNumber < 0 || PinNumber > 15)
    return;

  pPort->BSRR |= (1u << PinNumber);
}

void inline _GPIO_PinClear (GPIO_TypeDef * pPort, int PinNumber)
{
  if (PinNumber < 0 || PinNumber > 15)
    return;

  pPort->BSRR |= (0x10000u << PinNumber);
}

void _GPIO_SetPinMode (GPIO_TypeDef * pPort, int PinNumber, _GPIO_PinMode mode)
{
  // assumes that peripheral clock is on for this port

  if (PinNumber < 0 || PinNumber > 15)
    return;

  // clear current mode bits
  pPort->MODER &= ~(0b11ul << (PinNumber * 2));
  
  // set desired mode bits
  pPort->MODER |= ((unsigned long)mode << (PinNumber * 2));  
}

int _GPIO_GetPinIState (GPIO_TypeDef * pPort, int PinNumber)
{
  // assumes that peripheral clock is on for this port

  if (PinNumber < 0 || PinNumber > 15)
    return 0;

  return (pPort->IDR & (1u << PinNumber));
}

int _GPIO_GetPinOState (GPIO_TypeDef * pPort, int PinNumber)
{
  // assumes that peripheral clock is on for this port

  if (PinNumber < 0 || PinNumber > 15)
    return 0;

  return (pPort->ODR & (1u << PinNumber));
}

int _GPIO_Set_OutputType (GPIO_TypeDef * pPort, int PinNumber, _GPIO_OutputType otype)
{
  // only pins 0-15 inclusive are permitted
  if (PinNumber > 15)
    return -1;
  
  // set output state
  if (otype == _GPIO_OutputType_PushPull)
    pPort->OTYPER &= ~(1u << PinNumber);
  
  else
  //if (otype == _GPIO_OutputType_OpenDrain)
    pPort->OTYPER |= (1u << PinNumber);
   
  return 0;  
}

int _GPIO_Set_PullType (GPIO_TypeDef * pPort, int PinNumber, _GPIO_PullType ptype)
{
  // only pins 0-15 inclusive are permitted
  if (PinNumber > 15)
    return -1;

  // clear bits first (puts pin in no-pull state)
  pPort->PUPDR &= ~(0x3u << (2 * PinNumber));

  // put back desired configuration
  pPort->PUPDR |= ((unsigned int)ptype << (2 * PinNumber));

  return 0;
}

int _GPIO_SetPinAlternateFunction (GPIO_TypeDef * pPort, unsigned int PinNumber, unsigned int FunctionNumber)
{
  // only functions 0-7 inclusive are permitted
  if (FunctionNumber > 7)
    return -1;

  // only pins 0-15 inclusive are permitted
  if (PinNumber > 15)
    return -2;

  // switch pin to alternate function mode
  // punch out 2 bits for AF for this pin
  pPort->MODER &= ~(0x3u << (2 * PinNumber));
  // set bits as '10' for this pin position (AF)
  pPort->MODER |= (0x2u << (2 * PinNumber));

  // set alternate function
  // register is broken into two, pins 0-7 on afr[0], pins 8-15 on afr[1]
  int afindex = 0;
  unsigned int PinPos = PinNumber;

  if (PinNumber > 7)
  {
    PinPos -= 8;
    afindex = 1;
  }

  // clear bits first
  pPort->AFR[afindex] &= ~(0xfu << (4 * PinPos));
  // set requested af
  pPort->AFR[afindex] |= (FunctionNumber << (4 * PinPos));

  return 0;
}
