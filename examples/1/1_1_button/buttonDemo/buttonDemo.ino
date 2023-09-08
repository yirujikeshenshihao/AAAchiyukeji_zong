/**************要删除的***********/
/*      按键的测试程序，内置三种写法 
 *      按键结构体写法  
 *      按键延时消抖 串口发送单个字符
 *      按键事件发生 函数回调（软件延时消抖【阻塞式】）
*/
#include "AAAchiyukeji_zong.h"
AAAchiyukeji_zong AAAchiysukejfi_zong;
/*********************************/
/*=================您可以修改的参数====================*/
#define test  "testsing"

/******************不需要修改的参数*********************/

/*---------------------  按键结构体写法 ------------------------------
  #define buttonPin1  2   //绿色按键引脚
  #define buttonPin2  3   //红色按键引脚
  #define buttonPin3  4   //黄色按键引脚
  #define buttonPin4  5   //
  const int buttonPins[] = {buttonPin1, buttonPin2, buttonPin3, buttonPin4};                                   //将按键存储到数组
  bool buttonON[] = {LOW, LOW, LOW, LOW};                                                                      //按键生效的状态
  struct Button {
  int buttonState = !buttonON;                                                                               // 按钮状态变量，与按钮按下时的状态取反
  int lastButtonState = !buttonON;                                                                           // 按钮状态初始化，与按钮按下时的状态取反
  long lastDebounceTime = 0;                                                                                 // 记录抖动变量
  long debounceDelay = 30;                                                                                   // 抖动时间变量
  bool flag = false;                                                                                         // 按钮flag
  };
  Button buttons[8];                                                                                           // 创建按按键结构体
  void setup() {
  Serial.begin(9600);                                                                                        //串口9600初始化
  initButtons();                                                                                             //初始化所有按键
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
  }
  void loop() {
  for (int num = 0; num < sizeof(buttonPins) / sizeof(buttonPins[0]); num++) {
    getButton(buttonPins[num], num);                                                                         // 检测按键
  }
  }
  void initButtons() {
  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);                                                                    //适用于多个按钮
  }
  }

  void getButton(int _buttonPin, int _buttonIndex) {
  int reading = digitalRead(_buttonPin);                                                                     // 读取状态
  if (reading != buttons[_buttonIndex].lastButtonState) {                                                    // 如果状态改变
    buttons[_buttonIndex].lastDebounceTime = millis();                                                       // 更新时间
  }                                                                                                          // 如果等待大于debounceDelay
  if ((millis() - buttons[_buttonIndex].lastDebounceTime) > buttons[_buttonIndex].debounceDelay) {
    if (reading != buttons[_buttonIndex].buttonState) {                                                      // 读取状态不等于按钮状态
      buttons[_buttonIndex].buttonState = reading;                                                           // 更新状态
      if (buttons[_buttonIndex].buttonState == buttonON[_buttonIndex]) {                                     // 判断按钮是否真的按下
        Serial.print(_buttonIndex);                                                                          // 打印按钮编号
        Serial.println(" is pressed");                                                                       // 输出按钮按下的文字
        buttons[_buttonIndex].flag = true;                                                                   // 按钮flag为真
      }
      else {
        buttons[_buttonIndex].flag = false;                                                                  // 按钮flag为假
      }
    }
  }
  buttons[_buttonIndex].lastButtonState = reading;                                                           // 更新last状态
  }*/
  
  /*---------------------  按键结构体写法  单一按键状态 ------------------------------
#define buttonON    LOW                                                                   //按键生效的状态
#define buttonPin1  2   //总开按键引脚
#define buttonPin2  3   //风扇按键引脚
#define buttonPin3  4   //电阻按键引脚
#define buttonPin4  5   //电感按键引脚
#define buttonPin5  6   //电容按键引脚 
const int buttonPins[] = {buttonPin1, buttonPin2, buttonPin3, buttonPin4, buttonPin5};                                   //将按键存储到数组

struct Button {
  int buttonState = !buttonON;                                                                               // 按钮状态变量，与按钮按下时的状态取反
  int lastButtonState = !buttonON;                                                                           // 按钮状态初始化，与按钮按下时的状态取反
  long lastDebounceTime = 0;                                                                                 // 记录抖动变量
  long debounceDelay = 30;                                                                                   // 抖动时间变量
  bool flag = false;                                                                                         // 按钮flag
};
Button buttons[sizeof(buttonPins) / sizeof(buttonPins[0])];                                                                                           // 创建按按键结构体
void setup() {
  Serial.begin(9600);                                                                                        //串口9600初始化
  initButtons();                                                                                             //初始化所有按键
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成
}
void loop() {
  for (int num = 0; num < sizeof(buttonPins) / sizeof(buttonPins[0]); num++) {
    getButton(buttonPins[num], num);                                                                         // 检测按键
  }
}
void initButtons() {
  for (int i = 0; i < sizeof(buttonPins) / sizeof(buttonPins[0]); i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);                                                                    //适用于多个按钮
  }
}

void getButton(int _buttonPin, int _buttonIndex) {
  int reading = digitalRead(_buttonPin);                                                                     // 读取状态
  if (reading != buttons[_buttonIndex].lastButtonState) {                                                    // 如果状态改变
    buttons[_buttonIndex].lastDebounceTime = millis();                                                       // 更新时间
  }                                                                                                          // 如果等待大于debounceDelay
  if ((millis() - buttons[_buttonIndex].lastDebounceTime) > buttons[_buttonIndex].debounceDelay) {
    if (reading != buttons[_buttonIndex].buttonState) {                                                      // 读取状态不等于按钮状态
      buttons[_buttonIndex].buttonState = reading;                                                           // 更新状态
      if (buttons[_buttonIndex].buttonState == buttonON) {                                     // 判断按钮是否真的按下
        Serial.print(_buttonIndex);                                                                          // 打印按钮编号
        Serial.println(" is pressed");                                                                       // 输出按钮按下的文字
        buttons[_buttonIndex].flag = true;                                                                   // 按钮flag为真
      }
      else {
        buttons[_buttonIndex].flag = false;                                                                  // 按钮flag为假
      }
    }
  }
  buttons[_buttonIndex].lastButtonState = reading;                                                           // 更新last状态
}
  */

/*---------------------  按键延时消抖 串口发送单个字符-------------------*/
  #define button1 2                                     //设置引脚
  #define button2 3
  #define button3 4

  void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  pinMode(button1,INPUT_PULLUP);                       //设置引脚为上拉模式
  pinMode(button2,INPUT_PULLUP);
  pinMode(button3,INPUT_PULLUP);
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成


  }

  void loop() {
  portSendData(button1,'A');                           //当引脚触发时，串口发送对应的字符
  portSendData(button2,'B');
  portSendData(button3,'C');

  }

  void portSendData(int pin,char cha){
  if(!digitalRead(pin)){                               //当检测到低电平时
    delay(50);                                         //延时消抖
    while(!digitalRead(pin));                          //当检测变为高电平，也就是按键松开时
    Serial.print(cha);                                 //串口发送相应字符
  }
  }


/*---------------------  按键事件发生 函数回调（软件延时消抖【阻塞式】）------------

#define button1 2                                     //设置引脚
#define button2 3
#define button3 4

void setup() {
  Serial.begin(9600);                                  //设置串口波特率
  pinMode(button1, INPUT_PULLUP);                      //设置引脚为上拉模式
  pinMode(button2, INPUT_PULLUP);
  pinMode(button3, INPUT_PULLUP);
  Serial.println("Initialization complete 初始化完成");                                                    //串口打印初始化完成


}

void loop() {
  portSendData(button1, button1_callback);             //按键引脚绑定相应的回调函数
  portSendData(button2, button2_callback);
  portSendData(button3, button3_callback);

}

void portSendData(int pin, void voidmo(int)) {         //创建按键发生按下后的事件函数
  void(*pf)(int);                                      //创建函数指针，
  pf = voidmo;                                         //指向发生事件后的函数
  if (!digitalRead(pin)) {                             //当检测到低电平时
    delay(50);                                         //延时消抖
    while (!digitalRead(pin));                         //当检测变为高电平，也就是按键松开时
    pf(pin);                                           //参数回调
  }
}

void button1_callback(int pin) {
  Serial.println("按键1按下");

}
void button2_callback(int pin) {
  Serial.println("按键2按下");
}
void button3_callback(int pin) {
  Serial.println("按键3按下");
}
*/