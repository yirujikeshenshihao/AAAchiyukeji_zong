#include "RTClib.h"  //引入时钟模块的库

/*时钟相关宏定义*/
//#define nowTime                                 //设置时钟模块的时间为程序的下载时间，注销不设置时间
#define Timing 6    // 时间校正值  【单位：秒】
RTC_DS3231 ds3231;  //创建一个DS3231对象
/*****************串口打印*********************************/
#define printTime 1000  //串口打印时间 【单位：毫秒】
uint32_t printLastTime;
/**************************************************/
void setup() {
  delay(1000);
  Serial.begin(9600);
  if (!ds3231.begin()) {  // RTC初始化状态返回
    Serial.println(F("找不到RTC"));
  }
  delay(10);
  alarmSetInit();  //RTC设置闹钟设置
}

void loop() {
  if (millis() - printLastTime >= printTime) {
    printLastTime = millis();
    DS3231NowTime();
  }

  Serial1Port();
}

void alarmSetInit() {  //DS3231初始状态设置
#ifdef nowTime
  ds3231.adjust(DateTime(F(__DATE__), F(__TIME__)));
  DateTime now = ds3231.now();
  int newSecond = now.second() + Timing;
  int newMin = now.minute();

  if (newSecond >= 60) {
    newSecond %= 60;
    newMin++;
  }
  ds3231.adjust(DateTime(now.year(), now.month(), now.day(), now.hour(), newMin, newSecond));
#endif
  ds3231.disable32K();                 //我们不需要32K引脚，所以禁用它
  ds3231.clearAlarm(2);                //如果不这样做，这很容易导致问题，因为两个寄存器在重新启动/重新编译时都不会重置
  ds3231.writeSqwPinMode(DS3231_OFF);  //停止SQW引脚处的振荡信号 //否则，setAlarm1将失败
  ds3231.disableAlarm(2);              //关闭警报2（如果尚未关闭） //同样，这不是在重新启动时完成的，因此以前设置的警报很容易被忽略
}

void Serial1Port() {
  if (Serial.available() > 0) {
    char cha = Serial.read();
    if (cha == 'S') {
      DS3231CalibrationJudgment();  //通过串口设置DS3231时间
    } else {
    }
  }
}
//判断DS3231的时间，有没有判断校准时间
void DS3231CalibrationJudgment() {
  while (Serial.available() > 0) {

    //2022, 1, 22, 11, 30, 0, 0,
    // 年 , 月, 日, 时, 分, 秒,刷屏次数,
    uint16_t year = Serial.parseInt();
    uint8_t month = Serial.parseInt();
    uint8_t day = Serial.parseInt();
    uint8_t hour = Serial.parseInt();
    uint8_t min = Serial.parseInt();
    uint8_t sec = Serial.parseInt();
    while (Serial.available() > 0) {
      Serial.read();  //清空串口
      delay(2);
    }
    Serial.println(String(F("设置的RTC时间为：")) + year + "/" + month + "/" + day + " " + hour + ":" + min + ":" + sec);
    ds3231.adjust(DateTime(year, month, day, hour, min, sec));
  }
}

void DS3231NowTime() {          //获取DS3231现在的时间
  char tim[21] = "0";           //日期储存字符串
  DateTime now = ds3231.now();  //建立时间对象
                                // 格式化日期为指定的字符串格式
  snprintf(tim, sizeof(tim), "%02d-%02d-%02d %02d:%02d:%02d",
           now.year(), now.month(), now.day(), now.hour(), now.minute(), now.second());
  Serial.println(tim);
}
