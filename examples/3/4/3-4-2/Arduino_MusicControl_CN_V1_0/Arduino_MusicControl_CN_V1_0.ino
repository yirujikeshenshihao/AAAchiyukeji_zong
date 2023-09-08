#include <SoftwareSerial.h>            // 软串口

#define ARDUINO_RX   12                // 连接到串行 MP3 播放器模块的 TX
#define ARDUINO_TX   13                // 连接到串行 MP3 播放器模块的 RX
SoftwareSerial mySerial(ARDUINO_RX, ARDUINO_TX);  // 软串口
static int8_t Send_buf[8] = {0} ;      // 发送指令数组

// 设置MP3模块 调用设置指令集
#define SINGLE_CYCLE_ON  0X00
#define SINGLE_CYCLE_OFF 0X01
#define DEV_TF           0X02
#define CMD_PLAY_W_INDEX 0X03          //指定歌曲播放一次
#define CMD_SET_VOLUME   0X06
#define CMD_SEL_DEV      0X09
#define CMD_PLAY         0X0D          // 开启
#define CMD_PAUSE        0X0E          // 暂停
#define CMD_SINGLE_CYCLE 0X19
#define CMD_CYCLE_PLAY   0X08          // 循环指定歌曲播放

#define volumeSound      30            // 设置音量声音大小 0 ~ 30         
#define musicNumber      1             // 控制音乐编号 第几首

void setup() {
  Serial.begin(9600);                        // 设置波特率
  // ------------ MP3 ----------------
  mySerial.begin(9600);                      // 设置波特率
  sendCommand(CMD_SEL_DEV, DEV_TF);          // 设置SD卡
  sendCommand(CMD_SET_VOLUME, volumeSound);  // 设置音量大小 范围 0 ~ 30
  sendCommand(CMD_PAUSE, musicNumber);       // 暂停指定音乐
  // ---------------------------------
  Serial.println("Start successfully");      // 启动提示
}

void loop() {
  if (Serial.available() > 0) {                  // 判断串口如果有指令
    String receiveData = "";                     // 创建接收变量
    while (Serial.available() > 0) {             // 循环读取
      receiveData += char(Serial.read());        // 读取
      delay(2);                                  // 延迟2毫秒
    }
    Serial.print("Receive Data: ");              // 接收数据打印提示
    Serial.println(receiveData);                 // 打印提示
    // 根据不同指令判断进行不同操作
    if (receiveData[0] == '2') {                 // 循环播放音乐
      sendCommand(CMD_PLAY_W_INDEX, musicNumber);  // 循环播放指定音乐
    } else if (receiveData[0] == '3') {          // 关闭播放音乐
      sendCommand(CMD_PAUSE, musicNumber);       // 暂停指定音乐
    }
  }

  // loop回括号
}

// MP3 发送控制指令
void sendCommand(int8_t command, int16_t dat) {
  delay(20);
  Send_buf[0] = 0x7e;                 // 起始字节
  Send_buf[1] = 0xff;                 // 版本
  Send_buf[2] = 0x06;                 // 没有起始字节和结束字节的命令字节数
  Send_buf[3] = command;
  Send_buf[4] = 0x00;                 // 0x00 = 无反馈，0x01 = 反馈
  Send_buf[5] = (int8_t)(dat >> 8);   // 数据
  Send_buf[6] = (int8_t)(dat);        // 数据
  Send_buf[7] = 0xef;                 // 结束字节
  for (uint8_t i = 0; i < 8; i++) {
    mySerial.write(Send_buf[i]) ;
  }
}
