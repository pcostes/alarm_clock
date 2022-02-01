// Main Code for Clock, Peter Costescu
// https://adafruit.github.io/RTClib/html/index.html reference for RTCLib
#include "RTClib.h"
#include "display.h"
RTC_DS3231 rtc;
bool alarm_triggered = false;

Display disp;

#define INT_PIN 2 // connect to _INT/SQW, no pullup needed
#define BRIGHT_PIN A0

void print_current_time();
void on_alarm();
void set_alarm(DateTime alarm_time);

void setup () 
{

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  if (!rtc.begin()) 
  {
    while (true) 
    {
      digitalWrite(LED_BUILTIN, HIGH);
      delay(300);
      digitalWrite(LED_BUILTIN, LOW);
      delay(300);
    }                               
  }

  disp.init();
  disp.set_data_time(10, 30);
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  rtc.disable32K(); // 32K wave gen not needed, wastes power
  // clear & disable registers, which persist through reupload/reboot
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.writeSqwPinMode(DS3231_OFF); // turn from square wave mode to int mode
  rtc.disableAlarm(2); // not used
  
  // set interrupt pin
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), on_alarm, FALLING); // calls onAlarm on falling edge. INT_PIN normally high.

  // schedule alarm 5 seconds in future
  DateTime alarm_time = rtc.now() + TimeSpan(5);
  set_alarm(alarm_time);
}

long prev_millis = 0;
int epsilon = 100; // every 10 milliseconds, update the display from the clock

void loop () 
{
    //print_current_time();

    // display the current time to the display
    disp.set_brightness(analogRead(BRIGHT_PIN));
    if (millis() - prev_millis > epsilon)
    {
      DateTime current = rtc.now();
      disp.set_data_time(current.hour(), current.minute());
      prev_millis = millis();
    }
    // if Set Alarm Button is pressed, clear the current alarm and set a new one.
    
    // handle checking for alarms
    if (alarm_triggered)
    {
      alarm_triggered = false;
      // handle the triggered alarm
      DateTime current = rtc.now();

      rtc.clearAlarm(1);
      // now set the new alarm  
      // preserve hour, minute, second, increment day. 
      //set_alarm(current + TimeSpan(0, 0, 1, 0)); // temporarily disable alarm for testing.
    }

    //delay(1000);
}

void set_alarm(DateTime alarm_time)
{
  if (!rtc.setAlarm1(alarm_time, DS3231_A1_Hour)) //  DS3231_A1_Hour means hour, minute, and second must match
  {
    Serial.println("Error, couldn't set alarm.");
  }
  else
  {
    char format[] = "MMM DD YYYY hh:mm:ss";
    Serial.print("Alarm will happen at ");
    Serial.println(alarm_time.toString(format));
  }
}
void on_alarm()
{
  alarm_triggered = true;
}
