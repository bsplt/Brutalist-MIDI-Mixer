#include <Wire.h>

int CHANNELS[] = { 15, 16, 14, 8 };
int INPUTS[] = { 18, 19, 20, 21 };
int LEDS[] = { 4, 5, 6, 7 };
int VALUES[16][4];
int MASK[16][4];

int counter;

void setup() {
  Wire.begin();

  getMask();

  for (int i = 0; i < 4; i++) {
    pinMode(CHANNELS[i], OUTPUT);
    pinMode(LEDS[i], OUTPUT);
    pinMode(INPUTS[i], INPUT);
  }

  counter = 0;
}

void loop() {

  for (int i = 0; i < 4; i++) {
    digitalWrite(LEDS[i], counter & 1 << i);
  }
  counter++;

  Wire.requestFrom(42, 8, false);
  Serial.print("\tR:\t");

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(String(int(c) - 63) + "\t");
  }

  Serial.print("\n");

  for (int i = 0; i < 16; i++) {
    setChannel(i);
    for (int j = 0; j < 4; j++) {
      VALUES[i][j] = analogRead(INPUTS[j]);
    }
  }

  for (int j = 0; j < 4; j++) {
    Serial.print("\t" + String(j) + ":\t");
    for (int i = 0; i < 16; i++) {
      if (MASK[i][j] > 0) {
        Serial.print(String(VALUES[i][j]));
      } else {
        Serial.print("-");
      }
      Serial.print("\t");
    }
    Serial.print("\n");
  }
  Serial.print("\n\n");
  delay(500);
}

void setChannel(int channel) {
  PORTB = channel << 1;
}

void getMask() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 4; j++) {
      MASK[i][j] = 1;
      if (j == 2 && i >= 6 && i <= 12) MASK[i][j] = 0;
      if (j == 3 && i >= 7 && i <= 12) MASK[i][j] = 0;
      if (i >= 14) MASK[i][j] = 0;
    }
  }
}