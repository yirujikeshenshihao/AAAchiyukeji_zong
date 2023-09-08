#define SR04Num 3                  //超声波数量
#define runIntervalTime 500        // 运行采集间隔时间
#define collectionIntervalCount 5  // 间隔差 次数
#define distanceIntervalValue 10   // 距离间隔差 cm


int TrigPin[SR04Num] = { 9, 11, 13 };  // 超声波Trig引脚
int EchoPin[SR04Num] = { 8, 10, 12 };  // 超声波Echo引脚
struct SR04 {
  float newDistance = 200.0;  // 新数据 默认值
  float oldDistance = 200.0;  // 老数据 默认值
  float Value = 200.0;        //
  int collectionCount = 0;    // 距离间隔运行差次数
  unsigned long runTime = 0;  // 记录阶段运行时间
};
SR04 sr04[SR04Num];
void setup() {
  Serial.begin(9600);  // 设置当前波特率
  for (int i = 0; i < SR04Num; i++) {
    pinMode(TrigPin[i], OUTPUT);  // 设置引脚输出
    pinMode(EchoPin[i], INPUT);   // 设置引脚输入
  }
  Serial.println(F("Started successfully"));
}

void loop() {
 // for (int i = 0; i < SR04Num; i++) {
    srFilteredData(2);  //获取距离
  //}
}

void srFilteredData(int _index) {
  if (millis() - sr04[_index].runTime >= runIntervalTime) {  // 判断时间间隔
    // 处理完成后，重置记录阶段时间
    sr04[_index].runTime = millis();                                                          // 记录阶段
    sr04[_index].newDistance = getDistanceData(TrigPin[_index], EchoPin[_index]);             // 将检测到数据存储于距离存储值下标0
    if (abs(sr04[_index].newDistance - sr04[_index].oldDistance) >= distanceIntervalValue) {  // 如果检测数据值大于上一次数据值间隔差
      if (++sr04[_index].collectionCount >= collectionIntervalCount) {                        // 超过五次则重置
        sr04[_index].oldDistance = sr04[_index].newDistance;                                  // 更新数据值
        sr04[_index].collectionCount = 0;                                                     // 运行次数重置
      }
    } else {                                                // 如果数据变化在固定范围内，则直接更新
      sr04[_index].oldDistance = sr04[_index].newDistance;  // 更新数据值
    }
    sr04[_index].Value = sr04[_index].oldDistance;  //更新数据
    Serial.print("Distance ");
    Serial.print(_index + 1);
    Serial.print(" : ");
    Serial.print(sr04[_index].oldDistance);
    Serial.println("cm");
  }
}

// 获取超声波距离数据 返回float数据类型 距离值 // 注意：超声波模块最大测试距离为  2cm~450cm
float getDistanceData(int TrigPin, int EchoPin) {
  float tempDistance;                                  // 临时存储变量
  digitalWrite(TrigPin, LOW);                          // 设置引脚低电平
  delayMicroseconds(2);                                // 延迟2微妙
  digitalWrite(TrigPin, HIGH);                         // 设置引脚高电平
  delayMicroseconds(10);                               // 延迟等待10微妙
  digitalWrite(TrigPin, LOW);                          // 设置引脚低电平
  tempDistance = pulseIn(EchoPin, HIGH) / 58.0;        // 公式计算距离
  tempDistance = (int(tempDistance * 100.0)) / 100.0;  // 转换为cm
  //  Serial.print("ShiJi: "); Serial.print(tempDistance); Serial.print(" ------ ");
  if (tempDistance < 0) {           // 判断检测距离数据小于0则
    tempDistance = 400;             // 等于400
  } else if (tempDistance > 400) {  // 判断检测距离数据大于400则等于400
    tempDistance = 400;             // 等于400
  }
  return tempDistance;  // 输出反馈距离
}
