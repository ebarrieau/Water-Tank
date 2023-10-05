#include "SimpleTimer.h"

volatile int16_t * pInterruptCounter = NULL;

void SimpleTimer::Setup(volatile int16_t * pCurrent)
{
  
  pInterruptCounter = pCurrent;

  noInterrupts();
  // Clear registers
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = 0;

  // 1 Hz (16000000/((15624+1)*1024))
  OCR1A = 15624;
  // CTC
  TCCR1B |= (1 << WGM12);
  // Prescaler 1024
  TCCR1B |= (1 << CS12) | (1 << CS10);
  // Output Compare Match A Interrupt Enable
  TIMSK1 |= (1 << OCIE1A);
  interrupts();

}

int16_t SimpleTimer::Start(int16_t current, int16_t seconds)
{
  return current + seconds;
}

// Timer return correct result regardless of int16_t overflow if:
//      - Duration of timer is less than half of int16_t range
//      - You are reading result within half of int16_t range of timer completion
// This implementation is good for durations of less than ~22.75 days
bool SimpleTimer::Ended(int16_t current, int16_t target)
{
  return (target - current) < 0;
}

ISR(TIMER1_COMPA_vect)
{
  if (pInterruptCounter)
  {
    (*pInterruptCounter)++;
  }
}