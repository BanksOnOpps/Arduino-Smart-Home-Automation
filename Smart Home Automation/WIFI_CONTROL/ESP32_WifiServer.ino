#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiClient.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif
// Which pin on the Arduino is connected to the NeoPixels?
// On a Trinket or Gemma we suggest changing this to 1:
#define LED_PIN    26
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 4
// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

String item = "0";
const char* ssid = "La Buena Vida🤘";
const char* password = "Whereareyougoing";
WiFiServer server(80);

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C mylcd(0x27,16,2);
#include "xht11.h"
xht11 xht(17);
#include <ESP32_Servo.h>
Servo servo1;
Servo servo2;

int minUs = 1000;
int maxUs = 2800;
int servo1Pin = 5;
int servo2Pin = 13;

#define fanPin1 19
#define fanPin2 18
#define buzPin 25
#define waterPin 34 
#define gasPin 23
#define led_y 12  //Define the yellow led pin to 12
#define pyroelectric 14 //Define the PIR Motion sensor pin to 14

unsigned char dht[4] = {0, 0, 0, 0};//Only the first 32 bits of data are received, not the parity bits

void setup() {
  Serial.begin(115200);
  mylcd.init();
  mylcd.backlight();
  pinMode(led_y, OUTPUT);
  pinMode(fanPin1, OUTPUT);
  pinMode(fanPin2, OUTPUT);
  pinMode(buzPin, OUTPUT); 
  pinMode(pyroelectric, INPUT);
  pinMode(waterPin, INPUT);
  pinMode(gasPin, INPUT);
  pinMode(pyroelectric, INPUT);
  servo1.attach(servo1Pin, minUs, maxUs);
  servo2.attach(servo2Pin, minUs, maxUs);

    // These lines are specifically to support the Adafruit Trinket 5V 16 MHz.
  // Any other board, you can remove this part (but no harm leaving it):
// #if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
//   clock_prescale_set(clock_div_1);
// #endif
//   // END of Trinket-specific code.

//   strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
//   strip.show();            // Turn OFF all pixels ASAP
//   strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
// }

WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  Serial.println("TCP server started");
  MDNS.addService("http", "tcp", 80);
  mylcd.setCursor(0, 0);
  mylcd.print("ip:");
  mylcd.setCursor(0, 1);
  mylcd.print(WiFi.localIP());  //LCD displays the ip adress
}

void loop() {
  WiFiClient client = server.available();
  if (!client) {
      return;
  }
  while(client.connected() && !client.available()){
      delay(1);
  }
  String req = client.readStringUntil('\r');
  int addr_start = req.indexOf(' ');
  int addr_end = req.indexOf(' ', addr_start + 1);
  if (addr_start == -1 || addr_end == -1) {
      Serial.print("Invalid request: ");
      Serial.println(req);
      return;
  }
  req = req.substring(addr_start + 1, addr_end);
  item=req;
  Serial.println(item);
  String s;
  if (req == "/")  //Browser accesses address can read the information sent by the client.println(s);
  {
      IPAddress ip = WiFi.localIP();
      String ipStr = String(ip[0]) + '.' + String(ip[1]) + '.' + String(ip[2]) + '.' + String(ip[3]);
      s = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>Hello from ESP32 at ";
      s += ipStr;
      s += "</html>\r\n\r\n";
      Serial.println("Sending 200");
      client.println(s);  //Send the string S, you can read the information when visiting the address of E smart home using a browser.  
  //client.print(s);
  }
  if(req == "/led/on") //Browser accesses the ip address/led/on
  {
    client.println("turn on the LED");
    digitalWrite(led_y, HIGH);
  }
  if(req == "/led/off") //Browser accesses the ip address/led/off
  {
    client.println("turn off the LED");
    digitalWrite(led_y, LOW);
  }
   if(req == "/window/open") //Browser access the ip address/window/open
  {
    client.println("open the window");
    servo1.write(180);
  }
  if(req == "/window/close") //Browser access the ip address/window/open
  {
    client.println("close the window");
    servo1.write(0);
  }
  if(req == "/door/open") //Browser access the ip address/door/open
  {
    client.println("open the door");
    servo2.write(0);
  }
  if(req == "/door/close") //Browser access the ip address/door/close
  {
    client.println("close the door");
    servo2.write(90);
  }
  
  if(req == "/buzzer/on") //Browser access the ip address/buzzer/on
  {
    client.println("buzzer is on");
    analogWrite(buzPin, 400); 
  }
  if(req == "/buzzer/off") //Browser access the ip address/buzzer/off
  {
    client.println("buzzer is off");
    analogWrite(buzPin, 0);
  }
  if(req == "/fan/on") //Browser accesses the ip address/fan/on
  {
    client.println("turn on the fan");
    digitalWrite(fanPin1, LOW); //pwm = 0
    analogWrite(fanPin2, 90);
  }
  if(req == "/fan/off") //Browser accesses the ip address/fan/off
  {
    client.println("turn off the fan");
    digitalWrite(fanPin1, LOW); //pwm = 0
    analogWrite(fanPin2, 0);
  }
  if(req == "/red/on")
  {
    client.println("red on");
    colorWipe(strip.Color(255,   0,   0), 50); 
  }
  if(req == "/red/off")
  {
    client.println("red off");
    colorWipe(strip.Color(0, 0, 0), 50);
  }
  if(req == "/green/on")
  {
    client.println("green on");
    colorWipe(strip.Color(0, 255, 0), 50); 
  }
  if(req == "/green/off")
  {
    client.println("green off");
    colorWipe(strip.Color(0, 0, 0), 50);
  }
  if(req == "/blue/on")
  {
    client.println("blue on");
    colorWipe(strip.Color(0, 0, 255), 50); 
  }
  if(req == "/blue/off")
  {
    client.println("blue off");
    colorWipe(strip.Color(0, 0, 0), 50);
  }
  if(req == "/yellow/on")
  {
  client.println("yellow on");
  colorWipe(strip.Color(200,   200,   0), 50);
  }
  if(req == "/yellow/off")
  {
    client.println("yellow off");
    colorWipe(strip.Color(0,   0,   0), 50);
  }
  if(req == "/cyan/on")
  {
    client.println("cyan on");
    colorWipe(strip.Color(0,   100,   255), 50);
  }
  if(req == "/cyan/off")
  {
    client.println("cyan off");
    colorWipe(strip.Color(0,   0,   0), 50);
  }
  if(req == "/purple/on")
  {
    client.println("purple on");
    colorWipe(strip.Color(100,   0,   255), 50);
  }
  if(req == "/purple/off")
  {
    client.println("purple off");
    colorWipe(strip.Color(0,   0,   0), 50);
  }
  if(req == "/white/on")
  {
    client.println("white on");
    colorWipe(strip.Color(255,   255,   255), 50);
  }
  if(req == "/white/off")
  {
    client.println("white off");
    colorWipe(strip.Color(0,   0,   0), 50);
  }

  if(req == "/sfx/on")
  {
    client.println("sxf on");
    theaterChaseRainbow(50);
  }
  if(req == "/sfx/off")
  {
    client.println("sfx off");
    colorWipe(strip.Color(0, 0, 0), 50);
  }
  if(req == "/rain/on")
  {
    int water_val = analogRead(waterPin);
    client.println(water_val);
  }
  if(req == "/rain/off")
  {
    client.println("no rain");
  }
  if(req == "/gas/on")
  {
    boolean gasVal = digitalRead(gasPin);
    if(gasVal == 0)
    {
      client.println("safe");
    }
    else{
      client.println("dangerous");
    }
  }
  if(req == "/gas/off")
  {
    client.println("off");
  }
  if(req == "/body/on")
  {
    boolean pyroelectric_val = digitalRead(pyroelectric);
    if(pyroelectric_val == 1)
    {
      client.println("intruder");
    }
    else{
      client.println("home safe...");
    }
  }
  if(req == "/body/off")
  {
    client.println("off");
  }
  if(req == "/temp/on")
  {
    if(xht.receive(dht))
    {
    Serial.print("Temp:");
    Serial.print(dht[2]);
    Serial.println("C");
    }
    else{
    Serial.println("sensor error");
    }
    client.println(dht[2]);
    delay(1000);
     
  }
  if(req == "/temp/off")
  {
    client.println("off");
  }
  if(req == "/humidity/on")
  {
    if(xht.receive(dht))
    {
    Serial.print("RH:");
    Serial.print(dht[0]); //The integral part of humidity, DHT [1] is the fractional part
    Serial.print("%");
    delay(200);
    }
    else{
      Serial.println("sensor error");
    }
    client.println(dht[0]);
    delay(1000);
  }
  if(req == "/humidity/off")
  {
    client.println("off");
  }

  //client.print(s);
  client.stop();
}

void colorWipe(uint32_t color, int wait) {
  for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
    strip.setPixelColor(i, color);         //  Set pixel's color (in RAM)
    strip.show();                          //  Update strip to match
    delay(wait);                           //  Pause for a moment
  }
}

// Rainbow cycle along whole strip. Pass delay time (in ms) between frames.
void rainbow(int wait) {
 
  for(long firstPixelHue = 0; firstPixelHue < 5*65536; firstPixelHue += 256) {
    for(int i=0; i<strip.numPixels(); i++) { // For each pixel in strip...
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      int pixelHue = firstPixelHue + (i * 65536L / strip.numPixels());
     
      strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(pixelHue)));
    }
    strip.show(); // Update strip with new contents
    delay(wait);  // Pause for a moment
  }
}

// Rainbow-enhanced theater marquee. Pass delay time (in ms) between frames.
void theaterChaseRainbow(int wait) {
  int firstPixelHue = 0;     // First pixel starts at red (hue 0)
  for(int a=0; a<30; a++) {  // Repeat 30 times...
    for(int b=0; b<3; b++) { //  'b' counts from 0 to 2...
      strip.clear();         //   Set all pixels in RAM to 0 (off)
      // 'c' counts up from 'b' to end of strip in increments of 3...
      for(int c=b; c<strip.numPixels(); c += 3) {
     
        int      hue   = firstPixelHue + c * 65536L / strip.numPixels();
        uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
        strip.setPixelColor(c, color); // Set pixel 'c' to value 'color'
      }
      strip.show();                // Update strip with new contents
      delay(wait);                 // Pause for a moment
      firstPixelHue += 65536 / 90; // One cycle of color wheel over 90 frames
    }
  }
}





