#include "UIButton.h"

UIButton::UIButton(int pin, unsigned long epsilon)
{
  m_pin = pin;
  m_epsilon = epsilon;

  pinMode(m_pin, INPUT_PULLUP);

  // state
  time_depressed = 0;
  m_state = open;
}

UIButton::UIState UIButton::update()
{
  bool press = digitalRead(m_pin) == LOW; // if low, button is pressed

  if (press)
  {
    switch (m_state)
    {
      case open:
        m_state = held; // transitioning to held, set start time
        time_depressed = millis();
        break;
      case held:
        if (millis() - time_depressed > m_epsilon)
          m_state = activated; // only transition if epsilon, otherwise do not change states
        break;
      case activated:
        m_state = cleared;
        break;
      case cleared:
        m_state = cleared;
    }
  }
  else // release
  {
    m_state = open;
  }
  return m_state;
}

UIButton::UIState UIButton::get_state()
{
  return m_state;
}
