#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);

#define pyroelectric 14 //Define the PIR Motion sensor pin to 14
#define led_y 12  //Define the yellow led pin to 12
#define buzPin 25 //Define the buzzer pin to 25

void setup() {
  Serial.begin(9600);
  mylcd.init();
  mylcd.backlight();
  pinMode(buzPin, OUTPUT); //Set pin to output mode
  pinMode(pyroelectric, INPUT);
  pinMode(led_y, OUTPUT);  //Set pin to output mode 
  mylcd.setCursor(0, 0);
  mylcd.print("Home Safe...");

}

void loop() {
  boolean pyroelectric_val = digitalRead(pyroelectric);
  Serial.print("pyroelectric value = ");
  Serial.println(pyroelectric_val);
  delay(200);
  if(pyroelectric_val == 1)
  {
          mylcd.clear();
      mylcd.setCursor(0, 0);
      mylcd.print("Intruder..."); //Display Intruder if Motion is Detected

    digitalWrite(led_y, HIGH); //Turn on the LED Light if Motion is Detected
    analogWrite(buzPin, 127); //Sound the Buzzer Alarm if Motion is Detected
    delay(500);

  }else{
    digitalWrite(led_y, LOW); //Turn off the LED Light if no Motion is Detected
    analogWrite(buzPin, 0); //Turn off the Buzzer Alarm if no Motion is Detected 
          mylcd.clear();
      mylcd.setCursor(0, 0);
      mylcd.print("Home Safe..."); //Display Home Safe if no Motion is Detected

  }
}
