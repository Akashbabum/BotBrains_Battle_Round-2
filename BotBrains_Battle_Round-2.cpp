#include <Servo.h>
#include <LowPower.h>

const int soilMoisturePin = A0;
const int potentiometerPin = A1;
const int pumpPin = 9;
const int buttonPin = 2;
const int buzzerPin = 8;
const int ledPin = 7;
Servo servoMotor;

int customThreshold = 300;

void setup() {
  pinMode(soilMoisturePin, INPUT);
  pinMode(potentiometerPin, INPUT);
  pinMode(pumpPin, OUTPUT);
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  servoMotor.attach(10);

  attachInterrupt(digitalPinToInterrupt(buttonPin), wakeUp, FALLING);
}

void loop() {
  customThreshold = analogRead(potentiometerPin); // Read potentiometer value for custom threshold
  customThreshold = map(customThreshold, 0, 1023, 200, 800); // Map the potentiometer value to a valid threshold range
  
  int soilMoisture = analogRead(soilMoisturePin);
  
  if (soilMoisture < customThreshold) {
    digitalWrite(pumpPin, HIGH);
    servoMotor.write(90);
    feedback(true);
    delay(5000);           // Watering duration
    digitalWrite(pumpPin, LOW);
    servoMotor.write(0);
    feedback(false);
  }

  delay(1000); // Check soil moisture every second

  enterSleepMode(); // Enter sleep mode after each loop iteration
}

void feedback(bool watering) {
  if (watering) {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
    delay(100);
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  } else {
    digitalWrite(buzzerPin, HIGH);
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(buzzerPin, LOW);
    digitalWrite(ledPin, LOW);
  }
}

void enterSleepMode() {
  // Ensure pump and servo are off before entering sleep mode
  digitalWrite(pumpPin, LOW);
  servoMotor.write(0);

  // Enter sleep mode
  LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
}

void wakeUp() {
  // Empty interrupt function to wake up the microcontroller
}
