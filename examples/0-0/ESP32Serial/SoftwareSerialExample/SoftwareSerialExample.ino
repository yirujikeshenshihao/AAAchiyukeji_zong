


#define mySerial  Serial2
void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  if (mySerial.available()) {
    String str= mySerial.readString();
     Serial.println(str);
  }

  if (Serial.available()) {
    mySerial.write(Serial.read());
  }
//    if(digitalRead(21)){
//     Serial.println("ok");
//  }
}
