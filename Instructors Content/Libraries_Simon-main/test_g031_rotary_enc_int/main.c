// Basic STM32G031 Demo
//  generic bourns PEC11R style rotary encoder
//  on interrupts, not using encoder feature of timer
//
// Core clock is 64MHz
//
// This demo uses the debug console for output
// 
// The encoder is connected to PA9/PA10 and
//  the program will show CW/CCW activity and counts
//  since last inspected
//
// Using Simon's Libraries
//////////////////////////////////////

#include "stm32g031xx.h"
#include "cmsis_gcc.h"

#include <stdio.h>

#include "gpio.h"
#include "pll.h"

// globals to track encoder activity since last read
//  these will count up, need to be cleared on ACK
uint32_t _eventCountCW = 0;
uint32_t _eventCountCCW = 0;

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
   
  // check (often) for encoder activity
  while (1)
  {
    if (_eventCountCCW)
    {
      printf ("\r\nEvent CCW %d", _eventCountCCW);
      _eventCountCCW = 0;
    }
    if (_eventCountCW)
    {
      printf ("\r\nEvent CW %d", _eventCountCW);
      _eventCountCW = 0;
    }
  }
}

void _OneTimeInits(void)
{
  // turn on GPIO clock for Port A and C
  RCC->IOPENR |= RCC_IOPENR_GPIOAEN;
  RCC->IOPENR |= RCC_IOPENR_GPIOCEN;

  // configure encoder inputs as PA9 and PA10
  _GPIO_SetPinMode (GPIOA, 10, _GPIO_PinMode_Input); // D4
  _GPIO_SetPinMode (GPIOA, 9, _GPIO_PinMode_Input);  // D5 

  // configure user LED for output
  _GPIO_PinClear (GPIOC, 6);
  _GPIO_SetPinMode (GPIOC, 6, _GPIO_PinMode_Output); 

  // EXTICR already defaults to PAX as corresponding input in configurable mode RM0444::13.3.3
  
  // set CPU wakeup on EXTI10 line - RM0444::13.5.12
  EXTI->IMR1 |= EXTI_IMR1_IM10_Msk;
  // set CPU wakeup on EXTI9 line - RM0444::13.5.12
  EXTI->IMR1 |= EXTI_IMR1_IM9_Msk;
  
  // configure for rising edge on line EXTI10 - RM0444::13.5.1
  EXTI->RTSR1 |= EXTI_RTSR1_RT10_Msk;
  EXTI->RTSR1 |= EXTI_RTSR1_RT9_Msk;
  
  // event unmask (not necessary here, the interrupt is enough)
  //EXTI->EMR1 |= EXTI_EMR1_EM15_Msk;
  
  // enable the IRQ for this type of interrupt
  NVIC_EnableIRQ (EXTI4_15_IRQn);

  // enable interrupts in general
  __enable_irq();
}

// ISR for interrupt source EXTI15 (configured as a PA15 rising edge trigger interrupt)
void EXTI4_15_IRQHandler (void)
{
  // clear the interrupt bit for a rising edge trigger RM0444::13.5.4
  // don't care what triggered it
  EXTI->RPR1 = EXTI_RPR1_RPIF10_Msk;
  EXTI->RPR1 = EXTI_RPR1_RPIF9_Msk;

  // what port state shows is what matters (1 went high first)
  uint32_t ps = GPIOA->IDR;
  if ((ps & GPIO_IDR_ID10_Msk) && (!(ps&GPIO_IDR_ID9_Msk)))
    _eventCountCCW++;
  if ((ps & GPIO_IDR_ID9_Msk) && (!(ps&GPIO_IDR_ID10_Msk)))
    _eventCountCW++;

  // clear the pending interrupt request
  NVIC_ClearPendingIRQ (EXTI4_15_IRQn);
}
