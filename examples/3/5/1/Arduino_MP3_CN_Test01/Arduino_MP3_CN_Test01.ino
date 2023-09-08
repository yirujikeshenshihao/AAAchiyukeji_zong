/**
    控制播放音乐
*/


#include "Arduino.h"
#include "SoftwareSerial.h"
#include "DFRobotDFPlayerMini.h"
#define mp3Volume 25   //MP3音量

#define mySoftwareRX    3  //接MP3的RX
#define mySoftwareTX    2//接MP3的TX
SoftwareSerial mySoftwareSerial(mySoftwareRX, mySoftwareTX);          // // RX, TX


//#define mySoftwareSerial Serial2
DFRobotDFPlayerMini myDFPlayer;

void setup() {
  Serial.begin(9600);
  delay(3000);

  mySoftwareSerial.begin(9600);
  if (!myDFPlayer.begin(mySoftwareSerial)) {             //使用softwareSerial与mp3通信。
    Serial.println(F("无法开始："));
    Serial.println(F("1.请重新检查连接！"));
    Serial.println(F("2.请插入SD卡！"));
    while (!myDFPlayer.begin(mySoftwareSerial)) {
      delay(500);
    }
  }
  Serial.println(F("启动成功"));
  Serial.println(F("串口输入a为上一首，b为下一首，c为暂停，d为继续播放，+为音量增加，-为音量减小，e为文件夹一音乐随机，f为文件夹二音乐随机，g为文件夹三音乐随机，s为关闭音乐"));

  //  myDFPlayer.enableLoopAll();
  myDFPlayer.volume(25);  //Set volume value. From 0 to 30
  //  myDFPlayer.loop(3);
  //  myDFPlayer.play(1);  //Play the first mp3
}

void loop() {
  String readData = "";                               //写入串口数据存储值
  if (Serial.available() > 0) {
    while (Serial.available() > 0) {
      readData += char(Serial.read());
      delay(2);
    }

    switch (readData[0]) {
      case 'a': {
          Serial.println("上一首");
          myDFPlayer.previous();
        } break;
      case 'b': {
          Serial.println("下一首");
          myDFPlayer.next();
        } break;
      case 'c': {
          Serial.println("暂停");
          myDFPlayer.pause();
        } break;
      case 'd': {
          Serial.println("继续播放");
          myDFPlayer.start();
        } break;
      case '+': {
          Serial.println("声音增加");
          myDFPlayer.volumeUp();
          Serial.println("当前音量："); Serial.println(myDFPlayer.readVolume());
        } break;
      case '-': {
          Serial.println("声音减小");
          myDFPlayer.volumeDown();
          Serial.println("当前音量："); Serial.println(myDFPlayer.readVolume());
        } break;
      case 's': {
          Serial.println("关闭播放");
          myDFPlayer.stop();
          Serial.print("当前音乐播放状态："); Serial.println("关闭");
        } break;

      default: {
          Serial.println("字符无效");
          break;
        }
    }
  }

  //                 文件夹号  歌曲序号
  //  myDFPlayer.playFolder(2,3);
}
