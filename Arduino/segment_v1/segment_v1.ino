// seven segment custom code, 1/16/2022 Peter Costescu
// serial NOT ALLOWED, breaks display because wired to 0 & 1 pins (tx/rx on mega)
#include <TimerOne.h>

// which segments to make numerals 0-9?
String numbers[] = {"ABCDEF", "BC", "ABGED", "ABGCD", "FGBC", "AFGCD", "AFGECD", "ABC", "ABCDEFG", "ABGFC"}; //0=ABCDEF, 1 = BC, etc.
char letters[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G'};
bool number_patterns[10][7]; // ten numerals, seven segments
// number patterns[0] is {1, 1, 1, 1, 1, 0} (1 for A-F, 0 for G)

// pins
#define NUM_PINS 8
#define POINT (NUM_PINS - 1) // the last segment
#define NUM_SEGMENTS 7 // point does not count as a segment
String segments = "ABCDEFGP"; // 0=A, 1=B, etc.
int segment_pins[NUM_PINS] = {4, 5, 6, 7, 8, 9, 10, 11}; // A, B, C, D, E, F, G, P

#define NUM_SECTION_PINS 4
int section_pins[NUM_SECTION_PINS] = {12, 0, 1, 3};

// data arrays
int data[NUM_SECTION_PINS] = {0, 0, 0, 0};
bool dps[NUM_SECTION_PINS] = {false, false, false, false}; // points have their own array
int section = 0;

// pwm brightnes
#define BRIGHT_PIN A0
int pwm_counter = 0;
int pwm_max = 20;
int pwm_thresh = 5; // integer arithmetic for threshold, not floating duty cycle

void write_digit(int section, int digit, bool dps_on)
{
  //Serial.println("Writing digit " + String(digit) + " on section " + String(section));
  // set the section
  for (int i = 0; i < NUM_SECTION_PINS; i++)
  {
      digitalWrite(section_pins[i], i != section); // if i == section, write low (false)
  }

  // enable pins
  for (int i = 0; i < NUM_SEGMENTS; i++)
  {
    digitalWrite(segment_pins[i], number_patterns[digit][i]);
  }
  
  // display point
  digitalWrite(segment_pins[POINT], dps_on);
}

void refresh_display()
{
  for (int i = 0; i < NUM_PINS; i++) // include display point
  {
    digitalWrite(segment_pins[i], LOW);
  }
  
  if (pwm_counter < pwm_thresh)
    write_digit(section, data[section], dps[section]);
  
  section++;
  if (section == NUM_SECTION_PINS) // wrap back to 0, update pwm
  {
    section = 0;
    pwm_counter = (pwm_counter + 1) % pwm_max;
  }
}

void setup() 
{
  // initalize segment pins, set to low (off)
  for (int i = 0; i < NUM_PINS; i++)
  {
    pinMode(segment_pins[i], OUTPUT);
    digitalWrite(segment_pins[i], LOW);
  }

  // initalize section pins, set to high (disabled)
  for (int i = 0; i < NUM_SECTION_PINS; i++)
  {
    pinMode(section_pins[i], OUTPUT);
    digitalWrite(section_pins[i], HIGH);
  }

  // calculate and store pin assignments per digit
  for (int numeral = 0; numeral <= 9; numeral++)
  {
    String pattern = numbers[numeral]; // ex. "ABCD"

    // assign number patterns. when writing a digit, loop through number_patterns[digit] and assign high or low to the output pins in order
    for (int i = 0; i < NUM_SEGMENTS; i++)
    {
        number_patterns[numeral][i] = (pattern.indexOf(letters[i]) != -1); // true if pattern contains letters[i]
    }
  }
  
  // attach interrupt
  Timer1.initialize();
  Timer1.attachInterrupt(refresh_display, 200); // 200 microseconds
}

int digit = 0;
int epsilon = 500;
long prev_millis = 0;

void loop() 
{
  dps[1] = true;
  for (int i = 0; i < NUM_SECTION_PINS; i++)
  {
    data[i] = (digit + i) % 10;
  }

  // brightness
  pwm_thresh  = (analogRead(BRIGHT_PIN) * pwm_max) / 1023;

  if (millis() - prev_millis > epsilon)
  {
    digit = (digit + 1) % 10;
    prev_millis = millis();
  }
}
