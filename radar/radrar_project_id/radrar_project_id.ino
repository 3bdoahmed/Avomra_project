#include <Servo.h>

#define buzzerpin 6
#define trigPin  10
#define echoPin  11
#define servopin 12

// ************function of ultrasonic ***************
float ultra_dis(int trgpin, int echopin) {
  digitalWrite(trgpin, LOW);
  delayMicroseconds(2);
  digitalWrite(trgpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trgpin, LOW);
  float x = pulseIn(echopin, HIGH);
  float dis = (x * 0.0343) / 2;
  return dis;
}
//*********************************************

Servo myServo;
int flag_dis= -1;

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerpin,OUTPUT);

  Serial.begin(9600);
  myServo.attach(servopin);
}

void loop() {
  for (int i = 15; i <= 165; i++) {
    float distance = ultra_dis(trigPin, echoPin);
    if (distance != flag_dis)
    {
       Serial.print(i);                 
       Serial.print(","); 
       Serial.print(distance); 
       Serial.print("."); 
       flag_dis = distance;
    }
    myServo.write(i);
    if (distance<=50)
    {
      digitalWrite(buzzerpin,HIGH);
    }
    else
    {
      digitalWrite(buzzerpin,LOW);
    }
    delay(30);
    
  }

  delay(500); // Add a delay between servo sweeps

  for (int i = 165; i > 15; i--) {
    float distance = ultra_dis(trigPin, echoPin);
    if (distance != flag_dis)
    {
       Serial.print(i);                 
       Serial.print(","); 
       Serial.print(distance); 
       Serial.print("."); 
       flag_dis = distance;
    }
    if (distance<=50)
    {
      digitalWrite(buzzerpin,HIGH);
    }
    else
    {
      digitalWrite(buzzerpin,LOW);
    }
    
    myServo.write(i);
    delay(30);
    
}


}
