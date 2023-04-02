#include <Servo.h>
Servo myservo;
int Ebutton = 2;
int button1 = 7;
const int trig = 8; //ประกาศขา trig
const int echo = 9; //ประกาศขา echo

int led =13;
int buzzer =11;
int ldr = A0;
int mois = A1;

int moisValue = 0;
bool isday = 0;
bool status= 0;
bool flood = 0;
int door =0;
int data;
int newdata[1]={0};
long duration, distance;

void quaterNote(){
  delay(350);
  noTone(buzzer);
  delay(50); 
}
void eightNote(){
  delay(125);
  noTone(buzzer); 
  delay(50);
}
void nokia(){
  tone(buzzer, 659.25); 
  digitalWrite(led,HIGH);
    eightNote();     
  tone(buzzer,587.33);
  digitalWrite(led,LOW);  
    eightNote();     
  tone(buzzer,369.99);
    quaterNote();
  tone(buzzer,415.30);
    quaterNote(); 
  tone(buzzer,554.37);
  digitalWrite(led,HIGH);
    eightNote();
  tone(buzzer,493.88);
  digitalWrite(led,LOW);
    eightNote();
  tone(buzzer,293.66);
    quaterNote();
  tone(buzzer,329.63);
    quaterNote();
  tone(buzzer,493.88);
  digitalWrite(led,HIGH);
    eightNote();
  tone(buzzer,440.00);
  digitalWrite(led,LOW);
    eightNote();
  tone(buzzer,277.18);
    quaterNote();
  tone(buzzer,329.63);
    quaterNote();
  tone(buzzer,440.00);
  delay(500);
  noTone(buzzer);
  delay(1000);

}

void setup() {
  Serial.begin(9600);
  myservo.attach(12);  
  pinMode(echo, INPUT); 
  pinMode(trig, OUTPUT);
  pinMode(led, OUTPUT);
  pinMode(button1, INPUT);
  pinMode(Ebutton, INPUT);
  pinMode(led, OUTPUT);
  pinMode(buzzer, OUTPUT);
  digitalWrite(led, LOW);
  noTone(buzzer);
  if(analogRead(ldr)>=25){
    isday=1;
  }
  else{
    isday=0;
  }

  myservo.writeMicroseconds(3000);
  delay(650);
  myservo.writeMicroseconds(1450); 
  delay(1000);
  Serial.println("closing");
  myservo.writeMicroseconds(0);
  delay(600);
  myservo.writeMicroseconds(1450); 

}

void loop() {
  
  while (Serial.available())    
  { 
    data = Serial.read();
  }
  if(data=='1'){
    newdata[0]=1;
  }
  else{
    newdata[0]=0;
  }


  digitalWrite(trig, HIGH); 
  delayMicroseconds(5); 
  digitalWrite(trig, LOW); //ใช้งานขา trig
 
  duration = pulseIn(echo, HIGH); //อ่านค่าของ echo
  distance = (duration/2) / 29.1;

  
  Serial.println(analogRead(ldr));
  if(analogRead(mois)<=250 || distance <= 5){//flood
    flood=1;
    Serial.println("flood");
    if(status==0){
      status=1;            
      myservo.writeMicroseconds(3000);
      delay(500);
      myservo.writeMicroseconds(1450);
    }
    nokia();
  }
  else{
    status=0;
    noTone(buzzer);
    if(flood){//after flood, close the door
      status=1;
      Serial.println("closing");
      myservo.writeMicroseconds(0);
      delay(500);
      myservo.writeMicroseconds(1450);
      flood=0;      
      }
    if(analogRead(ldr)>=25){//isday
      digitalWrite(led,LOW);
      if(digitalRead(button1)==HIGH && door==0 && status==0){
        status=1;
        Serial.println("close->full");
        myservo.writeMicroseconds(3000);
        delay(500);
        myservo.writeMicroseconds(1450);  
        door=2;
      }
      else if(digitalRead(button1)==HIGH && door==2 && status==0){
        status=1;
        Serial.println("closing");
        myservo.writeMicroseconds(0);
        delay(500);
        myservo.writeMicroseconds(1450); 
        door=0;       
      }
      else if(newdata[0]==1 && door==0 && status==0){
        status=1;
        Serial.println("close->half");
        myservo.writeMicroseconds(3000);
        delay(500);
        myservo.writeMicroseconds(1450); 
        delay(2000);
        door=2;
      }
      else if(newdata[0]==1 && door==2 && status==0){
        status=1;
        Serial.println("closing");
        myservo.writeMicroseconds(0);
        delay(500);
        myservo.writeMicroseconds(1450); 
        delay(2000);
        door=0;
      }
      if(isday){
        Serial.println("Day");
        Serial.println("-------------------------------");
        isday=0;
      }     
    }
    else{//night
      digitalWrite(led,HIGH);
      if(digitalRead(button1)==HIGH && door==0 && status==0|| newdata[0]==1 && door==0 ){
        status=1;
        Serial.println("close->half");
        myservo.writeMicroseconds(3000);
        delay(250);
        myservo.writeMicroseconds(1450); 
        door=2;
      }
      else if(digitalRead(button1)==HIGH && door==2 && status==0|| newdata[0]==1 && door==2){
        status=1;
        Serial.println("closing");
        myservo.writeMicroseconds(0);
        delay(250);
        myservo.writeMicroseconds(1450); 
        door=0;       
      }      
      else if(newdata[0]==1 && door==0 ){
        status=1;
        Serial.println("close->half");
        myservo.writeMicroseconds(3000);
        delay(250);
        myservo.writeMicroseconds(1450); 
        delay(2000);
        door=2;
      }
      else if(newdata[0]==1 && door==2 ){
        status=1;
        Serial.println("closing");
        myservo.writeMicroseconds(0);
        delay(250);
        myservo.writeMicroseconds(1450); 
        delay(2000);
        door=0;
      }
      if(!isday){
        Serial.println("Night");
        Serial.println("-------------------------------");
        isday=1;
      }          
    }

  }
  
}