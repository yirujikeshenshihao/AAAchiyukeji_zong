int dustPin=A0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
float dustVal=0;
float dustVot=0;
float dustDensity=0;
int Pm2_5 = 0;

void setup(){
Serial.begin(9600);
pinMode(ledPower,OUTPUT);
pinMode(dustPin, INPUT);
}
 
void loop(){
// ledPower is any digital pin on the arduino connected to Pin 3 on the sensor
digitalWrite(ledPower,LOW);
delayMicroseconds(delayTime);
dustVal=analogRead(dustPin);
delayMicroseconds(delayTime2);
digitalWrite(ledPower,HIGH);
delayMicroseconds(offTime);
 
delay(1000);

//  将ADC输出值转换为模拟电压值
dustVot = dustVal*0.0049;   

/* 
 * 将采集到的电压值转换为粉尘浓度
 * 公式：浓度=[传感器输出模拟电压 * 0.1667 - 0.1] * 1000;
 * 公式根据根据官方规格书中的《Fig. 3 Output Voltage vs. Dust Density》图表计算得出
 * 公式未经过校准，仅为理论计算值
 * 得出粉尘的浓度，单位为微克每立方米  
 * 参考链接 https://www.cnblogs.com/sangko/articles/14091777.html
 */
dustDensity = ((dustVot*0.1667)-0.1)*1000;  

//  将粉尘的浓度转换为PM2.5指数
//  参考标准为PM2.5中国标准
if (dustDensity <= 0) {
  Pm2_5 = 0;
  }
else if (dustDensity < 35) {
  Pm2_5 = dustDensity * 1.4286;
  }
else if (dustDensity < 150) {
  Pm2_5 = (dustDensity - 35)* 1.25 + 50 ;
  }
else if (dustDensity < 250) {
  Pm2_5 = (dustDensity - 150)* 1 + 200 ;
  }
else Pm2_5 = (dustDensity - 250)* 0.8 + 300 ;

Serial.print("Pm2.5：");Serial.println(Pm2_5);
}
