int sensors[8] = {0, 1, 2, 3, 4, 5, 6, 7};
int minVal[8];
int maxVal[8];

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 8; i++) {
    pinMode(sensors[i], INPUT);
    minVal[i] = 1023;
    maxVal[i] = 0;
  }
  
  delay(2000);
  Serial.println("Vodi po chernomu i belomu 5 sekund");
  delay(5000);
  
  for (int t = 0; t < 1000; t++) {
    for (int i = 0; i < 8; i++) {
      int val = analogRead(sensors[i]);
      if (val < minVal[i]) minVal[i] = val;
      if (val > maxVal[i]) maxVal[i] = val;
    }
    delay(1);
  }
  
  for (int i = 0; i < 8; i++) {
    Serial.print("Datchik ");
    Serial.print(i);
    Serial.print(": min=");
    Serial.print(minVal[i]);
    Serial.print(" max=");
    Serial.println(maxVal[i]);
  }
}

void loop() {}
