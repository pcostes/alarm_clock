// Main Code v3 for Clock, Peter Costescu

#include <EEPROM.h>
#include "display.h"
#include "rtc.h"
#include "helpers.h"
#include "UIButton.h"
#include "UISwitch.h"
#include "UIEncoder.h"

// display
Display disp;

// rtc
RTC rtc;

// display refresh and brightness
#define BRIGHT_PIN A0
long prev_millis = 0;
int epsilon = 100; // every 100 milliseconds, update the display from the clock

// button, switch, encoder shaft
UIButton set_time_button(23, 1000, LOW);
bool is_setting_time = false; //whether we are currently in a blinking set mode

UISwitch sw(25);

UIButton next_digit_button(35, 50, LOW);

// led
int alarm_led_pin = 27;

// temporary setting data
int current_section = 0;
byte setting_data[NUM_SECTIONS] = {0, 0, 0, 0};
byte section_maxes[NUM_SECTIONS] = {2, 9, 5, 9};
bool choose_switch_time = false; // otherwise, would be setting the alarm
void setup () 
{
  // builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // status LED
  pinMode(alarm_led_pin, OUTPUT);
  digitalWrite(alarm_led_pin, LOW);

  // encoder
  UIEncoder::init(18, 19);

  // Display
  disp.init();
  // rtc
  rtc.init(false);
  //rtc.set_time(11, 5); // 1:05 AM
  //rtc.set_alarm(1, 6); 
  //rtc.set_alarm(rtc.now() + TimeSpan(0, 0, 0, 5));
}

void loop () 
{
    // display the current time to the display
    disp.set_brightness(analogRead(BRIGHT_PIN));
    if (millis() - prev_millis > epsilon)
    {
      DateTime current = rtc.now();
      if (is_setting_time)
      {
        disp.set_data(setting_data);
      }
      else
      {
          disp.set_data_time(rtc.hour_12(), current.minute());

      }
      prev_millis = millis();
    }

    // if set time button is pressed, adjust clock
    // if Set Alarm Button is pressed, clear the current alarm and set a new one (eeprom).
    
    // handle checking for alarms
    if (rtc.alarm_triggered())
    {
      // set a new alarm, preserve hour, minute, second from eeprom, increment day. 
      disp.set_data_time(rtc.hour_12(), rtc.minutes());
      Helpers::blink_reps(200, 10);
      rtc.clear();
      //rtc.set_time(1, 5);
      //rtc.set_alarm(rtc.now() + TimeSpan(0, 0, 0, 4));
    }

    // button update
    
    if (set_time_button.update() == UIButton::UIState::activated)
    {
      is_setting_time = !is_setting_time;
      if (is_setting_time)
        choose_switch_time = !sw.state(); // if we are going *into* a setting and if switch is left (low), we want to set the time. right, alarm
      if (is_setting_time)
      {
        current_section = disp.set_digit_to_blink(0);
        byte hrs, mins;
        
        if (choose_switch_time)
        {
          hrs = rtc.hours();
          mins = rtc.minutes();
        }
        else
        {
          // initalize from eeprom
          hrs = EEPROM.read(0);
          mins = EEPROM.read(1);
        }
        setting_data[0] = hrs / 10;
        setting_data[1] = hrs % 10;
        setting_data[2] = mins / 10;
        setting_data[3] = mins % 10;
        UIEncoder::set_value(setting_data[current_section]);
      }
      else // set the time
      {
        disp.set_digit_to_blink(-1);
        byte hrs = setting_data[0] * 10 + setting_data[1];
        byte mins = setting_data[2] * 10 + setting_data[3];
        if (choose_switch_time)
        {
          rtc.set_time(hrs, mins);
          rtc.set_alarm(EEPROM.read(0), EEPROM.read(1));
          rtc.clear();
        }
        else
        {
          // set the eeprom
          
          EEPROM.update(0, hrs);
          EEPROM.update(1, mins);
          rtc.set_alarm(hrs, mins);
          // set the alarm on rtc
        }
      }
      //digitalWrite(LED_BUILTIN, is_setting_time);
    }
    if (next_digit_button.update() == UIButton::UIState::activated)
    {
      if (is_setting_time)
      {
        current_section = disp.blink_right();
        UIEncoder::set_value(setting_data[current_section]);
      }
    }
    if (setting_data[0] > 1)
    {
      section_maxes[1] = 3;
      if (setting_data[1] > section_maxes[1])
        setting_data[1] = section_maxes[1];
    }
    else
      section_maxes[1] = 9;
    setting_data[current_section] = UIEncoder::get_value(section_maxes[current_section]);
    // switch update
    digitalWrite(alarm_led_pin, rtc.am());

    
}
