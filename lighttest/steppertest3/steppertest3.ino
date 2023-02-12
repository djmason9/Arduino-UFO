#include <AccelStepper.h>

const int DIR = 4;
const int STEP = 15;
const int maxSpeed = 150000;
const int minSpeed = 200;

#define motorInterfaceType 1
AccelStepper motor(motorInterfaceType, STEP, DIR);


int incomingByte = 0;  // for incoming serial data
const int READY_PIN = 2;

void setup() {
  Serial.begin(115200);  // opens serial port, sets data rate to 9600 bps
  pinMode(READY_PIN, OUTPUT);
  delay(5);
  motor.setMaxSpeed(maxSpeed);
  motor.setAcceleration(minSpeed);
  motor.setCurrentPosition(-1);
  digitalWrite(READY_PIN, HIGH);
  
  Serial.println("***********");
  Serial.println("Enter steps");
  Serial.println("***********");
}

void loop() {
  // reply only when you receive data:
  if (Serial.available() > 0) {
    // read the incoming byte:
    incomingByte = Serial.parseInt();
    if (incomingByte == 1) {
      motor.setAcceleration(maxSpeed);
    } else {
      motor.setAcceleration(minSpeed);
    }

    if (incomingByte > 0) {

      Serial.println("***********");

      printShit("I received: ", incomingByte);
      printShit("Current Position : ", motor.currentPosition());

      delay(500);
      motor.moveTo(incomingByte);
      printShit("Move to Position : ", incomingByte);


    } else if (incomingByte == -1) {
      printShit("Current Position : ", motor.currentPosition());
    }
  }


  motor.run();
}

void printShit(String msg, int pos) {
  Serial.println("");
  Serial.print(msg);
  Serial.println(pos);
  Serial.println("***********");
}