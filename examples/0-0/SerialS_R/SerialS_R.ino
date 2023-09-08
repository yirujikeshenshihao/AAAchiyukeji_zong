void setup() {
  Serial.begin(115200);
  Serial2.begin(115200);
  Serial.println("初始化完成");
}
void loop() {

  if (Serial2.available() > 0) {
    byte inByte = Serial2.read();
    Serial.print(inByte,HEX);
    Serial.print(" ");    
  }

}
