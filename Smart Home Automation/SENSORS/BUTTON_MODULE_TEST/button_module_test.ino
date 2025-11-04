 #define btn1 16 // Pin connected to the button
// #define btn2 27
int buttonState1 = 0;     // Variable to store the button state
// int buttonState2 = 0;

void setup() {
  Serial.begin(9600);    // Initialize serial communication at 9600 baud rate
  pinMode(btn1, INPUT); // Set the button pin as INPUT
  // pinMode(btn2, INPUT);
}

void loop() {
  buttonState1 = digitalRead(btn1); // Read the state of the button
  buttonState2 = digitalRead(btn2);

  // Check if the button is pressed
  if (buttonState1 == HIGH) {
    Serial.println("Button1 is pressed");
  } else {
    Serial.println("Button1 is not pressed");
  }

  delay(100); // Delay for stability

  if (buttonState2 == HIGH) {
    Serial.println("Button2 is pressed");
  } else {
    Serial.println("Button2 is not pressed");
  }
  delay(100);
 
}

