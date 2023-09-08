#include "Adafruit_VL53L0X.h"

#define distanceTime  100   // 设置获取一次距离的时间
#define distanceMax   500   //距离的最大值 
#define distanceMin   35   //距离的最小值 
int  distance = 0;  //存储距离值

Adafruit_VL53L0X lox = Adafruit_VL53L0X(); //实例化对象
VL53L0X_RangingMeasurementData_t measure; //建立数据接收结构体
void setup() {
  delay(50);
  Serial.begin(9600);
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while (1);
  }
  Serial.println(F("VL53L0X API Simple Ranging example\n\n"));
}


void loop() {

  obtainLaserDistance(distanceTime);  //获取距离值
}
/*********************************************
  函数名：obtainLaserDistance
  功  能：获取激光距离
  形  参：_time 设置获取距离的时间 【单位：毫秒】
  返回值：无
**********************************************/
void obtainLaserDistance ( unsigned long _time ) {
  static unsigned long  lastTime = 0; //建立静态局部变量，存储上次的时间值，系统断电后此值归零
  if (millis() - lastTime >= _time) { //判断当前时间-上次时间>=采样时间
    lastTime = millis(); //更新时间
    lox.rangingTest(&measure, false); //传入“true”以获得调试数据打印输出！
    if (measure.RangeStatus != 4) {  //相位故障的数据不正确
      Serial.print("Distance (mm): ");

      distance = constrain(measure.RangeMilliMeter, distanceMin, distanceMax);  //将获取的距离值锁定在一定范围
      Serial.println(distance);
    } else {
      Serial.println(" out of range ");
    }
  }
}

