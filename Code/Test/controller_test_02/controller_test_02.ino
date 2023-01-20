#include <Wire.h>

int CHANNELS[] = { 15, 16, 14, 8 };
int INPUTS[] = { 18, 19, 20, 21 };
int VALUES[14][4];

void setup() {
  Wire.begin();

  for (int i = 0; i < 4; i++) {
    pinMode(CHANNELS[i], OUTPUT);
    pinMode(INPUTS[i], INPUT);
  }
}

void loop() {
  Wire.requestFrom(42, 8, false);
  Serial.print("\tR:\t");

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(String(int(c) - 63) + "\t");
  }

  Serial.print("\n");

  for (int i = 0; i < 14; i++) {
    setChannel(i);
    for (int j = 0; j < 4; j++) {
      if (j == 2 && i >= 6 && i <= 12) continue;
      if (j == 3 && i >= 7 && i <= 12) continue;
      VALUES[i][j] = analogRead(INPUTS[j]);
    }
  }

  for (int j = 0; j < 4; j++) {
    Serial.print("\t" + String(j) + ":\t");
    for (int i = 0; i < 14; i++) {
      Serial.print(String(VALUES[i][j]) + "\t");
    }
    Serial.print("\n");
  }
  Serial.print("\n\n");
  delay(500);
}

void setChannel(int channel) {
  PORTB = channel << 1;
}