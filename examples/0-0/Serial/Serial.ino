/**************要删除的***********/
/*
      可能需要用到的
 *    ******ESP32 蓝牙部分******
      #include "BluetoothSerial.h"
      BluetoothSerial SerialBT;

 *    ******ESP32 软串口部分******
      #include <SoftwareSerial.h>
      #define MYPORT_TX 12
      #define MYPORT_RX 13
      SoftwareSerial myPort;
      myPort.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
 *    ******软串口******
      #include <SoftwareSerial.h>
      #define MYPORT_TX 12
      #define MYPORT_RX 13
      SoftwareSerial mySerial(10, 11); // RX, TX
        mySerial.begin(9600);

*/
#include "AAAchiyukeji_zong.h"
AAAchiyukeji_zong AAAchiysukejfi_zong;
/*********************************/
/*=================您可以修改的参数====================*/
#define test  "testsing"

/******************不需要修改的参数*********************/

/*函数作用：间隔一定时间，读取模拟引脚数据
  函数入口参数：
  -----------pin：
  -----------pin：
  -----------pin：
  返回值：
*/

void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  Serial.println("初始化完成");                         //串口打印初始化完成
  /*串口相关函数
     if(Serial)
    available()
    availableForWrite()
    begin()
    end()
    find()
    findUntil()
    flush()
    parseFloat()
    parseInt()
    peek()
    print()
    println()
    read()
    readBytes()
    readBytesUntil()
    readString()
    readStringUntil()
    setTimeout()
    write()
    serialEvent()

  */

}

void loop() {

  if (SerialBT.available() > 0) {      //判断串口是否有内容
    char cha = SerialBT.read();        //读取首字符
    if (cha == 'H') {                  //判断首字符内容
      int heatTime = SerialBT.parseInt();  //赋值查找传入串口中的有效整数
      Serial.print("串口输入：");
      Serial.println(heatTime);
    } else if (cha == 'C') {
      coolTime = SerialBT.parseInt();      //赋值查找传入串口中的有效整数
      Serial.print("串口输入：");
      Serial.println(coolTime);
    }
    while (SerialBT.available() > 0) {  //清除串口内容
      cha = SerialBT.read();
      delay(2);
    }
  }


}
