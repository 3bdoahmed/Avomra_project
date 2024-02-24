//************ include the library ***********
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>
#include "DHT.h"

LiquidCrystal_I2C lcd(0x27, 16, 2);
//*************** Define Pin *******************
#define buzzer 10
#define DHT11PIN A0  
#define in1 12
#define in2 13
#define ena 11
#define pinled 9
#define touch A1
#define servopin 7
//*************Initialize DHT Sensor***************
DHT dht(DHT11PIN, DHT11);

//********* Keypad Configuration********************
const byte numRows = 3;
const byte numCols = 3;
char keymap[numRows][numCols] = {
  {'1', '2' , '3'},
  {'4', '5' , '6'},
  {'7', '8' , '9'},
};
byte rowPins[numRows] = {2, 3, 4};
byte colPins[numCols] = {5, 6,8};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//*************Flags and Timers********************
bool isDoorOpen = false;
bool iswake = false;
bool flag_ddor = true;
unsigned long sleep_time = 0;
unsigned long doorOpenTime = 0;
unsigned long x = 0;
String alarm = "";
//*****************************************
Servo myservo;
//********************Function Declarations**********
void checkWakeUpTime(); // func to set wake-up time
void checkDoorStatus(); // func to control the state of door 
void know_temp(int dh_t); // func to tell you about temp in rom
void know_Humidity(int dh_h); // func to tell you about humidity in room
void processKeypadInput(); // func espacially for controlling the buttons in keypad
//***********************************************
void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(DHT11PIN, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(ena, OUTPUT);
  pinMode(pinled, OUTPUT);
  pinMode(touch, INPUT);
  myservo.attach(servopin);
  lcd.init();
  dht.begin();
  Serial.begin(9600);
}

void loop() {
  int dh_h = dht.readHumidity();
  int dh_t = dht.readTemperature();
  Serial.print("temperature: ");
  Serial.println(dh_t);
  Serial.print("Humidity: ");
  Serial.println(dh_h);
  
  //----------->> colling the funcations <<--------------
  checkWakeUpTime();
  checkDoorStatus();
  processKeypadInput();
}

//*********** Function Definitions *******************

void processKeypadInput() {
  char keypressed = myKeypad.getKey();
  String s = "";

  if (keypressed == '1') {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Hello, ");
    lcd.setCursor(0, 1);
    lcd.print("HOW Can help you?");
  } else if (keypressed == '2') {
    digitalWrite(buzzer, LOW);
    iswake = true;
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("time you want");
    lcd.setCursor(0, 1);
    lcd.print("wake up: ");
    while (alarm.length() < 3) {
      keypressed = myKeypad.getKey();
      if (keypressed) {
        alarm += keypressed;
        lcd.print(keypressed);
        delay(100);
      }
      sleep_time = millis();
    }
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("alarm will run");
    lcd.setCursor(0, 1);
    lcd.print("after: ");
    lcd.print(alarm);
    lcd.print("0 s");
    
  }
  else if (keypressed == '3'){
    digitalWrite(buzzer, LOW);
  }
  else if (keypressed == '4') {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("the lights");
    lcd.setCursor(0, 1);
    lcd.print("of room is on");
    digitalWrite(pinled, HIGH);
  } else if (keypressed == '5') {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("the lights");
    lcd.setCursor(0, 1);
    lcd.print("of room is off");
    digitalWrite(pinled, LOW);
  } else if (keypressed == '7') {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Enter speed ");
    lcd.setCursor(0, 1);
    lcd.print("of fan: ");
    while (s.length() < 2) {
      keypressed = myKeypad.getKey();
      if (keypressed) {
        s += keypressed;
        lcd.print(keypressed);
        delay(100);
      }
    }
    int x = s[0] + s[1];
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("the speed is: ");
    lcd.setCursor(0, 1);
    lcd.print(x);
    analogWrite(ena, x);
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
  } else if (keypressed == '8') {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("the motor ");
    lcd.setCursor(0, 1);
    lcd.print("aready closed");
    analogWrite(ena, 0);
  }
  else if (keypressed == '9') {
    int dh_t = dht.readTemperature();
    know_temp(dh_t);
  
  }
   else if (keypressed == '6') {
    int dh_h = dht.readHumidity();
    know_Humidity(dh_h);
  } 
}
//----------------------------------------------------

void checkWakeUpTime() {

  int h = (alarm[0] - '0') * 100;
  int m = (alarm[1] - '0') * 10 + (alarm[2] - '0');
  int time_wake_up = (h + m) * 10;
  if (millis() - sleep_time >= time_wake_up&& iswake) {
    alarm = "";
    digitalWrite(buzzer, HIGH);
    digitalWrite(pinled, HIGH);
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("wake up, ");
    lcd.setCursor(0, 1);
    lcd.print("yaa Shalabi");
    iswake = false; // fage to do this IF statement once only
  }
}
//-----------------------------------------------------

void checkDoorStatus() {
  int if_p = digitalRead(touch);

  if (if_p == HIGH &&flag_ddor== true ) {
    isDoorOpen = true; // to enter the IF (it count the sec befor the door close) when only opend door.
    doorOpenTime = millis();
    myservo.write(180);
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("door is opened: ");
    flag_ddor=false;
    //delay(100);
  }

  if (millis() - x >= 5000 && isDoorOpen == true) {
    flag_ddor=true;
    int countdown = 5 - ((millis() - doorOpenTime) / 1000);
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Watch out,");
    lcd.setCursor(0, 1);
    lcd.print("closing in:");
    lcd.print(countdown);
    delay(200);

    if (countdown <= 0) {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("door is closed");
      lcd.setCursor(0, 1);
      lcd.print("Secure now!");
      myservo.write(90);
      x = millis();
      isDoorOpen = false;
    }
  }
}
//--------------------------------------------------

void know_temp(int dh_t) {
  String s = "";
  if (dh_t >= 30) { 
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Temp is:");
    lcd.print(dh_t);
    lcd.print(" pross");
    lcd.setCursor(0, 1);
    lcd.print("button 7 to open fun");
  } else if (dh_t < 30 ) {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Temp is:");
    lcd.print(dh_t);
    lcd.setCursor(0, 1);
    lcd.print("needn't open fan");
 
  }
}
//------------------------------------------------

void know_Humidity(int dh_h){
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Humidity is:");
    lcd.print(dh_h);
    lcd.setCursor(0, 1);
  }
