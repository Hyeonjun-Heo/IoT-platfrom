#include <Servo.h>

Servo myservo;

int pos = 0;
int servoPin = 9;

void setup() {
  Serial.begin(9600);

  pinMode(6, OUTPUT);
  pinMode(7, INPUT);
  pinMode(servoPin, OUTPUT);

  myservo.attach(9);
}

void loop() {
  int readValue = digitalRead(7);
  Serial.println(readValue);

  if (readValue == HIGH){
    digitalWrite(6, HIGH);
  }

  else {
    digitalWrite(6, LOW);
  }

}