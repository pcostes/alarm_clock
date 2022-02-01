#include "RTClib.h"
#include "helpers.h"

#define INT_PIN 2 // connect to _INT/SQW, no pullup needed


class RTC 
{
    private:
        static bool internal_alarm_triggered;
        static void on_alarm();
        
        RTC_DS3231 rtc;

    public:
        void init(bool compile_time);
        bool alarm_triggered();
        DateTime now();
        byte hours();
        byte minutes();
        void set_alarm(DateTime alarm_time); 

};
