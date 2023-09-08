#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{
  /*LCD初始化*/
  lcd.init();     
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("humidity(%):");
}

int i=0;
void loop()
{
  
  lcdDisPlay(1000,2);
}

/*********************************************
  函数名：lcdDisPlay
  功  能: 显示屏刷新显示
  形  参：_time:屏幕刷新的时间；_val:要显示的值  
  返回值：无
**********************************************/
void lcdDisPlay(unsigned long _time, int _val) {
  static unsigned long lastTime = 0; //记录时间戳
  if (millis() - lastTime >= _time) {
    lastTime = millis();
    lcd.setCursor( 6, 1);
    lcd.print("   ");
    lcd.setCursor(6, 1);
    lcd.print(i++);
  }
}
