#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"
MAX30105 particleSensor;

const byte RATE_SIZE = 4; //数组大小，增加以实现更多的平均值，4比较好
byte rates[RATE_SIZE]; //心率数组
byte rateSpot = 0; //记录心率数组的位置
long lastBeat = 0; //上次心跳时间
float beatsPerMinute; //心率
int beatAvg; //平均心率

void setup()
{
  Serial.begin(115200);
  if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) { //使用默认的I2C端口，400kHz速度
    Serial.println("MAX30105未找到。请检查连线/电源。");
    while (1);
  }
  Serial.println("请用稳定的压力将食指放在传感器上。");
  particleSensor.setup(); //使用默认设置配置传感器
  particleSensor.setPulseAmplitudeRed(0x0A); //将红色LED亮度设为低以指示传感器正在运行
  particleSensor.setPulseAmplitudeGreen(0); //关闭绿色LED
}

void loop()
{
  long irValue = particleSensor.getIR(); //获取红外线数值

  if (checkForBeat(irValue) == true) //检测是否有心跳
  {
    //我们感应到心跳了！
    long delta = millis() - lastBeat;
    lastBeat = millis();
    beatsPerMinute = 60 / (delta / 1000.0); //计算心率

    if (beatsPerMinute < 255 && beatsPerMinute > 20)
    {
      rates[rateSpot++] = (byte)beatsPerMinute; //存储心率值
      rateSpot %= RATE_SIZE; //移动变量

      //取读数平均值
      beatAvg = 0;
      for (byte x = 0 ; x < RATE_SIZE ; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  Serial.print("IR="); //打印红外线数值
  Serial.print(irValue);
  Serial.print(", BPM="); //打印心率值
  Serial.print(beatsPerMinute);
  Serial.print(", Avg BPM="); //打印平均心率
  Serial.print(beatAvg);

  if (irValue < 50000)
    Serial.print(" 没有手指？"); //如果没有手指，打印此消息

  Serial.println();
}
