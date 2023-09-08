
#include <Wire.h>
#define TCAADDR 0x70           // 设置IIC扩展模块地址
void setup(void) {
  Wire.begin();
  setIicChannel(0);
}

void loop() {
}

/*********************************************
  函数名：setIicChannel
  功  能：设置IIC扩展模块通讯通道
  形  参：_data 要设置的通道
  返回值：无
**********************************************/
void setIicChannel(byte _data)
{
  if (_data > 7)
    return;
  Wire.beginTransmission(TCAADDR);
  Wire.write(1 << _data);
  Wire.endTransmission();
}
