#include <ESP32_Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);

Servo myservo;
#define servoPin 5 //Define the Servo Motor sensor pin to 5
int minUs = 1000; // Min pulse width for Servo Motor (0-degree) 
int maxUs = 2800; //Max pulse width for Servo Motor (180-degree) 
#define waterPin 34 //Define the Steam sensor pin to 34

void setup() {
  Serial.begin(9600);
    mylcd.init();
  mylcd.backlight();
  pinMode(waterPin, INPUT);
  myservo.attach(servoPin, minUs, maxUs);
  myservo.write(180);
  delay(200);
  mylcd.setCursor(0, 0);
  mylcd.print("Weather Normal!");

}

void loop() {
  int water_val = analogRead(waterPin);
  Serial.println(water_val);
  if(water_val > 1500) {
    myservo.write(0);
    delay(200);
          mylcd.setCursor(0, 0);
      mylcd.print("Shutting Windows!"); 

  }
  else {
    myservo.write(180);
    delay(200);
    mylcd.setCursor(0, 0);
      mylcd.print("Weather Normal!."); 
  }
}


