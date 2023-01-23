#include <Wire.h>
#include <MIDIUSB.h>

int CROP = 32;
int SNAP = 64;
int JUMP = 2;

int CHANNELS[] = { 15, 16, 14, 8 };
int INPUTS[] = { 18, 19, 20, 21 };
int LEDS[] = { 4, 5, 6, 7 };
int VALUES[16][5];
int MASK[16][5];
int MIDI[16][5];

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
    int sign = value < 0 ? 8 : 0;
    VALUES[incomingCounter + 0][4] = 0;
    VALUES[incomingCounter + 8][4] = 0;
    VALUES[incomingCounter + sign][4] = abs(value);
    incomingCounter++;
  }

  for (int i = 0; i < 16; i++) {
    setChannel(i);
    for (int j = 0; j < 4; j++) {
      VALUES[i][j] = analogRead(INPUTS[j]);
    }
  }

  int CHANGES[16][5];

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      CHANGES[i][j] = 0;

      if (MASK[i][j] == 0) continue;

      if (MASK[i][j] == 4 && OLD_VALUES[i][j] != VALUES[i][j]) {
        CHANGES[i][j] = 1;
        continue;
      }

      if (VALUES[i][j] < CROP && OLD_VALUES[i][j] > CROP) {
        CHANGES[i][j] = 1;
        continue;
      }

      if (VALUES[i][j] > 1023 - CROP && OLD_VALUES[i][j] < 1023 - CROP) {
        CHANGES[i][j] = 1;
        continue;
      }

      if (abs(OLD_VALUES[i][j] - VALUES[i][j]) < JUMP) {
        VALUES[i][j] = OLD_VALUES[i][j];
        continue;
      };

      if (VALUES[i][j] < CROP && OLD_VALUES[i][j] < CROP) continue;

      if (VALUES[i][j] > 1023 - CROP && OLD_VALUES[i][j] > 1023 - CROP) continue;

      if (MASK[i][j] == 1 && VALUES[i][j] > JUMP && VALUES[i][j] < 1023 - JUMP) continue;

      CHANGES[i][j] = 1;
    }
  }

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {
      if (CHANGES[i][j] == 0) continue;

      int control = j * 16 + i;

      if (MASK[i][j] == 1) {
        int value = VALUES[i][j] > 511 ? 127 : 0;
        if (value == MIDI[i][j]) continue;
        controlChange(0, control, value);
        MIDI[i][j] = value;
        continue;
      }

      if (MASK[i][j] == 2) {
        int value = 127;
        if (VALUES[i][j] >= CROP && VALUES[i][j] < 511 - SNAP) value = map(VALUES[i][j], CROP, 511 - SNAP, 127, 65);
        if (VALUES[i][j] > 511 + SNAP && VALUES[i][j] <= 1023 - CROP) value = map(VALUES[i][j], 511 + SNAP, 1023 - CROP, 63, 0);
        if (VALUES[i][j] >= 511 - SNAP && VALUES[i][j] <= 511 + SNAP) value = 64;
        if (VALUES[i][j] > 1023 - CROP) value = 0;
        if (value == MIDI[i][j]) continue;
        controlChange(0, control, value);
        MIDI[i][j] = value;
        continue;
      }

      if (MASK[i][j] == 3) {
        int value = 127;
        if (VALUES[i][j] > 1023 - CROP) value = 0;
        if (VALUES[i][j] >= CROP && VALUES[i][j] <= 1023 - CROP) value = map(VALUES[i][j], CROP, 1023 - CROP, 127, 0);
        if (i == 13) value = 127 - value;
        if (value == MIDI[i][j]) continue;
        controlChange(0, control, value);
        MIDI[i][j] = value;
        continue;
      }

      if (MASK[i][j] == 4) {
        if (VALUES[i][j] == 0) continue;
        Serial.println(String(control) + ":\t" + String(VALUES[i][j]));
        for (int n = 0; n < VALUES[i][j]; n++) {
          controlChange(0, control, 127);
          controlChange(0, control, 0);
        }
      }
    }
  }

  MidiUSB.flush();
}


void setChannel(int channel) {
  PORTB = channel << 1;
}

void getMask() {
  for (int i = 0; i < 16; i++) {
    for (int j = 0; j < 5; j++) {

      // 0: No input
      // 1: Push button
      // 2: Poti with mid snap
      // 3: Linear poti
      // 4: Rotary encoder

      MASK[i][j] = 0;
      if (j == 0 && i < 14) MASK[i][j] = 1;
      if (j == 1 && i < 14) MASK[i][j] = 1;
      if (j == 1 && i < 14) MASK[i][j] = 1;
      if (j == 2 && i < 5) MASK[i][j] = 2;
      if (j == 2 && i == 5) MASK[i][j] = 3;
      if (j == 2 && i == 13) MASK[i][j] = 3;
      if (j == 3 && i < 5) MASK[i][j] = 2;
      if (j == 3 && i >= 5 && i <= 6) MASK[i][j] = 3;
      if (j == 3 && i == 13) MASK[i][j] = 3;
      if (j == 4) MASK[i][j] = 4;
    }
  }
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = { 0x0B, 0xB0 | channel, control, value };
  MidiUSB.sendMIDI(event);
}