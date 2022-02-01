#pragma once
#include <Arduino.h>

class UISwitch
{
  private:
    int m_pin;
  public:
    UISwitch(int pin);
    bool state();
};
