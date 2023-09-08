/**************要删除的***********/
/*      编码器中断方式触发
*/
#include "AAAchiyukeji_zong.h"
AAAchiyukeji_zong AAAchiysukejfi_zong;
/*********************************/
//中断1  IO2连接DT
//中断2  IO3连接SCK
#define encoderSW  4  //编码器encoderSW引脚
/*=================您可以修改的参数====================*/


/******************不需要修改的参数*********************/
long Position  =0;//存储编码器上一次的值
bool Modes=false;//触发标志  false:未触发；true：已触发
unsigned long  encoderLastTime;  //存储上一次按键编码器
void setup(){
  Serial.begin(9600);                                  //设置串口波特率
  pinMode(encoderSW,INPUT_PULLUP);                     //编码器按键引脚上拉
  attachInterrupt(1, A_inter, RISING); //开启中断 中断IO1 触发方式：上升沿 回调函数：A_inter
  attachInterrupt(0, B_inter, RISING); //开启中断 中断IO0 触发方式：上升沿 回调函数：B_inter
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
}
void loop(){
  portSendData(encoderSW, "按键按下\n");                          //当引脚触发时，串口发送对应的字符

}

/*函数作用：当某一按键的引脚按下时，串口打印相应的字符
 *函数入口参数：
 *-----------pin：按键的引脚
 *-----------cha：串口显示的字符
 *返回值：无
*/
void portSendData(int pin, String cha) {
  if (!digitalRead(pin)) {                             //当检测到低电平时
    delay(50);                                         //延时消抖
    while (!digitalRead(pin));                         //当检测变为高电平，也就是按键松开时
    Serial.print(cha);                                 //串口发送相应字符
  }  
}

/*函数作用：中断0发生时的回调函数
 *函数入口参数：无
 *返回值：无
*/
void A_inter(){ //中断函数A

  if (!Modes && millis() - encoderLastTime >= 20) {//消抖并判断哪个先触发 A|B
    Position++;               //数值自增
    Serial.println(Position); //串口打印
  }
  Modes = true;      //更改触发标志
  encoderLastTime = millis(); //记录触发时间
}
/*函数作用：中断1发生时的回调函数
 *函数入口参数：无
 *返回值：无
*/
void B_inter(){ //中断函数B

  if (Modes && millis() - encoderLastTime >= 20){ //消抖并判断哪个先触发 A|B
    Position--;               //数值自减
    Serial.println(Position); //串口打印
  }
  Modes = false;     //更改触发标志
  encoderLastTime = millis(); //记录触发时间
}
