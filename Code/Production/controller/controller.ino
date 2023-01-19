#include <Wire.h>

void setup() {
  Wire.begin();
}

void loop() {
  Wire.requestFrom(42, 8, false);

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }

  Serial.println("---");

  delay(1000);
}