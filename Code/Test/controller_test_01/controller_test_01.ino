#define S0 15
#define S1 16
#define S2 14
#define S3 8

#define I0 18
#define I1 19
#define I2 20
#define I3 21

int values[6];

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  pinMode(I0, INPUT);
  pinMode(I1, INPUT);
  pinMode(I2, INPUT);
  pinMode(I3, INPUT);

  Serial.begin(9600);
}

void loop() {
  delay(200);

  digitalWrite(S0, HIGH);
  digitalWrite(S1, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);

  Serial.println(analogRead(I0));
}
