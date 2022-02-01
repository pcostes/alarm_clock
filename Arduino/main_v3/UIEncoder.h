#pragma once
#include <Arduino.h>

class UIEncoder
{
  private:
    static volatile byte value;
    static volatile bool A;
    static volatile bool B;
    static volatile bool prev_A;
    static volatile bool prev_B;
    static int m_pin_a, m_pin_b;
    
  public:
    static byte get_value(int max);
    static void init(int pin_a, int pin_b);
    static void encoderA();
    static void encoderB();
    static void set_value(byte val);
};
