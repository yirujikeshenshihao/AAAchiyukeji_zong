/*
 * 测试数据
 * 2A 1B 00 A5 2A D3 FF FF FF 34 00 00 00 00 00 4E E8 93 C2 61 D3 8E C2 00 00 00 00 00 00 9E 23
*/
// 定义结构体
struct SensorData {
  uint8_t sn;
  uint16_t Addr;
  int angle;
  int distance;
  uint16_t user_cmd;
  float F_Path_Power_Level;
  float RX_Level;
  uint16_t Acc_X;
  uint16_t Acc_Y;
  uint16_t Acc_Z;
};

// 定义一个指向结构体的指针
SensorData* sensorDataPtr;

void setup() {
  Serial.begin (9600); // 打开串口，设置波特率为9600
  sensorDataPtr = (SensorData*)malloc(sizeof(SensorData)); // 分配内存空间
}

void loop() {
  // 等待串口数据
  if (Serial.available() >0) {
      uint8_t  heart =  Serial.peek();   //读取串口中的下一个字节，但不消除
    if (heart == 0x2A) {  //判断数据头是否为数据固定头
    // 读取结构体数据
    Serial.readBytes((char*)sensorDataPtr, sizeof(SensorData));
     // 输出结构体数据
    Serial.print("Temperature: ");
    Serial.print(sensorDataPtr->sn, HEX);
    Serial.print(", Humidity: ");
    Serial.print(sensorDataPtr->angle);
    Serial.print(", Light Level: ");
    Serial.println(sensorDataPtr->distance);
    }
    else{
     while (Serial.available() > 0) {  //清除串口内容
     char cha = Serial.read();
      delay(2);
    }
     Serial.println("数据乱，丢掉");
    }
   
  }
}
