//accontrol.ino

//COM9
#include <SoftwareSerial.h>
SoftwareSerial BTSerial(2,3);
/*
  Measure current temperature and ambient light level, then determine whether to turn on or off A/C.
  
  Temperature Measurement
  Reads an analog input on A0(output from LM35DZ), change it to the real room temperature and print it to serial monitor.
  
  Ambient Light Measurement
  Reads an analog input on A3
 */
int IRledPin =  13;    // IR LED connected to digital pin 13
float powervoltage=5;//define the power supply voltage.
int On = 0; //Initial state of A/C should be OFF

void setup() {
  // initialize serial communication at 9600 bits per second:
  pinMode(IRledPin, OUTPUT);   
  Serial.begin(9600);
  BTSerial.begin(9600);
}

void loop() {
  if (BTSerial.available()) {
    String s = BTSerial.readStringUntil('\n');
    if(s=="a"){
      if(analogRead(A0)>500){
        TurnOff();
        delay(100);
      }
    }
  }
}

// This procedure sends a 38KHz pulse to the IRledPin 
// for a certain # of microseconds. We'll use this whenever we need to send codes
void pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
 
  cli();  // this turns off any background interrupts
 
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
   digitalWrite(IRledPin, HIGH);  // this takes about 3 microseconds to happen
   delayMicroseconds(10);         // hang out for 10 microseconds
   digitalWrite(IRledPin, LOW);   // this also takes about 3 microseconds
   delayMicroseconds(10);         // hang out for 10 microseconds
 
   // so 26 microseconds altogether
   microsecs -= 26;
  }
 
  sei();  // this turns them back on
}

void TurnOff() {
  delayMicroseconds(23516); //Time off (Left Column on serial monitor)
  pulseIR(3100);           //Time on  (Right Column on serial monitor)
  delayMicroseconds(8620);
  pulseIR(520);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(520);
  delayMicroseconds(1520);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(500);
  pulseIR(500);
  delayMicroseconds(500);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(480);
  pulseIR(520);
  delayMicroseconds(520);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(500);
  delayMicroseconds(1540);
  pulseIR(500);
}