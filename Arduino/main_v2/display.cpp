// display.cpp
// Seven Segment display class, Peter Costescu

#include "display.h"

Display* Display::current_disp = NULL;

void Display::write_digit(byte section, byte digit, bool dps_on)
{
  //Serial.println("Writing digit " + String(digit) + " on section " + String(section));
  // set the section
  for (int i = 0; i < NUM_SECTIONS; i++)
  {
      digitalWrite(section_pins[i], i != section); // if i == section, write low (false)
  }

  // enable pins
  for (int i = 0; i < NUM_SEGMENTS; i++)
  {
    digitalWrite(segment_pins[i], number_patterns[digit][i]);
  }
  
  // display point
  digitalWrite(segment_pins[POINT], dps_on);
}

void Display::refresh_display()
{
  for (int i = 0; i < NUM_PINS; i++) // include display point
  {
    digitalWrite(segment_pins[i], LOW);
  }
  
  if (pwm_counter < pwm_thresh)
    write_digit(section, data[section], dps[section]);
  
  section++;
  if (section == NUM_SECTIONS) // wrap back to 0, update pwm
  {
    section = 0;
    pwm_counter = (pwm_counter + 1) % pwm_max;
  }
}

void Display::init()
{
  current_disp = this;
  // initalize segment pins, set to low (off)
  for (int i = 0; i < NUM_PINS; i++)
  {
    pinMode(segment_pins[i], OUTPUT);
    digitalWrite(segment_pins[i], LOW);
  }

  // initalize section pins, set to high (disabled)
  for (int i = 0; i < NUM_SECTIONS; i++)
  {
    pinMode(section_pins[i], OUTPUT);
    digitalWrite(section_pins[i], HIGH);
  }

  // calculate and store pin assignments per digit
  for (int numeral = 0; numeral <= 9; numeral++)
  {
    String pattern = numbers[numeral]; // ex. "ABCD"

    // assign number patterns. when writing a digit, loop through number_patterns[digit] and assign high or low to the output pins in order
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        number_patterns[numeral][i] = (pattern.indexOf(letters[i]) != -1); // true if pattern contains letters[i]
    }
  }

  // set dps
  bool dps_settings[NUM_SECTIONS] = {false, true, false, false};
  set_dps(dps_settings);
  
  // attach interrupt
  Timer1.initialize();
  Timer1.attachInterrupt(interrupt_callback, 200); // 200 microseconds
}

void Display::set_data_ind(byte ind, byte val)
{
  if (ind >= 0 && ind < NUM_SECTIONS && val < 10)
    data[ind] = val;
}

void Display::set_data_time(byte hrs, byte mns)
{
  if (hrs < 0 || hrs >= 24 || mns < 0 || mns >= 60)
    return;
  data[0] = hrs / 10;
  data[1] = hrs % 10;
  data[2] = mns / 10;
  data[3] = mns % 10;
}

void Display::set_dp(byte ind, bool val)
{
  if (ind >= 0 && ind < NUM_SECTIONS)
    dps[ind] = val;
}

void Display::set_dps(bool vals[NUM_SECTIONS])
{
  for (int i = 0; i < NUM_SECTIONS; i++)
  {
    dps[i] = vals[i];
  }
}

void Display::interrupt_callback()
{
  current_disp->refresh_display();
}

void Display::set_brightness(int pot)
{
  pwm_thresh = (pot * pwm_max) / 1023; // pot 0-1023
}
