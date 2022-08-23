#include "HUSKYLENS.h"
#include "SoftwareSerial.h"
#include "DFRobot_Servo.h"
HUSKYLENS huskylens;
Servo myservo1;
Servo myservo2;
int pos1 = 0;
int pos2 = 100;  
int ID1=1; 
int rev =-1;
int det =0;
int count =0;
volatile float mind_n_currentx, mind_n_currenty;
//HUSKYLENS green line >> SDA; blue line >> SCL
void follow(HUSKYLENSResult result);
void setup() {
    Serial.begin(115200);
    Serial.println("Hello");
    myservo1.attach(8);
    myservo2.attach(4);
    Wire.begin();                     // I2C
    while (!huskylens.begin(Wire))
    {
        Serial.println(F("Begin failed!"));
        Serial.println(F("1.Please recheck the \"Protocol Type\" in HUSKYLENS (General Settings>>Protocol Type>>I2C)"));
        Serial.println(F("2.Please recheck the connection."));
        delay(100);
    }

    huskylens.customText(String("ok"), 230, 35);
    delay(1000);
    huskylens.clearCustomText();
    Serial.println("Hello");
      mind_n_currentx = 40;
  mind_n_currenty = 150;
  myservo2.angle(pos2);
//  myservo.angle(180);
//  delay(30)
    
}

void loop() {
      huskylens.request(ID1);
      huskylens.isLearned(ID1);
      if ((!huskylens.available())&&det==0){
          if (pos1 ==180||pos1==0){ 
            rev = -rev;
            }
          if(rev==1){
            pos1++;
           
          }
          if(rev==-1){
            pos1--;
          }
          myservo1.angle(pos1);
          delay(40);
      }
      if ((huskylens.available())&&(det==0)){ det=1;}
      if (huskylens.available()&&det==1){
            HUSKYLENSResult result = huskylens.read();
            follow(result);
//            delay(15);
        }
      if ((!huskylens.available())&&det==1){ 
        count++;
      }
      if(count>=5000){
        Serial.println("Target Missing");
        huskylens.customText(String("Target Missing"), 180, 35);
      }
      if (huskylens.available()&&det==1){
        count==0;
        huskylens.clearCustomText();
      }
    
}

void follow(HUSKYLENSResult result){
    if (result.xCenter>200){
      pos1-=1;
      myservo1.angle(constrain(pos1,0,180));
      delay(20);
    }
    if (result.xCenter<120){
      pos1+=1;
      myservo1.angle(constrain(pos1,0,180));
      delay(20);
    }
    if (result.yCenter>140){
      pos2+=1;
      myservo2.angle(constrain(pos2,60,140));
      delay(20);
    }
    if (result.yCenter<100){
      pos2-=1;
      myservo2.angle(constrain(pos2,60,140));
      delay(20);
    }
    Serial.println(pos1);
    if (pos1>180){ pos1=180;}
    if (pos1<0){ pos1=0;}
    if (pos2>140){ pos2=140;}
    if (pos2<60){ pos2=60;}
}








    
