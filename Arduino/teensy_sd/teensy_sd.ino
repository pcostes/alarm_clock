// sd card reading
#include <SPI.h>
#include <SD.h>

int sd_cs = BUILTIN_SDCARD; // teensy specific
String file_name = "test.txt";
File file1;

void setup() 
{
  Serial.begin(9600);
  Serial.println("Init");

  if (!SD.begin(sd_cs))
  {
    Serial.println("Init failed.");
    for (;;) {delay(1);}
  }

  Serial.println("Opening file " + file_name);
  file1 = SD.open(file_name.c_str());
  if (file1)
  {
    while (file1.available())
    {
      Serial.write(file1.read());
    }

    file1.close();
  }
  else
  {
    Serial.println("Failed to read file.");
  }
}

void loop() 
{
}
