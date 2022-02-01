// Main Code v2 for Clock, Peter Costescu
// reference for RTCLib: https://adafruit.github.io/RTClib/html/index.html 

#include "display.h"
#include "rtc.h"
#include "helpers.h"
#include "UIButton.h"
#include "UISwitch.h"

// display
Display disp;

// rtc
RTC rtc;

// display refresh and brightness
#define BRIGHT_PIN A0
long prev_millis = 0;
int epsilon = 100; // every 10 milliseconds, update the display from the clock

// button, switch
UIButton set_time_button(23, 1000);
bool button_state = true;

UISwitch sw(25);

// led
int alarm_led_pin = 27;

void setup () 
{
  // builtin LED
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  // status LED
  pinMode(alarm_led_pin, OUTPUT);
  digitalWrite(alarm_led_pin, LOW);
  
  // Display
  disp.init();
 
  // rtc
  rtc.init(false);
  rtc.set_alarm(rtc.now() + TimeSpan(0, 0, 0, 5));
}

void loop () 
{
    // display the current time to the display
    disp.set_brightness(analogRead(BRIGHT_PIN));
    if (millis() - prev_millis > epsilon)
    {
      DateTime current = rtc.now();
      disp.set_data_time(current.hour(), current.minute());
      prev_millis = millis();
    }

    // if set time button is pressed, adjust clock
    // if Set Alarm Button is pressed, clear the current alarm and set a new one (eeprom).
    
    // handle checking for alarms
    if (rtc.alarm_triggered())
    {
      // set a new alarm, preserve hour, minute, second from eeprom, increment day. 
      //Helpers::blink_reps(500, 3);
      //rtc.set_alarm(rtc.now() + TimeSpan(0, 0, 0, 4));
    }

    // button update
    
    if (set_time_button.update() == UIButton::UIState::activated)
    {
      digitalWrite(LED_BUILTIN, button_state);
    }

    // switch update
    digitalWrite(alarm_led_pin, sw.state());

    
}
