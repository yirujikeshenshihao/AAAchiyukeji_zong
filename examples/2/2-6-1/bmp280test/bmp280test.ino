
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_BMP280.h>

/*BMP280*/
#define bmpTime 1000        //获取气压间隔时间
Adafruit_BMP280 bmp;        // I2C
double bmpValue = 0.0;      //存储气压值【单位：kpa】
unsigned long bmpLastTime;  //记录时间戳
void setup() {
  Serial.begin(9600);
  /*BMP280初始化*/
  if (!bmp.begin(0x76)) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring or "
                     "try a different address!"));
    Serial.print("SensorID was: 0x");
    Serial.println(bmp.sensorID(), 16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("        ID of 0x60 represents a BME 280.\n");
    Serial.print("        ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /*操作模式*/
                  Adafruit_BMP280::SAMPLING_X2,     /*温度过采样*/
                  Adafruit_BMP280::SAMPLING_X16,    /*压力过采样*/
                  Adafruit_BMP280::FILTER_X16,      /*过滤*/
                  Adafruit_BMP280::STANDBY_MS_500); /*待机时间*/
}

void loop() {
  if (millis() - bmpLastTime > bmpTime) {  //获取气压值
    bmpLastTime = millis();                //更新时间
    bmpValue = bmp.readPressure() / 1000;  //除1000得到Kpa
        Serial.print(F("Pressure = "));
    Serial.print(bmpValue);
    Serial.println(" Pa");
  }
}
