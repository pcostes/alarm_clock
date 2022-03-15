#include <TimerOne.h>
#include <TLC5615.h>

TLC5615 dac(53);

volatile unsigned long us = 0;
const unsigned long interval = 200;
void output()
{
  dac.analogWrite(512 + 512 * sin(us * 6.28 / 1000.0));
  us += interval;
}
 
void setup() 
{
  dac.begin();  
  Timer1.initialize(interval);
  Timer1.attachInterrupt(output);
}

void loop()
{
}
