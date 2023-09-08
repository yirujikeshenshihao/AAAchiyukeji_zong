
int sensorPin = A0;    // select the input pin for the potentiometer
#define acquisitionCycle       200    //采集周期
#define unusualNum    6  //原始数据异常次数
#define ifUnusuaTime  6000  //判定异常的时间

int newData = 0, oldData,  returnExceptionNum = 0;
unsigned long acquisitionLastTime = 0;
unsigned long abnormalPeriodLastTime = 0;
bool analogFlag = false;

void setup() {
  Serial.begin(9600);

}

void loop() {

getAnalogQuantity(sensorPin);





}

void getAnalogQuantity(int _pin) {

  if ((millis() - acquisitionLastTime) > acquisitionCycle) { //到达数据采集时间
    acquisitionLastTime = millis(); //更新下次的数据采集时间
    newData = analogRead(_pin);
    Serial.println(newData);
    if (abs(newData - oldData) > 3) {
      oldData = newData;
     // Serial.println("更新数据状态");
      returnExceptionNum++;
    }
  }
  if (millis() - abnormalPeriodLastTime > ifUnusuaTime) {
    abnormalPeriodLastTime = millis() ;
    if (returnExceptionNum > unusualNum) {
      returnExceptionNum = 0;
      Serial.println("       数据异常");
      analogFlag = false;
    }
    else {
      Serial.println("               数据正常");
      analogFlag = true;
    }
  }
}
