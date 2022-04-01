volatile boolean state = false;
int out_pin = 44; // pwm
ISR(TIMER3_COMPA_vect)
{
  digitalWrite(out_pin, state);
  state = !state;
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(out_pin, OUTPUT);
  TCCR3A = 0;
  TCCR3B = bit(WGM32) | bit(CS30); // WGM3 100 = CTC, CS3 001 = f_clk
  OCR3A = 180;
  TIMSK3 = bit(OCIE3A); // enable OC Interrupt Enable
  // 40 ns period
}

void loop() 
{
//  digitalWrite(LED_BUILTIN, HIGH);
//  delay(1000);
//  digitalWrite(LED_BUILTIN, LOW);
//  delay(1000);
}
