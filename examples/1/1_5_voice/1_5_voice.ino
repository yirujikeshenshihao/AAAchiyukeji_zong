/**************要删除的***********/
/*      获取声音传感器值 miiis()+指针入口传入全局变量
        获取声音传感器值并串口输出
*/
#include "AAAchiyukeji_zong.h"
AAAchiyukeji_zong AAAchiysukejfi_zong;
/*********************************/
#define voicePin   A0          //声音传感器引脚   
/*=================您可以修改的参数====================*/
#define voiceThreshold  600    //声音阈值
#define voiceUpThreshold   800 //声音的上限阈值 
#define voiceDownThreshold   200 //声音的下限阈值 
#define getVoiceTime       20   //多长时间获取一次声音值【单位：ms】
/******************不需要修改的参数*********************/
int voiceVal;  //存储获取的声音AD值

/*----------------------- 获取声音传感器值 miiis()+指针入口传入全局变量-------------------
void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成

}

void loop() {
  getVoiceValue(voicePin, getVoiceTime, &voiceVal); //获取声音模拟值
}
/*函数作用：间隔一定时间，读取模拟引脚数据
  函数入口参数：
  -----------pin：要读取的引脚
  -----------time：采样的间隔时间
  -----------val：采样值，先建立一个全局变量用以存放  
  返回值：无
*\/
void getVoiceValue(int pin, unsigned long time, int *val) { //获取模拟值函数
  static unsigned long  lastTime = 0; //建立静态局部变量，存储上次的时间值，系统断电后此值归零
  if (millis() - lastTime >= time) { //判断当前时间-上次时间>=采样时间
    lastTime = millis(); //更新时间
    *val = analogRead(pin); //读取模拟引脚
    // Serial.print("声音模拟值为："); //串口打印
    Serial.println( *val);
  }
}
*/

/*----------------------- 获取声音传感器值并串口输出-------------------*/
void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
}
void loop(){
  voiceVal=analogRead(voicePin);         //读取模拟值引脚
  Serial.print("声音值：");              //串口打印
  Serial.println(voiceVal);
  delay(20);  
}
