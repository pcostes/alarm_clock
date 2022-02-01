
#define encA 18
#define encB 19

volatile byte value = 0;
volatile bool A = false;
volatile bool B = false;
volatile bool prev_A = true;
volatile bool prev_B = true;

void encoderA() 
{
  B = digitalRead(encB);
  if (B != prev_B) // clockwise
  {
    if (B == HIGH && A == LOW) // only increment 
      value++;
    prev_B = B;   
  }
}

void encoderB()
{
  A = digitalRead(encA);
  if (A != prev_A) // ccw
  {
    if (A == HIGH && B == LOW)
      value--;
    prev_A = A;
  }
}

void setup() 
{
  Serial.begin(9600);
  pinMode(encA, INPUT);
  pinMode(encB, INPUT);

  attachInterrupt(digitalPinToInterrupt(encA), encoderA, CHANGE); 
  attachInterrupt(digitalPinToInterrupt(encB), encoderB, CHANGE);
}

bool last_a = false;
int value_min = 0;
int value_max = 9;

void loop() {
  if (value < value_min)
    value = value_max;
  if (value > value_max)
    value = value_min;
  Serial.println(value);
  delay(10);
}
