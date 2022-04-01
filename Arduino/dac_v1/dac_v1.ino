#include <TimerOne.h>
#include <TLC5615.h>

TLC5615 dac(53);

const unsigned long interval = 10;
const uint16_t uint_max = 1023;
const uint16_t uint_mid = uint_max / 2;

//const unsigned long freq = 10;
//const unsigned long period = 1000;
//volatile unsigned long us = 0;

// lookup
#define LOOKUP_SIZE 32
uint16_t lookup[LOOKUP_SIZE];
volatile unsigned short cnt = 0;

void output()
{
  dac.analogWrite(lookup[cnt]);
  cnt = (cnt + 1 ) % LOOKUP_SIZE;
  //us += interval;
}
 
void setup() 
{
  dac.begin();  
  Timer1.initialize(interval);
  Timer1.attachInterrupt(output);
  Serial.begin(9600);
  Serial.println(uint_max);
  Serial.println(uint_mid);
  Serial.println();
  for (int i = 0; i < LOOKUP_SIZE; i++)
  {
    lookup[i] = sin(6.28 * i / LOOKUP_SIZE) * uint_mid + uint_mid;
  }
}

void loop()
{
}
