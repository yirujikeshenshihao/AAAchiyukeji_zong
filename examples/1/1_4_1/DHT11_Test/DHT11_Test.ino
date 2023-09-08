#include "DHT.h"
/*DHT11*/
#define dhtPin 2       // 连接到DHT传感器的数字引脚
#define DHTTYPE DHT11  // DHT 11
#define dhtTime 1000   //获取DHT11温湿度的间隔时间【单位：毫秒】
DHT dht(dhtPin, DHTTYPE);
float tempVal, humiVal;
unsigned long dhtLastTime;  //记录时间戳
void setup() {
  Serial.begin(9600);
  /*DHT11初始化*/
  dht.begin();
}

void loop() {
  if (millis() - dhtLastTime > dhtTime) {  //获取温湿度
    dhtLastTime = millis();                //更新时间
    tempVal = dht.readTemperature();       //温度
    humiVal = dht.readHumidity();          //湿度
    Serial.print(F("Humidity: "));
    Serial.print(humiVal);
    Serial.print(F("%  Temperature: "));
    Serial.println(tempVal);
  }
}
