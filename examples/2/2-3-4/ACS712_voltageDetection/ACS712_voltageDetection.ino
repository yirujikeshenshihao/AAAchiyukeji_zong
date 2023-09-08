void setup() {
  Serial.begin(9600);

}
void loop() {
  int val = analogRead(A0);
  float temp = val / 4.235;
  float Voltage = (temp / 10) / 1.0;
  Serial.println(Voltage);
  delay(1000);
}
