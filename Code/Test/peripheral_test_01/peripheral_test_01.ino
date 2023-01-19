#include <RotaryEncoder.h>

#define PIN_IN1 A2
#define PIN_IN2 A3

RotaryEncoder *encoder = nullptr;

void checkPosition()
{
  encoder->tick();
}

void setup()
{
  Serial.begin(9600);
  while (!Serial)
    ;
  Serial.println("InterruptRotator example for the RotaryEncoder library.");
  encoder = new RotaryEncoder(PIN_IN1, PIN_IN2);
  attachInterrupt(digitalPinToInterrupt(PIN_IN1), checkPosition, CHANGE);
  attachInterrupt(digitalPinToInterrupt(PIN_IN2), checkPosition, CHANGE);
}

// Read the current position of the encoder and print out when changed.
void loop()
{
  static int pos = 0;

  encoder->tick(); // just call tick() to check the state.

  int newPos = encoder->getPosition();
  if (pos != newPos) {
    Serial.print("pos:");
    Serial.print(newPos);
    Serial.print(" dir:");
    Serial.println((int)(encoder->getDirection()));
    pos = newPos;
  } 
}
