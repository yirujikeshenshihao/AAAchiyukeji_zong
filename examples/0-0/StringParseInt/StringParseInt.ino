void setup() {
  delay(1000);
  Serial.begin(9600);
  String str = "111.13.2";
  Serial.println(parseInt(str));
  Serial.println(parseInt(str));
  Serial.println(parseInt(str));
}

void loop() {
}

/*
 *函数作用：字符串中一次返回到int整数
 *函数入口参数：
 *str2： 要获取整数的字符串
 *返回值：int
*/
int parseInt(String str2) {
  static String str1 = "";  // 用于存储前一个字符串
  int val1 = 0;             // 解析的整数值
  static int count = 0;     // 字符串遍历计数
  int loopCount = 0;        // 循环计数
  int chaCount = 0;         // 字符计数

  if (!areStringsEqual(str1.c_str(), str2.c_str())) {  // 如果两个字符串不相等，则重置前一个字符串和计数器
    str1 = str2;
    count = 0;
  }

  while (count < str1.length()) {
    if (str1[count] >= '0' && str1[count] <= '9') {
      chaCount = 1;                 // 标记已经遇到数字字符
      int num = str1[count] - '0';  // 将字符转换为数字
      val1 = val1 * 10 + num;       // 构建整数值
    } else {
      if (chaCount == 1) {
        return val1;  // 如果已经解析完整数部分，返回解析结果
      }
    }
    count++;
  }

  if (count != 0) {
    return val1;  // 返回解析结果
  }
  return 0;  // 默认返回0，如果没有找到整数部分
}
/*
 *函数作用： 比较两个字符串是否相等
 *函数入口参数：
 *str1： 要比较的的字符串1
 *str2： 要比较的的字符串2
 *返回值：bool
*/
bool areStringsEqual(const char* str1, const char* str2) {
  return strcmp(str1, str2) == 0;  // 比较两个字符串是否相等
}
