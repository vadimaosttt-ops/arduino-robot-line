// Робот по линии.

// Датчики линии (A0..A7)
int sensors[] = {0, 1, 2, 3, 4, 5, 6, 7};
int minVal[] = {32, 32, 32, 32, 32, 32, 32, 33};          // чёрное
int maxVal[] = {667, 788, 801, 805, 806, 812, 817, 825};  // белое
int pos[] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};  // 4500 = центр

// Моторы
const int AIN1 = 5;
const int AIN2 = 4;
const int PWMA = 3;
const int BIN1 = 6;
const int BIN2 = 7;
const int PWMB = 11;
const int LED = 13;

// ПИД
float lastError = 0;
float lastPosition = 4500;
float kp = -0.03;
float kd = -0.075;
int baseSpeed = 180;

void setup() {
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);
  pinMode(LED, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(sensors[i], INPUT);
  }
}

// leftSpeed/rightSpeed: -255..255, минус = назад
void drive(int leftSpeed, int rightSpeed) {
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);

  if (leftSpeed >= 0) {
    digitalWrite(AIN2, 1);
    digitalWrite(AIN1, 0);
    analogWrite(PWMA, leftSpeed);
  } else {
    digitalWrite(AIN2, 0);
    digitalWrite(AIN1, 1);
    analogWrite(PWMA, -leftSpeed);
  }

  if (rightSpeed >= 0) {
    digitalWrite(BIN1, 0);
    digitalWrite(BIN2, 1);
    analogWrite(PWMB, rightSpeed);
  } else {
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 0);
    analogWrite(PWMB, -rightSpeed);
  }
}

void loop() {
  long sum = 0;
  long active = 0;

  for (int i = 0; i < 8; i++) {
    int val = analogRead(sensors[i]);
    int norm = map(val, minVal[i], maxVal[i], 0, 1000);
    norm = constrain(norm, 0, 1000);
    sum += (long)norm * pos[i];
    active += norm;
  }

  float error;

  if (active < 150) {
    // линия потеряна — доворачиваем туда, где видели её последний раз
    error = (lastPosition < 4500) ? 1500 : -1500;
  } else {
    float position = (float)sum / active;
    lastPosition = position;
    error = 4500 - position;
  }

  int turn = (kp * error) + (kd * (error - lastError));
  lastError = error;
  turn = constrain(turn, -200, 200);

  digitalWrite(LED, active >= 150);

  drive(baseSpeed - turn, baseSpeed + turn);
  delay(5);
}
