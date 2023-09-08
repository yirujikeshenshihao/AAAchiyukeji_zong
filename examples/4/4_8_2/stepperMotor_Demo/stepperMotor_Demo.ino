

#define dirPin              2         // 方向引脚           方向  方向控制，低电平正转，高电平反转。                                    
#define stepPin             3         // 脉冲控制引脚        脉冲输入，往这个脚输入一个方波，电机转动一步，也就是(1.8/16)°（以1.8°电机，16细分为例），往这个脚持续输入方波，则电机持续转动。
#define enPin               4         // 使能引脚           使能，接低电平则模块开始工作，接高电平则模块关机。

#define dirBack             LOW        // 反方向电平   
#define dirForward          !dirBack   // 正方向电平      

#define enON                LOW        // 开启 驱动芯片 工作
#define enOFF               !enON      // 关闭 驱动芯片 工作

#define stepQuantity  300  //丝杆电机脉冲进量  【范围:0~300】

int stepTimeInterval = 20;    // ms
int pulseWidthMicros = 80;   // 脉冲间隔 us
int pulseQuantity = 25;   // 单次脉冲量

bool nowdir = dirBack;                 // 驱动方向
bool systemStart = false;              // 启动

unsigned long stepTime   = 0;          // 步进电机 时间间隔变量
long runCount = 0;                      // 运行数

void setup() {

  Serial.begin(9600);                  // 串口初始化,波特率设置为9600
  initDriver();                        // 步进电机控制引脚初始化

  digitalWrite(dirPin, dirForward);        // 配置电机方向
  Serial.println("\nSystem Start");      // 串口打印系统启动

}

void loop() {



  setStepperRun(dirForward);
  delay(1000);
  setStepperRun(!dirForward);
  delay(1000);
}
/*********************************************
  函数名：setStepperRun
  功  能: 设置步进电机运行方向
  形  参：_time 间隔时间， 【单位：毫秒】
  返回值：无
**********************************************/
void setStepperRun( bool _dir) {
  systemStart = true; //运行步进电机运行
  nowdir = _dir; //设置步进电机运行方向
  while (systemStart == true) {
    if (systemStart == true && millis() - stepTime >= stepTimeInterval) {     // 5ms 1次电机运行步数。
      stepTime = millis();
      digitalWrite(enPin, enON);                   // 驱动打开
      ctrlStepper(stepPin, dirPin, nowdir, pulseQuantity);
      Serial.println(++runCount);
      if (abs(runCount) >= stepQuantity) {
        digitalWrite(enPin, enOFF);                   // 驱动打开
        systemStart = false;
        runCount = 0;
        Serial.println("/*************************OFF************************/");
      }
      // digitalWrite(enPin, enOFF);               // 驱动关闭 不要关掉，关掉没有力量。
    }
  }
}

void ctrlStepper(int stepP, int dirP, bool dirState, unsigned long pulsesNum) {                             //控制步进电机（dirPin引脚，方向， 脉冲数）
  digitalWrite(dirP, dirState);                                                                                //配置方向
  for (unsigned long i = 0; i < pulsesNum; i++) {                                                             //给出脉冲
    digitalWrite(stepP, HIGH);
    delayMicroseconds(pulseWidthMicros);                                                                      //脉冲时间为 pulseWidthMicros * 2 微秒
    digitalWrite(stepP, LOW);
    delayMicroseconds(pulseWidthMicros);
  }
}


// 步进电机控制引脚初始化
void initDriver() {
  pinMode(enPin, OUTPUT);              // enPin 模式设置为OUTPUT
  pinMode(stepPin, OUTPUT);            // stepPIN 模式设置为OUTPUT
  pinMode(dirPin, OUTPUT);             // dirPIN 模式设置为OUTPUT
  digitalWrite(enPin, enOFF);          // 驱动 关闭
}
