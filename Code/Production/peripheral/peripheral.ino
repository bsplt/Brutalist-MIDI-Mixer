#include <RotaryEncoder.h>
#include <Wire.h>

#define E0_0 16
#define E0_1 17
#define E1_0 3
#define E1_1 4
#define E2_0 14
#define E2_1 15
#define E3_0 5
#define E3_1 6
#define E4_0 2
#define E4_1 13
#define E5_0 7
#define E5_1 8
#define E6_0 9
#define E6_1 10
#define E7_0 11
#define E7_1 12

RotaryEncoder *e0 = nullptr;
RotaryEncoder *e1 = nullptr;
RotaryEncoder *e2 = nullptr;
RotaryEncoder *e3 = nullptr;
RotaryEncoder *e4 = nullptr;
RotaryEncoder *e5 = nullptr;
RotaryEncoder *e6 = nullptr;
RotaryEncoder *e7 = nullptr;

void setup()
{
  pinMode(E0_0, INPUT);
  pinMode(E0_1, INPUT);
  pinMode(E1_0, INPUT);
  pinMode(E1_1, INPUT);
  pinMode(E2_0, INPUT);
  pinMode(E2_1, INPUT);
  pinMode(E3_0, INPUT);
  pinMode(E3_1, INPUT);
  pinMode(E4_0, INPUT);
  pinMode(E4_1, INPUT);
  pinMode(E5_0, INPUT);
  pinMode(E5_1, INPUT);
  pinMode(E6_0, INPUT);
  pinMode(E6_1, INPUT);
  pinMode(E7_0, INPUT);
  pinMode(E7_1, INPUT);

  Wire.begin(42);
  Wire.onRequest(requestEvent);

  e0 = new RotaryEncoder(E0_0, E0_1);
  attachInterrupt(digitalPinToInterrupt(E0_0), check_e0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E0_1), check_e0, CHANGE);

  e1 = new RotaryEncoder(E1_0, E1_1);
  attachInterrupt(digitalPinToInterrupt(E1_0), check_e1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E1_1), check_e1, CHANGE);

  e2 = new RotaryEncoder(E2_0, E2_1);
  attachInterrupt(digitalPinToInterrupt(E2_0), check_e2, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E2_1), check_e2, CHANGE);

  e3 = new RotaryEncoder(E3_0, E3_1);
  attachInterrupt(digitalPinToInterrupt(E3_0), check_e3, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E3_1), check_e3, CHANGE);

  e4 = new RotaryEncoder(E4_0, E4_1);
  attachInterrupt(digitalPinToInterrupt(E4_0), check_e4, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E4_1), check_e4, CHANGE);

  e5 = new RotaryEncoder(E5_0, E5_1);
  attachInterrupt(digitalPinToInterrupt(E5_0), check_e5, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E5_1), check_e5, CHANGE);

  e6 = new RotaryEncoder(E6_0, E6_1);
  attachInterrupt(digitalPinToInterrupt(E6_0), check_e6, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E6_1), check_e6, CHANGE);

  e7 = new RotaryEncoder(E7_0, E7_1);
  attachInterrupt(digitalPinToInterrupt(E7_0), check_e7, CHANGE);
  attachInterrupt(digitalPinToInterrupt(E7_1), check_e7, CHANGE);
}

void check_e0()
{
  e0->tick();
}

void check_e1()
{
  e1->tick();
}

void check_e2()
{
  e2->tick();
}

void check_e3()
{
  e3->tick();
}

void check_e4()
{
  e4->tick();
}

void check_e5()
{
  e5->tick();
}

void check_e6()
{
  e6->tick();
}

void check_e7()
{
  e7->tick();
}

int pos_e0, pos_e1, pos_e2, pos_e3, pos_e4, pos_e5, pos_e6, pos_e7;

void loop() {}

void requestEvent()
{
  e0->tick();
  e1->tick();
  e2->tick();
  e3->tick();
  e4->tick();
  e5->tick();
  e6->tick();
  e7->tick();

  int npos_e0 = e0->getPosition();
  int npos_e1 = e1->getPosition();
  int npos_e2 = e2->getPosition();
  int npos_e3 = e3->getPosition();
  int npos_e4 = e4->getPosition();
  int npos_e5 = e5->getPosition();
  int npos_e6 = e6->getPosition();
  int npos_e7 = e7->getPosition();

  int d_e0 = npos_e0 - pos_e0 + 63;
  int d_e1 = npos_e1 - pos_e1 + 63;
  int d_e2 = npos_e2 - pos_e2 + 63;
  int d_e3 = npos_e3 - pos_e3 + 63;
  int d_e4 = npos_e4 - pos_e4 + 63;
  int d_e5 = npos_e5 - pos_e5 + 63;
  int d_e6 = npos_e6 - pos_e6 + 63;
  int d_e7 = npos_e7 - pos_e7 + 63;

  char out[] = {(char) d_e0, (char) d_e1, (char) d_e2, (char) d_e3, (char) d_e4, (char) d_e5, (char) d_e6, (char) d_e7};

  pos_e0 = npos_e0;
  pos_e1 = npos_e1;
  pos_e2 = npos_e2;
  pos_e3 = npos_e3;
  pos_e4 = npos_e4;
  pos_e5 = npos_e5;
  pos_e6 = npos_e6;
  pos_e7 = npos_e7;
 
  Wire.write(out);
}