#include <HX711_ADC.h>
#if defined(ESP8266)|| defined(ESP32) || defined(AVR)
#include <EEPROM.h>
#endif

#define hx711Dout 3 //mcu > HX711 dout pin
#define hx711Sck  4 //mcu > HX711 sck pin

HX711_ADC LoadCell(hx711Dout, hx711Sck);//HX711 constructor
#define calFlagAddr   0 //是否校准过的地址，绑定为0x55，未绑定为0x00
#define calValAddr    1 //校准后的系数值
unsigned long t = 0;
struct Sensor {                                             //定义一个储存重量数据的结构体
  float Now;                                                //当前重量的当前值
  long Period = 1000;                                       //获取当前重量的时间间隔 这里设置为1000ms
  float Limit = 1000.0;                                     //重量阈值
  long previousMillis;                                      //过去时间值
  bool flag = false;                                        //false为未触发，true为触发。
};
Sensor weight;
bool smsFalg = false;                                        //false为未触发，true为触发。
void setup() {
  Serial.begin(9600);
  Serial.println(F("初始化..."));
  LoadCell.begin();
  unsigned long stabilizingtime = 2000; // preciscion right after power-up can be improved by adding a few seconds of stabilizing time
  boolean _tare = true; //set this to false if you don't want tare to be performed in the next step
  LoadCell.start(stabilizingtime, _tare);
  if (LoadCell.getTareTimeoutFlag() || LoadCell.getSignalTimeoutFlag()) {
    Serial.println(F("超时, 检查HX711引脚"));
    while (1);
  }
  else {
#if defined(ESP8266)|| defined(ESP32)
    EEPROM.begin(512);
#endif
    if (EEPROM.read(calFlagAddr) == 0x55) {
      float calibrationValue;                        //校准值
      EEPROM.get(calValAddr, calibrationValue);
      LoadCell.setCalFactor(calibrationValue);
      //      Serial.println(calibrationValue);
    }
    else {
      LoadCell.setCalFactor(-427.81); // user set calibration value (float), initial value 1.0 may be used for this sketch
      while (!LoadCell.update());
      calibrate(); //start calibration procedure
      EEPROM.write(calFlagAddr, 0x55);//校准成功
    }
#if defined(ESP8266)|| defined(ESP32)
    EEPROM.commit();
#endif
    Serial.println(F("初始化完成"));
  }
}

void loop() {
  if (millis() - weight.previousMillis > weight.Period) {
    weight.previousMillis = millis();
    if (LoadCell.update() == true) {
      weight.Now = LoadCell.getData(); //单位g
      if (weight.Now < 5) {
        weight.Now = 0; //过滤掉0点浮动
      }
      Serial.print(F("当前垃圾重量(g)是: "));
      Serial.println(weight.Now);
      if (weight.Now > weight.Limit) {
        weight.flag = true;
        if (smsFalg == false) { //查询短信flag，只发一次

          Serial.println(F("短信报警"));
          smsFalg = true;
        }
      }
      else {
        smsFalg = false;//短信flag清除
      }
    }
  }
  // receive command from serial terminal
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 't') LoadCell.tareNoDelay(); //tare
    else if (inByte == 'r') calibrate(); //calibrate
    else if (inByte == 'c') changeSavedCalFactor(); //edit calibration value manually
  }
  // check if last tare operation is complete
  if (LoadCell.getTareStatus() == true) {
    Serial.println("Tare complete");
  }

  /*手动开启校准*/
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    if (inByte == 'a')calibrate();
  }

  //检查最后一个皮重操作是否已完成：
  if (LoadCell.getTareStatus() == true) {
    Serial.println("结束");
  }

}

void calibrate() {
  Serial.println(F("***"));
  Serial.println(F("开始校准"));
  Serial.println(F("请移除传感器上的物品，使其压力为零"));
  Serial.println(F("移除后在串口中输入英文字符  t  并回车"));
  boolean _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      if (Serial.available() > 0) {
        char inByte = Serial.read();
        if (inByte == 't') LoadCell.tareNoDelay();
      }
    }
    if (LoadCell.getTareStatus() == true) {
      Serial.println(F("去皮完成"));
      _resume = true;
    }
  }
  Serial.println(F("请在传感器上放上已知质量的物体"));
  Serial.println(F("然后输入其质量，例如华为mate 30 pro 5g是198.03，单位为g"));
  float known_mass = 0;
  _resume = false;
  while (_resume == false) {
    LoadCell.update();
    if (Serial.available() > 0) {
      known_mass = Serial.parseFloat();
      if (known_mass != 0) {
        Serial.print(F("已知质量为: "));
        Serial.println(known_mass);
        _resume = true;
      }
    }
  }

  LoadCell.refreshDataSet(); //refresh the dataset to be sure that the known mass is measured correct
  float newCalibrationValue = LoadCell.getNewCalibration(known_mass); //get the new calibration value

  Serial.print(F("新的系数值已设置为: "));
  Serial.print(newCalibrationValue);
  Serial.println(F("是（y）或否（n）使用这个值"));
  Serial.print(F("保存到EEPROM地址的"));
  Serial.print(calValAddr);
  Serial.println(F("? y/n"));

  _resume = false;
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calValAddr, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calValAddr, newCalibrationValue);
        Serial.print(F("Value "));
        Serial.print(newCalibrationValue);
        Serial.print(F(" 保存到EEPROM地址: "));
        Serial.println(calValAddr);
        _resume = true;

      }
      else if (inByte == 'n') {
        Serial.println(F("值不保存"));
        _resume = true;
      }
    }
  }

  Serial.println(F("End calibration"));
  Serial.println(F("***"));
  Serial.println(F("To re-calibrate, send 'r' from serial monitor."));
  Serial.println(F("For manual edit of the calibration value, send 'c' from serial monitor."));
  Serial.println(F("***"));
}

void changeSavedCalFactor() {
  float oldCalibrationValue = LoadCell.getCalFactor();
  boolean _resume = false;
  Serial.println(F("***"));
  Serial.print(F("Current value is: "));
  Serial.println(oldCalibrationValue);
  Serial.println(F("Now, send the new value from serial monitor, i.e. 696.0"));
  float newCalibrationValue;
  while (_resume == false) {
    if (Serial.available() > 0) {
      newCalibrationValue = Serial.parseFloat();
      if (newCalibrationValue != 0) {
        Serial.print(F("New calibration value is: "));
        Serial.println(newCalibrationValue);
        LoadCell.setCalFactor(newCalibrationValue);
        _resume = true;
      }
    }
  }
  _resume = false;
  Serial.print(F("Save this value to EEPROM adress "));
  Serial.print(calValAddr);
  Serial.println("? y/n");
  while (_resume == false) {
    if (Serial.available() > 0) {
      char inByte = Serial.read();
      if (inByte == 'y') {
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.begin(512);
#endif
        EEPROM.put(calValAddr, newCalibrationValue);
#if defined(ESP8266)|| defined(ESP32)
        EEPROM.commit();
#endif
        EEPROM.get(calValAddr, newCalibrationValue);
        Serial.print(F("Value "));
        Serial.print(newCalibrationValue);
        Serial.print(F(" saved to EEPROM address: "));
        Serial.println(calValAddr);
        _resume = true;
      }
      else if (inByte == 'n') {
        Serial.println(F("Value not saved to EEPROM"));
        _resume = true;
      }
    }
  }
  Serial.println(F("End change calibration value"));
  Serial.println(F("***"));
}
