#include <OneWire.h>
#include <DallasTemperature.h>


/*DS18B20*/
#define oneWireBuspPin 19    // 定义DS18B20数据口连接IO
#define dsTime    1000   //定义获取DS18B20时间
float daTemp;   //存储温度值
OneWire oneWire(oneWireBuspPin); // 初始连接在单总线上的单总线设备
DallasTemperature sensors(&oneWire);

void setup(void)
{
  
  Serial.begin(9600);// 设置串口通信波特率
  sensors.begin(); // 初始库
}

void loop(void)
{
getDsTemp(dsTime);
}
/*********************************************
函数名：getDsTemp
功  能: 间隔一定时间获取DS18B20温度
形  参：_time 间隔时间， 【单位：毫秒】
返回值：无
**********************************************/
void getDsTemp(unsigned long _time) {
  static unsigned long lastTime = 0; //存储时间戳
  if (millis() > lastTime) {
    lastTime = millis() + _time; //更新时间
    sensors.requestTemperatures(); // 发送命令获取温度
    daTemp = sensors.getTempCByIndex(0); //获取温度
    Serial.print("Temperature for the device 1 (index 0) is: ");  //打印温度
    Serial.println(daTemp);
  }
}