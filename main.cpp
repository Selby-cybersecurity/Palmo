#include <Servo.h>

// Pin Definitions
const int BUTTON_PIN = 2;     // Button to trigger grip
const int SENSOR_PIN = A0;    // Flex sensor or Potentiometer for safety limit
const int SERVO_PIN  = 9;     // Servo motor signal pin

// Variables
Servo palmoServo;
int buttonState = 0;
bool isGripping = false;

// Customization - Adjust these based on your glove's mechanical range
const int OPEN_ANGLE = 0;      // Fully open hand
const int CLOSED_ANGLE = 120;  // Maximum safe closed grip

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP); // Uses internal resistor (LOW when pressed)
  palmoServo.attach(SERVO_PIN);
  
  // Start in the open position
  palmoServo.write(OPEN_ANGLE);
  Serial.begin(9600);
}

void loop() {
  // Read the button state
  buttonState = digitalRead(BUTTON_PIN);
  
  // Read the safety sensor (0 to 1023)
  int safetyLimit = analogRead(SENSOR_PIN);

  // If button is pressed (LOW), close the glove
  if (buttonState == LOW) {
    if (!isGripping) {
      Serial.println("Grip Activated");
      performGrip(CLOSED_ANGLE);
      isGripping = true;
    }
  } 
  // If button is released, open the glove
  else {
    if (isGripping) {
      Serial.println("Grip Released");
      performGrip(OPEN_ANGLE);
      isGripping = false;
    }
  }
  
  delay(50); // Small delay for debouncing
}

// Function to move servo smoothly
void performGrip(int targetAngle) {
  int currentAngle = palmoServo.read();
  
  if (currentAngle < targetAngle) {
    for (int pos = currentAngle; pos <= targetAngle; pos += 1) {
      palmoServo.write(pos);
      delay(15); // Adjust this delay to change the speed of the grip
    }
  } else {
    for (int pos = currentAngle; pos >= targetAngle; pos -= 1) {
      palmoServo.write(pos);
      delay(15);
    }
  }
}
