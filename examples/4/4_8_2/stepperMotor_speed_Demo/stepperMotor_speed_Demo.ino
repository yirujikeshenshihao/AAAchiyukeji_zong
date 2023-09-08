

#define dirPin 5   // 方向引脚           方向  方向控制，低电平正转，高电平反转。
#define stepPin 2  // 脉冲控制引脚        脉冲输入，往这个脚输入一个方波，电机转动一步，也就是(1.8/16)°（以1.8°电机，16细分为例），往这个脚持续输入方波，则电机持续转动。
const int ydirPin = 6;     // y方向控制引脚
const int ystepPin = 3;    // y步进控制引脚
const int zdirPin = 7;     // z方向控制引脚
const int zstepPin = 4;    // z步进控制引脚

#define enPin 8    // 使能引脚           使能，接低电平则模块开始工作，接高电平则模块关机。

#define dirBack LOW          // 反方向电平
#define dirForward !dirBack  // 正方向电平

#define enON LOW     // 开启 驱动芯片 工作
#define enOFF !enON  // 关闭 驱动芯片 工作

int pulseWidthMicros = 10;  // 脉冲间隔 us

bool nowdir = dirForward;     // 驱动方向
bool systemStart = false;  // 启动

void setup() {

  Serial.begin(9600);  // 串口初始化,波特率设置为9600
  initDriver();        // 步进电机控制引脚初始化
  digitalWrite(dirPin, dirForward);  // 配置电机方向
  Serial.println("\nSystem Start");  // 串口打印系统启动
  digitalWrite(enPin, enON);         // 驱动打开
}

void loop() {
  setStepperRun(dirBack);
}
/*********************************************
  函数名：setStepperRun
  功  能: 设置步进电机运行方向
  形  参：_time 间隔时间， 【单位：毫秒】
  返回值：无
**********************************************/
void setStepperRun(bool _dir) {
  systemStart = true;  //运行步进电机运行
  nowdir = _dir;       //设置步进电机运行方向

  ctrlStepper1(stepPin, dirPin, nowdir);
  ctrlStepper2(ystepPin, ydirPin, nowdir);
  ctrlStepper3(zstepPin, zdirPin, nowdir);
  
}

void ctrlStepper1(int stepP, int dirP, bool dirState) {  //控制步进电机（dirPin引脚，方向）
  digitalWrite(dirP, dirState);                         //配置方向
  static unsigned long time = 0;
  static bool flag = false;
  if (micros() - time > pulseWidthMicros) {
    time = micros();
    digitalWrite(stepP, flag = !flag);
  }
}
void ctrlStepper2(int stepP, int dirP, bool dirState) {  //控制步进电机（dirPin引脚，方向）
  digitalWrite(dirP, dirState);                         //配置方向
  static unsigned long time = 0;
  static bool flag = false;
  if (micros() - time > pulseWidthMicros) {
    time = micros();
    digitalWrite(stepP, flag = !flag);
  }
}
void ctrlStepper3(int stepP, int dirP, bool dirState) {  //控制步进电机（dirPin引脚，方向）
  digitalWrite(dirP, dirState);                         //配置方向
  static unsigned long time = 0;
  static bool flag = false;
  if (micros() - time > pulseWidthMicros) {
    time = micros();
    digitalWrite(stepP, flag = !flag);
  }
}

// 步进电机控制引脚初始化
void initDriver() {
  pinMode(enPin, OUTPUT);      // enPin 模式设置为OUTPUT
  pinMode(stepPin, OUTPUT);    // stepPIN 模式设置为OUTPUT
  pinMode(dirPin, OUTPUT);     // dirPIN 模式设置为OUTPUT
    pinMode(ystepPin,OUTPUT);     // Arduino控制A4988y步进引脚为输出模式
  pinMode(ydirPin,OUTPUT);      // Arduino控制A4988y方向引脚为输出模式
  pinMode(zstepPin,OUTPUT);     // Arduino控制A4988z步进引脚为输出模式
  pinMode(zdirPin,OUTPUT);      // Arduino控制A4988z方向引脚为输出模式
  digitalWrite(enPin, enOFF);  // 驱动 关闭
}
