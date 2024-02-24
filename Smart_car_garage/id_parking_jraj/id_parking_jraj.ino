//************ include the library ***********
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

LiquidCrystal_I2C lcd(0x27,16,2);
//************ define the pin *************
#define IRpin_in 3
#define IRpin_out 4
#define servopin 11



Servo myservo;
int cou=0;
unsigned long x=0;
unsigned long y=0;

void setup() {
  lcd.init(); 
  pinMode(IRpin_in,INPUT);
  pinMode(IRpin_out,INPUT);
   myservo.attach(servopin);
  Serial.begin(9600);

}

void loop() {
  int value_in=digitalRead(IRpin_in);
  int value_out=digitalRead(IRpin_out);
   Serial.print("counter = ");
   Serial.println(cou);
   Serial.print("sensor in = ");
   Serial.println(value_in);
   Serial.print("sensor out = ");
   Serial.println(value_out);
   
// avlaiable places in park equal 6
// Check if there is avlaiable places mean count is less than or equal to 6
  if (cou <6 )
  {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Empty Places: ");
    lcd.print(6 - cou);
    lcd.setCursor(0, 1);
    lcd.print("Booked Up: ");
    lcd.print(cou);
    
  // If the sensor_in is HIGH
  if (value_in== HIGH && (millis() - x) >= 2000 )
  {
    myservo.write(0);
     x=millis();
    cou++;
  }
  // if the IR sensor is low 
  else 
  {
    myservo.write(90);
  } 
  }
  
   // If count is greater than 6
  else
  {
    myservo.write(90);
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("complete");
    lcd.setCursor(0, 1);
    lcd.print("please wait");
  }
  
  // If the sensor_out is HIGH
  if (value_out== HIGH && (millis() - y) >= 2000 && cou > 0 )
  {
    y= millis();
    cou--;
  }


delay(500);
}
