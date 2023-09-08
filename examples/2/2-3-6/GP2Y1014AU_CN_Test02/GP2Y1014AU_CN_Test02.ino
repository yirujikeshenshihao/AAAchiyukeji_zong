
/*GP2Y1014AU传感器*/
#define dustPin A0
#define ledPower 2
#define Pm2_5Time 1000  //读取PM2.5的时间
int Pm2_5 = 0;          //存储PM2_5的值

void setup() {
  Serial.begin(9600);
  pinMode(ledPower, OUTPUT);
  pinMode(dustPin, INPUT);
}

void loop() {
  readPmVal();    //获取PM2.5的值
}

/*********************************************
  函数名：readPmVal
  功  能：读取PM2.5的值
  形  参：_time 设置间隔读取的时间【单位，ms】
  返回值：无
**********************************************/
void readPmVal(unsigned long _time) {
  static uint32_t lastTime = 0;
  if (millis() - lastTime >= _time) {
    lastTime = millis();
#define delayTime 280
#define delayTime2 40
#define offTime 9680

    float dustVal = 0;
    float dustVot = 0;
    float dustDensity = 0;

    // ledPower is any digital pin on the arduino connected to Pin 3 on the sensor
    digitalWrite(ledPower, LOW);
    delayMicroseconds(delayTime);
    dustVal = analogRead(dustPin);
    delayMicroseconds(delayTime2);
    digitalWrite(ledPower, HIGH);
    delayMicroseconds(offTime);




    dustVot = dustVal * 0.0049;  //  将ADC输出值转换为模拟电压值
    /* 
 * 将采集到的电压值转换为粉尘浓度
 * 公式：浓度=[传感器输出模拟电压 * 0.1667 - 0.1] * 1000;
 * 公式根据根据官方规格书中的《Fig. 3 Output Voltage vs. Dust Density》图表计算得出
 * 公式未经过校准，仅为理论计算值
 * 得出粉尘的浓度，单位为微克每立方米  
 * 参考链接 https://www.cnblogs.com/sangko/articles/14091777.html
 */
    dustDensity = ((dustVot * 0.1667) - 0.1) * 1000;

    //  将粉尘的浓度转换为PM2.5指数
    //  参考标准为PM2.5中国标准
    if (dustDensity <= 0) {
      Pm2_5 = 0;
    } else if (dustDensity < 35) {
      Pm2_5 = dustDensity * 1.4286;
    } else if (dustDensity < 150) {
      Pm2_5 = (dustDensity - 35) * 1.25 + 50;
    } else if (dustDensity < 250) {
      Pm2_5 = (dustDensity - 150) * 1 + 200;
    } else Pm2_5 = (dustDensity - 250) * 0.8 + 300;
    Serial.print("Pm2.5：");
    Serial.println(Pm2_5);
  }
}
