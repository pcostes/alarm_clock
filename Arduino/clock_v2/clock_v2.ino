// Main Code for Clock, Peter Costescu
// https://adafruit.github.io/RTClib/html/index.html reference for RTCLib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#define INT_PIN 2 // connect to _INT/SQW, no pullup needed
char format[50] = "YYYY MMM DD hh:mm:ss";

void print_current_time();
void on_alarm();
void set_alarm(DateTime alarm_time);

void setup () 
{
  Serial.begin(9600);

  while (!Serial);
  Serial.println("Looking for RTC");
  if (!rtc.begin()) 
  {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (true) delay(10);
  }

  // rtc remembers if it lost power since last setup.
  if (rtc.lostPower()) 
  {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(0, 0, 0, 8)); // time compiled, + 5 seconds for upload
    // Example of adjust for Jan 21, 2014 at 3 AM: rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); 
  }

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

void loop () 
{
    print_current_time();

    // if Set Alarm Button is pressed, clear the current alarm and set a new one.
    
    // handle checking for alarms
    if (rtc.alarmFired(1))
    {
      // handle the triggered alarm
      DateTime current = rtc.now();

      rtc.clearAlarm(1);
      // now set the new alarm  
      // preserve hour, minute, second, increment day. 
      set_alarm(current + TimeSpan(0, 0, 1, 0));
    }

    delay(1000);
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
  Serial.println("Alarm triggered!");
}

void print_current_time()
{
    DateTime now = rtc.now();
    char format[] = "MMM DD YYYY hh:mm:ss";
    Serial.println(now.toString(format));

//    Serial.print("Temperature: ");
//    Serial.print(rtc.getTemperature());
//    Serial.println(" C");

}
