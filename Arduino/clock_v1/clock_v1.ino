// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
#define INT_PIN 2 // connect to _INT/SQW, no pullup needed

void print_current_time();
void on_alarm();

void setup () 
{
  Serial.begin(9600);

  while (!Serial);

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
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); // time compiled
    // Example of adjust for Jan 21, 2014 at 3 AM: rtc.adjust(DateTime(2014, 1, 21, 3, 0, 0)); 
  }

  rtc.disable32K(); // 32K wave gen not needed, wastes power
  // clear & disable registers persist through reupload/reboot
  rtc.clearAlarm(1);
  rtc.clearAlarm(2);
  rtc.writeSqwPinMode(DS3231_OFF); // turn from square wave mode to int mode
  rtc.disableAlarm(2);

  // set interrupt pin
  pinMode(INT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INT_PIN), on_alarm, FALLING); // calls onAlarm on falling edge. INT_PIN normally high.

  // schedule alarm 10 seconds in future
  DateTime alarm_time = rtc.now() + TimeSpan(10);
  if (!rtc.setAlarm1(alarm_time, DS3231_A1_Second))
  {
    Serial.println("Error, couldn't set alarm.");
  }
  else
  {
    Serial.print("Alarm will happen in 10 seconds, at ");
    char format[10] = "hh:mm:ss";
    Serial.println(alarm_time.toString(format));
  }

}

void loop () 
{
    Serial.print("SQW: ");
    Serial.print(digitalRead(INT_PIN));
    Serial.print("  Alarm1: ");
    Serial.print(rtc.alarmFired(1));
    Serial.print(" ");
    print_current_time();

    
    if (rtc.alarmFired(1))
    {
      rtc.clearAlarm(1);
      Serial.println("Alarm cleared");
      // now set the new alarm  
    }
    delay(1000);
}

void on_alarm()
{
  Serial.println("Alarm triggered!");
}

void print_current_time()
{
    DateTime now = rtc.now();

    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();

//    Serial.print("Temperature: ");
//    Serial.print(rtc.getTemperature());
//    Serial.println(" C");

}
