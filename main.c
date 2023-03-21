#include Arduino.h

// Pin Definitions
#define M1 5   // Left Motor
#define M2 6   // Right Motor

// Constants
#define SPEED 150      // Motor speed

void setup() {
  pinMode(M1, OUTPUT);
  pinMode(M2, OUTPUT);
}

void loop() {
  // Move the robot forward
  forward();

  // Turn the robot left
  left();

  // Turn the robot right
  right();

  // Stop the robot
  stop();
}

void forward() {
  digitalWrite(M1, HIGH);
  digitalWrite(M2, HIGH);
  analogWrite(M1, SPEED);
  analogWrite(M2, SPEED);
}

void left() 
{
  digitalWrite(M1, HIGH);
  digitalWrite(M2, LOW);
  analogWrite(M1, SPEED);
  analogWrite(M2, SPEED);
}

void right() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, HIGH);
  analogWrite(M1, SPEED);
  analogWrite(M2, SPEED);
}

void stop() {
  digitalWrite(M1, LOW);
  digitalWrite(M2, LOW);
}
