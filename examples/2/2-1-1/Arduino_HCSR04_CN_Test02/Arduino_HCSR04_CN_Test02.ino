#include "SoftwareSerial.h"       //导入SoftwareSerial库
#include "DFRobotDFPlayerMini.h"  //导入DFRobotDFPlayerMini库

/*超声波*/
#define SR04Num 3                      //超声波数量
#define runIntervalTime 300            // 运行采集间隔时间
#define collectionIntervalCount 5      // 间隔差 次数
#define distanceIntervalValue 10       // 距离间隔差 cm
#define onTime 200                      //音量变化的时间
#define volumeChange 5                 //音量的变化量
#define volumeSetting 30               //音量大小设置(最大为30)
int TrigPin[SR04Num] = { 9, 11, 13 };  // 超声波Trig引脚
int EchoPin[SR04Num] = { 8, 10, 12 };  // 超声波Echo引脚
struct SR04 {


  bool RangeFlag = false;          //判断是否进入范围
  bool Flag = false;               //判断当前的状态 true ：在范围
  float newDistance = 200.0;       // 新数据 默认值
  float oldDistance = 200.0;       // 老数据 默认值
  float Value = 200.0;             //
  float RangeX = 10;               //范围的最小距离
  float RangeY = 80;               //范围的最大距离
  int volumeInit = volumeSetting;  //初始音量
  int volume = volumeInit;         //设置的音量
  int State = 0;                   //记录运行状态
  int collectionCount = 0;         // 距离间隔运行差次数
  unsigned long runTime = 0;       // 记录阶段运行时间
  unsigned long mosOnTime = 0;     // 记录音量变化的时间
};
SR04 sr04[SR04Num];

/*MP3*/
SoftwareSerial mp3One(2, 3);    //rx,tx                                                         //mega的13接MP3的TX，12接RX
SoftwareSerial mp3Two(4, 5);    //mega的11接MP3的TX，10接RX
SoftwareSerial mp3Three(6, 7);  //mega的51接MP3的TX，50接RX
DFRobotDFPlayerMini myDFPlayer[SR04Num];

void setup() {
  delay(2000);
  Serial.begin(9600);  // 设置当前波特率
  for (int i = 0; i < SR04Num; i++) {
    pinMode(TrigPin[i], OUTPUT);  // 设置引脚输出
    pinMode(EchoPin[i], INPUT);   // 设置引脚输入
  }
  delay(20);
  /*设置MP3*/
  mp3One.begin(9600);                               //设置1号MP3的串口波特率为9600
  while (!myDFPlayer[0].begin(mp3One)) {            //检查MP3模块是否启动成功
    Serial.println(F("1号MP3：请重新检查连接！"));  //串口打印请重新检查连接
    Serial.println(F("1号MP3：请插入SD卡！"));      //串口打印请插入SD卡！
    myDFPlayer[0].begin(mp3One);
  }
  Serial.println(F("1号MP3ok"));  //串口打印请重新检查连接
  delay(20);
  mp3Two.begin(9600);                               //设置2号MP3的串口波特率为9600
  while (!myDFPlayer[1].begin(mp3Two)) {            //检查MP3模块是否启动成功
    Serial.println(F("2号MP3：请重新检查连接！"));  //串口打印请重新检查连接
    Serial.println(F("2号MP3：请插入SD卡！"));      //串口打印请插入SD卡！
    myDFPlayer[1].begin(mp3Two);
  }
  Serial.println(F("2号MP3ok"));  //串口打印请重新检查连接
  delay(20);
  mp3Three.begin(9600);                             //设置3号MP3的串口波特率为9600
  while (!myDFPlayer[2].begin(mp3Three)) {          //检查MP3模块是否启动成功
    Serial.println(F("3号MP3：请重新检查连接！"));  //串口打印请重新检查连接
    Serial.println(F("3号MP3：请插入SD卡！"));      //串口打印请插入SD卡！
    myDFPlayer[2].begin(mp3Three);
  }
  Serial.println(F("3号MP3ok"));  //串口打印请重新检查连接

  for (int j = 0; j < SR04Num; j++) {     //将A0,A1,A2针脚设置为INPUT
    myDFPlayer[j].volume(volumeSetting);  //设置1号MP3音量大小
    delay(20);
  }
  for (int i = 0; i < 1; i++) {
    myDFPlayer[i].play(1);       // 播放0001.mp3
    myDFPlayer[i].loop(1);       // 循环播放
    myDFPlayer[i].enableLoop();  // 循环播放
    Serial.println("开始播放音乐1");
  }
  Serial.println(F("Started successfully"));
}

void loop() {
  for (int i = 0; i < SR04Num; i++) {
    srFilteredData(i);  //获取距离
  }
  for (int i = 0; i < SR04Num; i++) {
    if (sr04[i].RangeX < sr04[i].Value && sr04[i].Value < sr04[i].RangeY) {
      if (sr04[i].RangeFlag == false) {  //判断上一状态为未进入范围
        sr04[i].RangeFlag = true;        //更改为状态为进入范围
        sr04[i].Flag = true;             //范围触发
      }
    } else {
      sr04[i].RangeFlag = false;  //更改为状态为发生未进入范围
      sr04[i].Flag = false;       //范围未进入范围
    }
  }
  setMosOutput();
}
void setMosOutput() {
  for (int i = 0; i < SR04Num; i++) {
    switch (sr04[i].State) {
      case 0:
        if (sr04[i].Flag == true) {  //范围触发
          sr04[i].mosOnTime = millis();
          Serial.println("变换音量 ，音量减少, 准备播放音乐2");
          sr04[i].State++;  //进入下一个状态
        }
        break;

      case 1:
        if (millis() - sr04[i].mosOnTime >= onTime) {  //变化音量时间到达
          sr04[i].mosOnTime = millis();                //更新时间
          sr04[i].volume -= volumeChange;
          if (sr04[i].volume < 5) {  //当音量足够小时
            sr04[i].State++;         //进入下一个状态
            sr04[i].volume = 5;
          }
          Serial.println(String("音量降低为 ") + sr04[i].volume);
          myDFPlayer[i].volume(sr04[i].volume);
        }
        break;
      case 2:
        if (sr04[i].Flag == true) {
          Serial.println("开始播放音乐2");
          myDFPlayer[i].play(2);       // 播放0002.mp3
          myDFPlayer[i].loop(2);       // 循环播放
          myDFPlayer[i].enableLoop();  // 循环播放
        } else {
          myDFPlayer[i].play(1);       // 播放0001.mp3
          myDFPlayer[i].loop(1);       // 循环播放
          myDFPlayer[i].enableLoop();  // 循环播放
          Serial.println("开始播放音乐1");
        }

        sr04[i].State++;  //进入下一个状态
        break;
      case 3:
        if (millis() - sr04[i].mosOnTime >= onTime) {  //变化音量时间到达
          sr04[i].mosOnTime = millis();                //更新时间
          sr04[i].volume += volumeChange;
          if (sr04[i].volume >= sr04[i].volumeInit) {  //当音量恢复到初始音量时
            sr04[i].volume = sr04[i].volumeInit;
            if (sr04[i].Flag == true) {
              sr04[i].State++;  //进入下一个状态
            } else {
              sr04[i].State = 0;  //进入初始状态
            }
          }
          Serial.println(String("音量升高为 ") + sr04[i].volume);
              myDFPlayer[i].volume(sr04[i].volume);
        }
        break;
      case 4:
        if (sr04[i].Flag == false) {
          sr04[i].State = 1;  //进入下一个状态
          Serial.println(String("离开范围"));
        }
        break;
      default:
        sr04[i].State = 0;  //进入初始状态
        break;
    }
  }
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
    Serial.print("cm \t");
    if (_index == SR04Num - 1) {
      Serial.println();
    }
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
