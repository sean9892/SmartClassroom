//main.ino

//COM7
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27,16,2);

#include <DS1302.h>
#define CLK_PIN 6
#define DATA_PIN 7
#define RST_PIN 8
DS1302 rtc(RST_PIN, DATA_PIN, CLK_PIN);

#include <SoftwareSerial.h>
SoftwareSerial btSerial(2,3);
SoftwareSerial btSerial2(4,5);

#define btn 10

int g=0,c=0;   //grade and class
int Class = 0;    //send to python
int h=0,m=0,s=0;   //time info 
int cnt=0;    //count how many class that i get
int cnt2=0;   //count class of pressent
int H[]={ 8, 9,10,11,13,14,15,22};  //class time - hour
int M[]={35,35,35,35,25,25,25, 0};  //class time - minutes
String sub[10];   //subject memory
int Out = 10;   //out-class count
String out[3][20]={ //out-class memory
  {"최승","채연","강남","심관","김용","창체","이영","없음","OM","MO"}, //1st grade out-class
  {"없음"}, //2nd grade out-class
  {"없음"}  //3rd grade out-class
};
int today=0;
int last_today=0;
Time t;

void check() //make analog value(from variable resistance) to grade - class
{
  int mk=map(analogRead(A1),0,1023,1,10);
  if(mk==10) mk=9;
  c=((mk-1)%3)+1;
  g=(mk-c+1)/3+1;
  lcd.setCursor(2,0);
  lcd.print("CHOOSE CLASS");
  lcd.setCursor(6,1);
  lcd.print(g);
  lcd.print("-"); 
  lcd.setCursor(8,1);
  lcd.print(c);
  if(digitalRead(btn)){
    Class=mk;
  }
}

void reset() //reset all variables
{
  g=0;  //grade reset
  c=0;  //class reset
  Class = 0;    //send to python
  h=0;  //time info-hour
  m=0;  //time info-minutes
  s=0;  //time info-second
  cnt=0;    //count how many class that i get
  cnt2=0;   //count class of pressent
  //lcd reset
  lcd.init();       //lcd start
  lcd.backlight();  //turn on backlight
  lcd.clear();      //remove all in screen
}

void start()
{
  reset();
  while(Class==0){
    check();
  }
  Serial.println(Class);
  lcd.setCursor(0,0);
  lcd.print("    CHOOSED     ");
}

void setup () 
{
  //ready----------------------------
  Serial.begin(9600);   //first command for using Serial port
  btSerial.begin(9600);   //first command for using blooth 1
  btSerial2.begin(9600);  //first command for using blooth 2
  //rtc module reset
  rtc.halt(false);  
//  rtc.writeProtect(false);  //시간 변경 락 해제
//  rtc.setDOW(TUESDAY);      //요일
//  rtc.setTime(21,45,0);      //시간
//  rtc.setDate(28,9,2020);   //날짜
  rtc.writeProtect(true);     //시간 변경 락
  t=rtc.getTime();
  today=t.date;
  last_today=t.date;
}

void GetTime() //get current time
{
  t=rtc.getTime();
  h=t.hour;
  m=t.min;
  s=t.sec;
  today=t.date;
}

void get_sub() //get schedule by communicating with python
{
  while(cnt<8){
    if(Serial.available()){
      String s = Serial.readStringUntil('\n');
      sub[cnt] = s;
      cnt++;
    }
  }
}

void work()
{
  while(today==last_today){ //work once a day 
    GetTime();
    if(h==H[cnt2] && m==M[cnt2] && s<5 &&cnt2<8){ //check the schedule
      int i,Bool=0;
      for(i=0;i<Out;i++){
        if(sub[cnt2]==out[g-1][i]){
          Bool=1;
          break;
        }
      }
      if(Bool){ //off
        btSerial.print("a\n");
        btSerial2.print("a\n");
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print("off");
        lcd.print(cnt2);
      } // No case to turn on
      cnt2++;
      delay(10000);
    }
    else if(cnt2==8){ //end of the class
      GetTime();
      while(h==H[cnt2]&&m==M[cnt2]&&s<5) ;
      //turn off the light and AirConditional
      btSerial.print("a\n");
      btSerial2.print("a\n");
      cnt2++;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("END");
      lcd.print(cnt2);
      lcd.print(today);
      delay(10000);
    }
  }
}

void loop() 
{
  reset();
  start();
  get_sub();
  work();
}