/*
   噪声检测模块环境噪音声压监测工业级扬尘系统串行分贝传感器
  Arduino-------DB
  3.3V-------------VCC
  GND-----------GND
  2-----------TXD
  3-----------RXD
  串口波特率默认 115200
  淘宝链接：
  https://item.taobao.com/item.htm?id=571134655438
*/
#include <SoftwareSerial.h>

#define   DecibelModeTx  2                                      //噪声模块引脚
#define   DecibelModeRx  3
#define   DecibelTime    500                                    //多长时间获取一次声音值 【单位：ms】
//float Decibel_data = 0;                                       //全局储存分贝值
uint8_t Decibel_Read[] = {0x01, 0x02, 0x00, 0x00, 0x00, 0x00, 0x78, 0x0A}; //获取分贝指令
SoftwareSerial DBserial(DecibelModeTx, DecibelModeRx);            // 建立SoftwareSerial对象，MCU's sRX引脚11, MCU's sTX引脚12 先使用工具修改波特率
void setup() {
  Serial.begin(115200);                                       //串口初始化
  DBserial.begin(115200);                                     //分贝传感器软串口初始化 先使用工具修改波特率
  Serial.println("DBserial started at 115200");

}
void loop() {
  decibelEvent(DecibelTime, decibel_callback);  //获取分贝的函数，间隔DecibelTime获取一次分贝值，获取后decibel_callback处理
}

/*函数作用：获取声音数据的事件函数
 *函数入口参数：
 *-----------timer： 多长时间获取一次分贝数据
 *-----------voidmo：获取到数据后的回调函数
 *返回值：无
*/
void decibel_callback(float decibelValue) {
  if (decibelValue != 0) {
    Serial.print("dB:");
    Serial.println(decibelValue);                             //打印分贝值
  }

}
/*函数作用：获取声音数据的事件函数
 *函数入口参数：
 *-----------timer： 多长时间获取一次分贝数据
 *-----------voidmo：获取到数据后的回调函数
 *返回值：无
*/

void decibelEvent(long timer , void voidmo(float)) {          //声音获取数值后发生事件函数
  static unsigned long lastTimer = 0;
  void(*pf)(float);                                      //创建函数指针，
  pf = voidmo;                                         //指向发生事件后的函数

  if (millis() - lastTimer >= timer) {                               //每400ms更新一次分贝数据
    lastTimer = millis();
    DBserial.write(Decibel_Read, sizeof(Decibel_Read) / sizeof(Decibel_Read[0]));
     
    pf(Get_Decibel());                                           //参数回调
  }
}

/*   读取分贝值函数   输入值：无   返回值：分贝大小   */
float Get_Decibel() {
  uint8_t data[100] = {0};
  int index = 0;
  while (DBserial.available()) {
    data[index++] = DBserial.read();                             //将返回值存入data数组
    delay(2);
  }
  if (data[2] == 0x00) { //00：表示成功
    if (((data[6] << 8) | data[7]) == GetCrcData(data, 6)) {      //验证数据是否准确，也就是是否符合CRC
      return (data[3] + data[4] * 256) / 10.0;
    }
    else {
      return 0;
      Serial.println("GetCrcData error!");
    }
  }
  else {
    return 0;
    Serial.println("Data error!");
  }
}
/*   CRC校验函数   输入值：data,长度   返回值：校验数据   */
uint16_t GetCrcData(uint8_t *buffer, uint32_t len) {
  uint16_t wcrc = 0XFFFF;                                           //16位crc寄存器预置
  uint8_t temp;
  uint32_t i = 0, j = 0;                                            //计数
  for (i = 0; i < len; i++) {                                       //循环计算每个数据
    temp = *buffer & 0X00FF;                                        //将八位数据与crc寄存器亦或
    buffer++;                                                       //指针地址增加，指向下个数据
    wcrc ^= temp;                                                   //将数据存入crc寄存器
    for (j = 0; j < 8; j++) {                                       //循环计算数据的
      if (wcrc & 0X0001) {                                          //判断右移出的是不是1，如果是1则与多项式进行异或。
        wcrc >>= 1;                                                 //先将数据右移一位
        wcrc ^= 0XA001;                                             //与上面的多项式进行异或
      }
      else {                                                        //如果不是1，则直接移出
        wcrc >>= 1;                                                 //直接移出
      }
    }
  }
  uint8_t CRC_L;                                                    //定义数组
  uint8_t CRC_H;
  CRC_L = wcrc & 0xff;                                              //crc的低八位
  CRC_H = wcrc >> 8;                                                //crc的高八位
  return ((CRC_L << 8) | CRC_H);
}
