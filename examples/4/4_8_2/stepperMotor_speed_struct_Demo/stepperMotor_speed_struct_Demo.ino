/*
电机转动一圈 是3200脉冲
*/

//#define firenPin  //启用EN引脚

#define dirBack LOW          // 反方向电平
#define dirForward !dirBack  // 正方向电平

#define enON LOW     // 开启 驱动芯片 工作
#define enOFF !enON  // 关闭 驱动芯片 工作

int dirPin[] = { 3, 5V};  // 方向引脚           方向  方向控制，低电平正转，高电平反转。
int stePin[] = { 2, 4};  // 脉冲控制引脚        脉冲输入，往这个脚输入一个方波，电机转动一步，也就是(1.8/16)°（以1.8°电机，16细分为例），往这个脚持续输入方波，则电机持续转动。
//int enPin[] = { 4 };   // 使能引脚           使能，接低电平则模块开始工作，接高电平则模块关机。    如果不需要EN引脚则关闭

int stepperNum = sizeof(dirPin) / sizeof(dirPin[0]);  //通过设置的引脚确定使用的步进电机数量
struct Stepper {
  unsigned long PulseWidthMicros = 12;  // 脉冲间隔 us   //最小值为12
  bool Nowdir = dirForward;               // 驱动方向
  bool Systemstart = true;                // 启动
  bool Steflag = LOW;                     //存储当前的脉冲状态
  unsigned long Time = 0;                 //存储脉冲时间戳
  unsigned long PulseCount = 0;           // 记录的脉冲数量
  unsigned long lastPulseCount = 0;       // 记录上一次的脉冲数量
};
Stepper steppers[sizeof(dirPin) / sizeof(dirPin[0])];  //创建步进电机结构体

void setup() {
  Serial.begin(9600);                // 串口初始化,波特率设置为9600
  initDriver();                      // 步进电机控制引脚初始化
  Serial.println("\nSystem Start");  // 串口打印系统启动
}

void loop() {
  setStepperRun();
  // if (steppers[1].PulseCount > 3200) {
  //   steppers[0].Systemstart = false;
  // }
}
/*********************************************
  函数名：setStepperRun
  功  能: 设置步进电机运行方向
  形  参：_Time 间隔时间， 【单位：毫秒】
  返回值：无
**********************************************/
void setStepperRun() {
  for (int i = 0; i < stepperNum; i++) {
    if (steppers[i].Systemstart == true) {
      ctrlStepper1(i, stePin[i], dirPin[i], steppers[i].Nowdir);
    }
  }
}

void ctrlStepper1(int _index, int _stepP, int _dirP, bool _dirState) {  //控制步进电机（dirPin引脚，方向）
  digitalWrite(_dirP, _dirState);                                       //配置方向
  if (micros() - steppers[_index].Time > steppers[_index].PulseWidthMicros) {
    steppers[_index].Time = micros();
    digitalWrite(_stepP, steppers[_index].Steflag = !steppers[_index].Steflag);
    if (!steppers[_index].Steflag) {
      steppers[_index].PulseCount++;
    }
  }
}


// 步进电机控制引脚初始化
void initDriver() {
  for (int i = 0; i < stepperNum; i++) {
    pinMode(dirPin[i], OUTPUT);  // dirPin 模式设置为OUTPUT
  }
  for (int i = 0; i < stepperNum; i++) {
    pinMode(stePin[i], OUTPUT);  // stepPIN 模式设置为OUTPUT
  }
#ifdef firenPin
  for (int i = 0; i < stepperNum; i++) {
    pinMode(enPin[i], OUTPUT);  // enPin 模式设置为OUTPUT
  }

  for (int i = 0; i < stepperNum; i++) {
    digitalWrite(enPin, enON);  // 驱动 关闭
  }
#endif
}
