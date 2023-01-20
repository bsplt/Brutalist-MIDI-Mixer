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

  Serial.println("---");

  while (Wire.available()) {
    char c = Wire.read();
    Serial.print(c);
  }

  Serial.print("\n");

  /*
  Serial.println("---");
  setChannel(13);
  Serial.println(analogRead(INPUTS[2]));
  setChannel(0);
  Serial.println(analogRead(INPUTS[0]));
  */

  int OLD_VALUES[14][4];
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 4; j++) {
      OLD_VALUES[i][j] = VALUES[i][j];
    }
  }

  for (int i = 0; i < 14; i++) {
    setChannel(i);
    for (int j = 0; j < 4; j++) {
      if (j == 2 && i >= 6 && i <= 12) continue;
      if (j == 3 && i >= 7 && i <= 12) continue;
      VALUES[i][j] = analogRead(INPUTS[j]);
    }
  }

  /*
  for (int i = 0; i < 14; i++) {
    for (int j = 0; j < 4; j++) {
      if (OLD_VALUES[i][j] != VALUES[i][j]) {
        Serial.println("Change: " + String(i) + ", " + String(j) + ": " + String(VALUES[i][j]));
      }
    }
  }
*/

  Serial.println(" ");
  for (int j = 0; j < 4; j++) {
    Serial.print(String(j) + ": ");
    for (int i = 0; i < 14; i++) {
      Serial.print(String(VALUES[i][j]) + "\t");
    }
    Serial.print("\n");
  }
  Serial.println(" ");
  Serial.println(" ");
  delay(500);
}

void setChannel(int channel) {
  PORTB = channel << 1;
}