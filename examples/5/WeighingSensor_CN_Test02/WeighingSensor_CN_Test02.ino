#include <HX711_ADC.h>

// 称重模块针脚-数据参数 定义
#define HX711Interval 100             // 称重模块检测间隔时间
#define weightMin     150             // 称重重量间隔小值大于150则为有手机
#define weightMax     500             // 称重重量间隔大值并且小于500为正常状态
#define hx711dout     3               // mcu > HX711 dout pin
#define hx711sck      4               // mcu > HX711 sck pin

// 宏定义针脚端口
HX711_ADC LoadCell(hx711dout, hx711sck);   // HX711构造函数：
unsigned long HX711Time = 0;               // 运行间隔时间

void setup() {
  Serial.begin(9600);                      // 串口波特率115200
  Serial.println("系统启动中...");
  LoadCell.setCalFactor(490.61);            // 设置校准系数（浮点）
  LoadCell.begin();
  Serial.println("初始化称重模块");
  LoadCell.tareNoDelay();
  Serial.println("初始化完成, 系统启动完成.");
}

void loop() {
  // HXT700 模块重量检测
  static bool newDataReady = 0;                // 定义是否有数据变化的布尔值
  if (LoadCell.update()) {                     // 检查新数据/启动下一个转换：
    newDataReady = true;
  }
  float weight;                                // 存储重量
  if (newDataReady) {
    if (millis() - HX711Time > HX711Interval) {
      HX711Time = millis();
      weight = LoadCell.getData();
      if ( weight < 0.5 ) {
        weight = 0;
      }
      Serial.print("重量："); Serial.print(weight); Serial.println("g");
    }
  }
}
