/**************要删除的***********/
/*      获取遥杆传感器值 miiis()+指针入口传入全局变量
        获取遥杆传感器值并串口输出
*/
#include "AAAchiyukeji_zong.h"
AAAchiyukeji_zong AAAchiysukejfi_zong;
/*********************************/
#define rockerPinX   A0          //遥杆传感器X方向引脚   
#define rockerPinY   A1          //遥杆传感器Y方向引脚   
#define rockerPinK   2           //遥杆传感器按键引脚   


/*=================您可以修改的参数====================*/
#define rockerThreshold  600    //遥杆阈值
#define rockerUpThreshold   800 //遥杆的上限阈值 
#define rockerDownThreshold   200 //遥杆的下限阈值 
#define getrockerTime       20   //多长时间获取一次遥杆值【单位：ms】
/******************不需要修改的参数*********************/
int rockerXVal;  //存储获取的遥杆X AD值
int rockerYVal;  //存储获取的遥杆Y AD值


/*----------------------- 获取遥杆传感器值 miiis()+指针入口传入全局变量-------------------*/
void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  pinMode(rockerPinK, INPUT_PULLUP);
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
}

void loop() {
  getRockerValue(rockerPinX,rockerPinY,rockerPinK, getrockerTime, &rockerXVal, &rockerYVal); //获取遥杆模拟值
}
/*函数作用：间隔一定时间，读取模拟引脚数据
  函数入口参数：
  -----------x_pin：要读取的摇杆X引脚
  -----------y_pin：要读取的摇杆Y引脚
  -----------k_pin：要读取的摇杆按键引脚
  -----------x_val：传入用来存储遥杆X AD值
  -----------y_val：传入用来存储遥杆X AD值
  
  返回值：无
*/
void getRockerValue(int x_pin, int y_pin, int k_pin, unsigned long time, int *x_val, int *y_val ) { //获取模拟值函数
  static unsigned long  lastTime = 0; //建立静态局部变量，存储上次的时间值，系统断电后此值归零
  if (millis() - lastTime >= time) { //判断当前时间-上次时间>=采样时间
    lastTime = millis(); //更新时间
    *x_val = analogRead(x_pin); //读取模拟引脚
    *y_val = analogRead(y_pin); //读取模拟引脚
    Serial.print("遥杆X值：");              //串口打印
    Serial.print(*x_val);
    Serial.print(" 遥杆Y值：");              //串口打印
    Serial.println(*y_val);
  }

  if (!digitalRead(k_pin)) {                             //当检测到低电平时
    delay(50);                                         //延时消抖
    while (!digitalRead(k_pin));                         //当检测变为高电平，也就是按键松开时
    Serial.println("\t\t\t按键按下");
  }


}


/*----------------------- 获取遥杆传感器值并串口输出-------------------
  void setup() {
  Serial.begin(9600);                        //设置串口波特率
  pinMode(rockerPinK,INPUT_PULLUP);
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
  }
  void loop(){
  rockerXVal=analogRead(rockerPinX);         //读取X方向模拟值引脚
  rockerYVal=analogRead(rockerPinY);         //读取Y方向模拟值引脚

  Serial.print("遥杆X值：");              //串口打印
  Serial.print(rockerXVal);
  Serial.print(" 遥杆Y值：");              //串口打印
  Serial.print(rockerYVal);
  Serial.print(" 遥杆按键值：");           //串口打印
  Serial.println(digitalRead(rockerPinK));

  delay(500);
  }
*/
