//************ include the library ***********
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Keypad.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x20,16,2);
//************ define the pin ultrasonic ***********
#define trpin 9
#define echpin 10
#define servopin 11

// ************funcation of ultrasonic ***************

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
//*********** keypad  ********************
const byte numRows = 4;
const byte numCols = 3;

char keymap[numRows][numCols] =
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};
byte rowPins[numRows] = {8, 7, 6, 5};
byte colPins[numCols] = {4, 3, 2};
Keypad myKeypad = Keypad(makeKeymap(keymap), rowPins, colPins, numRows, numCols);

//********** inat varabols ******************************

Servo myservo; 
String c= "1234";
int cou=0;

void setup() {
  lcd.init(); 
  pinMode(trpin,OUTPUT);
  pinMode(echpin,INPUT);
  myservo.attach(servopin);
  Serial.begin(9600);
}

void loop() {
  
  float distance = ultra_dis(trpin,echpin);
  char keypressed = myKeypad.getKey();
  String s = "";
  Serial.print("the distance is ");
  Serial.print(distance);
  Serial.println("cm");
  Serial.println(cou);
  myservo.write(90);
  int y=0;

  if (cou %3 ==0 && cou !=0)
  {
     lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("wait for ");
       lcd.print((cou/3*10));
        lcd.print(" sce");
    while(y < (cou/3*10))
    {
      y++;
      lcd.setCursor(3, 1);
       lcd.print(y);
       delay(1000); 
    } 
  }
  
   if (distance <= 500)
   {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Enter Password");
    lcd.setCursor(0, 1);
   
      while (s.length() < 4) {
      keypressed = myKeypad.getKey();
      if (keypressed) {
        s += keypressed;
        lcd.print("*");
        delay(100); 
      }
    }
     
     if (s[0]==c[0] && s[1]==c[1] && s[2]==c[2] && s[3]==c[3] ) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Accpted");
        lcd.setCursor(0, 1);
        lcd.print("you can enter ");
        myservo.write(0);
        cou=0;
        delay(5000);
      }
      else {
        cou++;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("passwoed wrong");
         lcd.setCursor(0, 1);
         if (cou %3 ==0 && cou !=0)
         {
           lcd.print("max limit ");
         }
         else
         {
          lcd.print("try agian");
         }
        
        delay(500);
      }     
  }
   else {
     lcd.clear();
     lcd.setBacklight(LOW);   
  }

}
