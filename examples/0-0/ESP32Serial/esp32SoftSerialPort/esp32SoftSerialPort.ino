#define    cardTx  18      //定义硬件串口TX引脚
#define    cardRx  19      //定义硬件串口TX引脚
HardwareSerial cardSerial(1);//声明串口1
#define mySerial  cardSerial // 重新定义建立的串口名
void setup(){
  Serial.begin(9600);     //串口初始化
  cardSerial.begin(9600, SERIAL_8N1, cardTx, cardRx);
}

void loop(){
  if (mySerial.available()) {
    String str = mySerial.readString();
    Serial.println(str);
  }
  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
}
