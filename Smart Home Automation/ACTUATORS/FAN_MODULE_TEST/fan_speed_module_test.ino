#define btn1 16  
#define btn2 27  
#define fanPin 19            
int fanSpeed = 0; //Define the speed variables

void setup() {
  Serial.begin(9600);
  pinMode(btn1, INPUT);  
  pinMode(btn2, INPUT);  
  pinMode(fanPin, OUTPUT);                  
  analogWrite(fanPin, fanSpeed);            
}

void loop() {
  int increaseButtonState = digitalRead(btn1);
  int decreaseButtonState = digitalRead(btn2);

  // Turn on and increase speed if button 1 is pressed
  if (increaseButtonState == LOW && fanSpeed < 180) {
    fanSpeed += 30; // Increase the fan speed (increment can be adjusted)
    analogWrite(fanPin, fanSpeed);
    delay(100); 
  }

  // Turn off the fan if button 2 is pressed
  if (decreaseButtonState == LOW && fanSpeed > 0) {
    fanSpeed -= 30;  // Decrease the fan speed (decrement can be adjusted)
    analogWrite(fanPin, fanSpeed);
    delay(100); 
  }
}
