#pragma once
#include <Arduino.h>

class UIButton
{
  public:
    UIButton(int pin, unsigned long epsilon, bool active_state);

    enum UIState {open, held, activated, cleared};
    UIState update();
    UIState get_state();
        
  private:
    int m_pin;
    unsigned long m_epsilon;
    bool m_active_state;
    
    // state machine
    unsigned long time_depressed;
    UIState m_state;
};
