#include <EEPROM.h>

void setup() 
{
  Serial.begin(9600);
  byte hrs = EEPROM.read(0);
  byte mns = EEPROM.read(1);
  bool alarm = EEPROM.read(2);
  Serial.println();
  Serial.println(String(hrs) + ":" + String(mns) + " ; " + String(alarm));
  Serial.setTimeout(10);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("Waiting for address.");
  while (Serial.available() == 0)
  {
    ;
  }
  String str = Serial.readString();
  int addr = str.toInt();

  Serial.println("Received address: " + String(addr));

  Serial.println("Waiting for data.");
  while (Serial.available() == 0)
  {
    ;
  }
  str = Serial.readString();
  byte data = byte(str.toInt());
  Serial.println("Received data: " + String(data));

  EEPROM.update(addr, data);
  Serial.println("Wrote data " + String(data) + " to address " + String(addr));
}
