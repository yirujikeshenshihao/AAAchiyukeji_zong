#include <SoftwareSerial.h>       //使用软串口库
#include "DFRobotDFPlayerMini.h"
DFRobotDFPlayerMini mp3Player;
#define busyPin 7                 // 读取MP3播放状态引脚 用于循环播放音频
#define mp3TxPin 12                //UNO的9接MP3的RX
#define mp3RxPin 11                //UNO的8接MP3的TX
#define setVolume 25              //音量设置
SoftwareSerial mp3Serial(mp3RxPin, mp3TxPin);

void setup() {
  Serial.begin(9600);             // 串口初始化,波特率设置为9600
  pinMode(busyPin, INPUT);        // 引脚输入，播放时为低电平
  mp3Serial.begin(9600);
  mp3Player.begin(mp3Serial);   
  while (!mp3Player.begin(mp3Serial)) {                  //检查MP3模块是否启动成功
    Serial.println(F("Please recheck the connection"));  //串口打印请重新检查连接
    Serial.println(F("Please insert SD card"));          //串口打印请插入SD卡！
    mp3Player.begin(mp3Serial);
    delay(1);
  }
  mp3Player.volume(setVolume);     //设置音量，最大为30
  mp3Player.loop(1);
  Serial.println("System Start");  //系统正常启动
}

void loop() {







  
}


//指定播放第几首MP3音乐，和指定播放MP3播放的次数
void mp3PalyCount(int setSong, int setmp3Count) {
  static uint8_t mp3State = 0;
  static unsigned long mp3Time = 0;
  static int mp3Count = 0;  //MP3播放次数计算
  switch (mp3State) {
    case 0x00:
      {
        if (mp3Count < setmp3Count) {
          mp3Player.play(setSong);  // 播放音乐1
          mp3State++;               //状态++
          mp3Time = millis();       //记录时间
          Serial.println(mp3Count);
        }
      }
      break;
    case 0x01:
      {
        if (millis() - mp3Time > 800) {        //800ms为延时抖动，busy引脚在播放时不稳定，需等待800ms
          if (digitalRead(busyPin) == HIGH) {  //判断MP3是否播放完成
            Serial.println("over");
            mp3Count++;       //mp3播放次数+1
            mp3State = 0x00;  //状态归0
          }
        }
        // Serial.println(millis() - mp3Time);
      }
      break;
    case 0x02:
      break;
    default:
      break;
  }
}
