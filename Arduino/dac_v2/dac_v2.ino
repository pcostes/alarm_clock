#include <TimerHelpers.h>

// Timer 0

// input     T0     pin  6  (D4)
// output    OC0A   pin 12  (D6)
// output    OC0B   pin 11  (D5)

const byte timer0Input = 4;
const byte timer0OutputA = 6;
const byte timer0OutputB = 5;
  
void setup() {
   pinMode (timer0OutputA, OUTPUT); 
   pinMode (timer0OutputB, OUTPUT); 
   TIMSK0 = 0;  // no interrupts
   Timer0::setMode (2, Timer0::PRESCALE_1, Timer0::TOGGLE_A_ON_COMPARE | Timer0::TOGGLE_B_ON_COMPARE);
   OCR0A = 150;
   OCR0B = 100;
}  // end of setup

void loop() {}
