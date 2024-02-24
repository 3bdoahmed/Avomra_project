//************ include the library ***********
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16,2);
//************ define the pin ultrasonic ***********
#define trpin 9
#define echpin 10

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

void setup() {
  lcd.init(); 
  pinMode(trpin,OUTPUT);
  pinMode(echpin,INPUT);
  Serial.begin(9600);
  
}

void loop() {
  float distance = ultra_dis(trpin,echpin);
  Serial.print("the distance is: ");
   Serial.print(distance);
   Serial.println("cm");

   if (distance > 500 )
   {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Remaining: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("can get closer");
   }
   else if (distance <= 500 && distance >=300 )
   {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Remaining: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("distance is good");
   }
   else if (distance <300 && distance >=100)
   {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Remaining: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("watch out");
   }
   else if (distance <100)
   {
    lcd.clear();
    lcd.backlight();
    lcd.setCursor(0, 0);
    lcd.print("Remaining: ");
    lcd.print(distance);
    lcd.setCursor(0, 1);
    lcd.print("Must stay away");
   }
   delay(500);

}
