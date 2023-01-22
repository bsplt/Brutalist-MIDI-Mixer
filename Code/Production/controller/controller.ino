#include <Wire.h>

int CHANNELS[] = { 15, 16, 14, 8 };
int INPUTS[] = { 18, 19, 20, 21 };
int LEDS[] = { 4, 5, 6, 7 };
int VALUES[16][5];
int MASK[16][5];

void setup() {
  Wire.begin();

  getMask();

  for (int i = 0; i < 4; i++) {
    pinMode(CHANNELS[i], OUTPUT);
    pinMode(LEDS[i], OUTPUT);
    pinMode(INPUTS[i], INPUT);
  }
}

void loop() {

  Wire.requestFrom(42, 8, false);

  int OLD_VALUES[16][5];
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      OLD_VALUES[i][j] = VALUES[i][j];
    }
  }

  int incomingCounter = 0;
  while (Wire.available()) {
    int value = int(Wire.read()) - 63;
    VALUES[incomingCounter][4] = value;
    incomingCounter++;
  }

  for (int i = 0; i < 16; i++) {
    setChannel(i);
    for (int j = 0; j < 4; j++) {
      VALUES[i][j] = analogRead(INPUTS[j]);
    }
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      if (OLD_VALUES[i][j] != VALUES[i][j] && MASK[i][j] > 0) {
        Serial.println("Change: " + String(j) + ", " + String(i) + ": " + String(VALUES[i][j]));
      }
    }
  }
}

void setChannel(int channel) {
  PORTB = channel << 1;
}

void getMask() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      MASK[i][j] = 1;
      if (j == 2 && i >= 6 && i <= 12) MASK[i][j] = 0;
      if (j == 3 && i >= 7 && i <= 12) MASK[i][j] = 0;
      if (j == 4 && i >= 8) MASK[i][j] = 0;
      if (i >= 14) MASK[i][j] = 0;
    }
  }
}