int sensors[] = {0, 1, 2, 3, 4, 5, 6, 7};
int minVal[] = {32, 32, 32, 32, 32, 32, 32, 33};
int maxVal[] = {667, 788, 801, 805, 806, 812, 817, 825};
int pos[] = {1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000};
float lastError = 0;

const int AIN1 = 5;
const int AIN2 = 4;
const int PWMA = 3;
const int BIN1 = 6;
const int BIN2 = 7;
const int PWMB = 11;
const int LED = 13;

void setup() {
  pinMode(LED, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(PWMA, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(BIN2, OUTPUT);
  pinMode(PWMB, OUTPUT);

  for (int i = 0; i < 8; i++) {
    pinMode(sensors[i], INPUT);
  }
}

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
    int normalized = map(val, minVal[i], maxVal[i], 0, 1000);
    normalized = constrain(normalized, 0, 1000);

    sum += (long)normalized * pos[i];
    active += normalized;
  }

  if (active < 500) {
    drive(90, 90);
    digitalWrite(LED, LOW);
    return;
  }

  float position = (float)sum / active;
  float error = 4500 - position;
  
  float kp = -0.045;
  float kd = -0.022;


  int turn = (kp * error) + (kd * (error - lastError));
  lastError = error;
  int sp = 150;
  int baseSpeed = sp - abs(turn) ;
  if (baseSpeed < 40) baseSpeed = 40;
  if (baseSpeed > sp) baseSpeed = sp;

  int leftSpeed = baseSpeed - turn;
  int rightSpeed = baseSpeed + turn;

  drive(leftSpeed, rightSpeed);
  digitalWrite(LED, HIGH);

  delay(20);
}
