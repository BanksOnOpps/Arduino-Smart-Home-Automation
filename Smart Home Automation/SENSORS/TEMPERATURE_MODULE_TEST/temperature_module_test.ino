#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);
#include "xht11.h"
xht11 xht(17);
#define fanPin2 18

unsigned char dht[4] = {0, 0, 0, 0};//Only the first 32 bits of data are received, not the parity bits
void setup() {
  Serial.begin(9600);//Start the serial port monitor and set baud rate to 9600
  mylcd.init();
  mylcd.backlight();
  pinMode(fanPin2, OUTPUT);
}

void loop() {
  if (xht.receive(dht)) { //Returns true when checked correctly
    Serial.print("RH:");
    Serial.print(dht[0]); //The integral part of humidity, DHT [1] is the fractional part
    Serial.print("%  ");
    Serial.print("Temp:");
    Serial.print(dht[2]); //The integral part of temperature, DHT [3] is the fractional part
    Serial.println("C");
    
    mylcd.setCursor(0, 0);
    mylcd.print("T = ");
    mylcd.print(dht[2]);
    mylcd.setCursor(0, 1);
    mylcd.print("H = ");
    mylcd.print(dht[0]);
    //mylcd.clear();
    delay(200);
  } else {    //Read error
    Serial.println("sensor error");
  }
  if(dht[2] >= 50){
    analogWrite(fanPin2, 90);
  }else{
    analogWrite(fanPin2, 0);
    delay(3000);
  }
  delay(1000);  //It takes 1000ms to wait for the device to read
}
