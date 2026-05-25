int sensors[8] = {0, 1, 2, 3, 4, 5, 6, 7};
int minVal[8] = {585, 578, 576, 577, 577, 577, 577, 750};
int maxVal[8] = {595, 589, 590, 589, 589, 588, 588, 762};

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
    digitalWrite(BIN1, 1);
    digitalWrite(BIN2, 0);
    analogWrite(PWMB, rightSpeed);
  } else {
    digitalWrite(BIN1, 0);
    digitalWrite(BIN2, 1);
    analogWrite(PWMB, -rightSpeed);
  }
}

void loop() {
  int sum = 0;
  int active = 0;
  
  for (int i = 0; i < 8; i++) {
    int val = analogRead(sensors[i]);
    int normalized = map(val, minVal[i], maxVal[i], 0, 1);
    normalized = constrain(normalized, 0, 1);
    
    if (normalized == 0) {
      sum += i;
      active++;
    }
  }
  
  if (active == 0) {
    drive(40, 40);
    digitalWrite(LED, LOW);
    return;
  }
  
  float position = (float)sum / active;
  float error = position - 3.5;
  float kp = 8;
  int turn = kp * error;
  
  int leftSpeed = 40 - turn;
  int rightSpeed = 40 + turn;
  
  leftSpeed = constrain(leftSpeed, -255, 255);
  rightSpeed = constrain(rightSpeed, -255, 255);
  
  drive(leftSpeed, rightSpeed);
  digitalWrite(LED, HIGH);
  
  delay(20);
}
