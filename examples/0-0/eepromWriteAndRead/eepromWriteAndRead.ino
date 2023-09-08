#include <EEPROM.h>

struct MyObject {   //建立要进行存储的函数结构体
  int field1;
  byte field2;

};
MyObject customVar; //建立存储结构体
void setup() {

  Serial.begin(9600);

customVar.field1=96;
customVar.field2=65;


  eepromWriteData();  //存储结构体
customVar.field1=0;
customVar.field2=0;
  secondTest();    //读取结构体

}
void loop() {
  /* Empty loop */
}

/*********************************************
  函数名：eepromWriteData
  功  能: EEPROM读取地址内容
  形  参：无
  返回值：无
**********************************************/
void eepromWriteData() {
  EEPROM.put(0, customVar);  //存储结构体，从地址0开始，存储结构体
  Serial.println("存储完成");
}


/*********************************************
  函数名：eepromReadData
  功  能: EEPROM读取地址内容
  形  参：无
  返回值：无
**********************************************/
void secondTest() {
  EEPROM.get(0, customVar);
  Serial.println("读取完成");
}
