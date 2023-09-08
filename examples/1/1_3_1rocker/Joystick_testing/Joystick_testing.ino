void setup() {
  Serial.begin(9600);
  pinMode(12, INPUT_PULLUP);

}

void loop() {
  Serial.print("X:");
  Serial.print(analogRead(A1));
  Serial.print("\tY:");
  Serial.print(analogRead(A2));
  Serial.print("\tSW:");
  Serial.println(digitalRead(12)==0?"pass":"No pass");

}
