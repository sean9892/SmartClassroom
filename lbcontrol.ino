//lbcontrol.ino

//COM6
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
#include <Servo.h>
Servo servo;
//light

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  BTSerial.begin(9600);// set the data rate for the BT port
  servo.attach(9);
  servo.write(0);
}

void do_servo()
{
  int angle;
  for(angle=0;angle<180;angle++){
    servo.write(angle);
    delay(15);
  }
  delay(300);
  for(angle;angle>=0;angle--){
    servo.write(angle);
    delay(15);
  }
}

void loop() {
  if (BTSerial.available()) {
    String s = BTSerial.readStringUntil('\n');
    Serial.println(s);
    if(s=="a"){
      Serial.println("work");
      do_servo();
    }
  }
}