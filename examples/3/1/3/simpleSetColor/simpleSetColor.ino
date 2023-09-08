

#include <Adafruit_NeoPixel.h>
/*WS2812显示*/
#define pixelsColor  0x0000ff   //设置显示的颜色
#define PIN        2     //设置引脚
#define NUMPIXELS 1   //设置数量
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);  //实例化对象
void setup() {
  /*WS2812初始化*/
  pixels.begin();   
  pixels.clear();
  pixels.show();
}

void loop() {
  for (int i = 0; i < NUMPIXELS; i++) { // For each pixel...
    pixels.setPixelColor(i, pixels.Color(pixelsColor));
  }
  strip.setBrightness(50);
  pixels.show();

}

/*********************************************
  函数名：pixelSetColor
  功  能: WS2812设置颜色
  形  参：无
  返回值：无
**********************************************/
void pixelSetColor(){
  
}
