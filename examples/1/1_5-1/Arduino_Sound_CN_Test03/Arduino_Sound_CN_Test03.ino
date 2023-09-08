
// ----------------------------- 声音传感器 -------------------------------
#define soudPin         A0              // 读取声音传感器模拟量引脚
#define acquisitionTime 50             // 自适应采集时间
unsigned long startMillis = 0;          // 时间
int signalMax = 0;                      // 存储最大值
int signalMin = 1024;                   // 存储最小值
int sample;                             // 采样原始值
long runCount[2] = {0, 0};              // 运行次数
long triangleValue = 0;                 // 累加数
#define runMax 3                        // 判断是否开启持续次数
#define runMin 5                        // 判断是否关闭持续次数
int airflowValue = 0;                   // 气流大小值
// ----------------------------------------------------------------------

void setup() {
  Serial.begin(9600);                   // 设置波特率
  // --------------- 声音传感器 --------------------
  // 声音自适应获取当前 最小值 和 最大值
  startMillis = millis();               // 记录当前时间
  while (millis() - startMillis < acquisitionTime) {
    sample = analogRead(0);
    if (sample < 1024) {
      if (sample > signalMax) {
        signalMax = sample;
      }
      else if (sample < signalMin) {
        signalMin = sample;
      }
    }
  }
  // ---------------------------------------------

}


void loop() {
  // 获取吹气变化
  getAirflowChange();
  
}

// 获取气流大小  经过声音传感器3次处理
void getAirflowChange() {
  // 采集原始值
  sample = analogRead(soudPin);         // 获取原始数据值
  /*
    // 原始波形打印
    Serial.print(sample);
    Serial.print(",");
    Serial.print(signalMax);
    Serial.print(",");
    Serial.print(signalMin);
    Serial.print(",");
  */
  // 处理波形
  if (sample > signalMax) {             // 判断原始数据值大于采集最大值
    sample -= signalMax;                // 原始值减最大值
  } else if (sample < signalMin) {      // 判断原始数据值小于采集最大值
    sample = abs(signalMin - sample);   // 得到间隔差值
  } else {                              // 其它情况下
    sample = 0;                         // 等于0
  }
  /*
    // 正值表
    Serial.print(sample);
    Serial.print(",");
  */
  // 倒三角
  if (sample > 0) {                     // 判断数据是否大于0
    if (++runCount[0] > runMax) {       // 次数记录自增 如果 大于设定次数后
      runCount[1] = 0;                  // 重置值
      triangleValue += sample;          // 累加值
    }
  } else if (sample == 0) {             // 判断数据如果等于0
    if (++runCount[1] > runMin) {       // 次数记录自增 如果 大于设定次数后
      runCount[0] = 0;                  // 重置值
      runCount[1] = runMin;             // 设定值
      triangleValue = 0;                // 初始值
    }
  }

  // 获取幅度变化
  if (triangleValue != 0) {             // 如果不等于0
    airflowValue = triangleValue / runCount[0];   // 得到平均值
    Serial.print(airflowValue);         // 打印平均值
    Serial.print(",");
  } else {                              // 如果等于0
    airflowValue = 0;                   // 则等于0
    Serial.print(airflowValue);
    Serial.print(",");
  }
  // 打印触发累积量
  //  Serial.println(triangleValue);
  Serial.println();
}
