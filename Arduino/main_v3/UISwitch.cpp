#include "UISwitch.h"

UISwitch::UISwitch(int pin)
{
  m_pin = pin;
  pinMode(m_pin, INPUT);
}

bool UISwitch::state()
{
  return digitalRead(m_pin);
}
