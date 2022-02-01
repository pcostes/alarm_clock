#include "rtc.h"

bool RTC::internal_alarm_triggered = false;

void RTC::set_alarm(DateTime alarm_time)
{
if (!rtc.setAlarm1(alarm_time, DS3231_A1_Hour)) //  DS3231_A1_Hour means hour, minute, and second must match
  {
    // error code, LED?
  }
  else
  {
    char format[] = "MMM DD YYYY hh:mm:ss";
  }
}

void RTC::init(bool compile_time)
{
    if (!rtc.begin()) 
    {
        while (true) 
        {
            Helpers::blink_reps(300, 2);
        }                               
    }

    if (compile_time) // initialize RTC with compile time
    {
      rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    }
    
    rtc.disable32K(); // 32K wave gen not needed

    // clear & disable registers, which persist through reupload/reboot
    rtc.clearAlarm(1);
    rtc.clearAlarm(2);
    rtc.writeSqwPinMode(DS3231_OFF); // turn from square wave mode to int mode
    rtc.disableAlarm(2); // not used

    // set interrupt pin
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(INT_PIN), on_alarm, FALLING); // calls onAlarm on falling edge. INT_PIN normally high.
}

bool RTC::alarm_triggered()
{
    if (internal_alarm_triggered)
    {
        internal_alarm_triggered = false;
        rtc.clearAlarm(1);
        return true;
    }
    return false;
}

void RTC::on_alarm()
{
  internal_alarm_triggered = true;
}

DateTime RTC::now()
{
    return rtc.now();
}

byte RTC::hours()
{
    return rtc.now().hour();
}

byte RTC::minutes()
{
    return rtc.now().minute();
}
