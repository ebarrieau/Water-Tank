#include "SimpleTimer.h"

void SimpleTimer::Setup()
{
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

uint32_t SimpleTimer::Start(uint32_t current, float seconds)
{
  uint32_t duration = seconds * MILLIS_IN_SECOND;
  return current + duration;
}

bool SimpleTimer::Ended(uint32_t current, uint32_t target)
{
  return (current > target);
}
