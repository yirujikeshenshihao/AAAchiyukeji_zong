#include <SoftwareSerial.h>
SoftwareSerial HC05Serial(10, 11); // 接HC05的TX, 接HC05的RX

#define Role        1     //1是主角色（Master），0是从角色（Slave） 
#define Name        "HC05"//1是主角色（Master），0是从角色（Slave） 
#define Pswd        1234  //配对密码
#define Mode        1     //0是指定蓝牙地址连接模式（指定蓝牙地址由绑定指令设置）。1是任意蓝牙地址连接模式（不受绑定指令设置地址的约束）。2是回环角色（Slave-Loop） 默认连接模式：0
#define Baudrate    9600  //蓝牙模块波特率
#define ReadTimeout 3000   //设置HC05返回的超时时间
uint8_t systemState = 0x00;     //系统状态机
unsigned long systemTime;       //系统当前时间


// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600);
  HC05Serial.begin(38400);
  Serial.println("请一直按住HC05模块的小按键！");
}

// the loop function runs over and over again forever
void loop() {
  switch (systemState) {
    case 0x00: {
        Serial.println("AT");
        HC05Serial.println("AT");
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x01: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("AT测试成功！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x00;
        }
      } break;
    case 0x02: {
        Serial.println(String("") + "AT+ROLE=" + Role);
        HC05Serial.println(String("") + "AT+ROLE=" + Role);
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x03: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("角色设置成功！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x02;
        }
      } break;
    case 0x04: {
        Serial.println(String("") + "AT+NAME=" + Name);
        HC05Serial.println(String("") + "AT+NAME=" + Name);
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x05: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("名称设置成功！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x04;
        }
      } break;
    case 0x06: {
        Serial.println(String("") + "AT+PSWD=" + Pswd);
        HC05Serial.println(String("") + "AT+PSWD=" + Pswd);
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x07: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("配对密码设置成功！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x06;
        }
      } break;
    case 0x08: {
        Serial.println(String("") + "AT+CMODE=" + Mode);
        HC05Serial.println(String("") + "AT+CMODE=" + Mode);
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x09: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("连接模式设置成功！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x08;
        }
      } break;
    case 0x0A: {
        Serial.println(String("") + "AT+UART=" + Baudrate + ",0,0");
        HC05Serial.println(String("") + "AT+UART=" + Baudrate + ",0,0");
        systemState++;                    //进入状态0x01
        systemTime = millis();          //记录开始等待2s的时间点
      } break;
    case 0x0B: {
        if (millis() - systemTime <= ReadTimeout) {//休息时间到达
          if (HC05Data().indexOf("OK") != -1) {
            Serial.println("波特率设置设置成功！");
            Serial.println("可以停止按住按键，您可以断电再接电后开始使用了！");
            systemState++;                    //进入状态0x02
          }
        }
        else {
          systemState = 0x0A;
        }
      } break;
    case 0x0C: {
        
      } break;
    default: break;
  }
}
String HC05Data() {
  String readData = "";        //接收HC05返回的数据
  if (HC05Serial.available() > 0) {
    while (HC05Serial.available() > 0) {
      readData += char (HC05Serial.read());   //串口接收成字符串
      delay(2);
    }
    Serial.print(readData);
    return readData;
  }
}
