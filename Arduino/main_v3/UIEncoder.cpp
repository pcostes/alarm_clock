#include "UIEncoder.h"

volatile byte UIEncoder::value = 0;
volatile bool UIEncoder::A = false;
volatile bool UIEncoder::B = false;
volatile bool UIEncoder::prev_A = true;
volatile bool UIEncoder::prev_B = true;
int UIEncoder::m_pin_a = 0;
int UIEncoder::m_pin_b = 0;

void UIEncoder::encoderA() 
{
  B = digitalRead(m_pin_b);
  if (B != prev_B) // clockwise
  {
    if (B == HIGH && A == LOW) // only increment 
      value++;
    prev_B = B;   
  }
}

void UIEncoder::encoderB()
{
  A = digitalRead(m_pin_a);
  if (A != prev_A) // ccw
  {
    if (A == HIGH && B == LOW)
      value--;
    prev_A = A;
  }
}

void UIEncoder::init(int pin_a, int pin_b) 
{
  m_pin_a = pin_a;
  m_pin_b = pin_b;
  pinMode(m_pin_a, INPUT);
  pinMode(m_pin_b, INPUT);

  attachInterrupt(digitalPinToInterrupt(m_pin_a), encoderA, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(m_pin_b), encoderB, CHANGE);
}

byte UIEncoder::get_value(int max)
{
  return value % (max + 1);
}

void UIEncoder::set_value(byte val)
{
  value = val;
}
