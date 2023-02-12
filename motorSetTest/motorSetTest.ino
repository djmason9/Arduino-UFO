#include <AccelStepper.h>

const int DIR = 4;
const int STEP = 15;
const int maxSpeed = 300000;
const int minSpeed = 200000;
const int acceleration = 200;
const int READY_PIN = 2;
int MOVE_COW = 0;
int MOVING_COW = 0;
const int COW_TOP = 3600;

#define motorInterfaceType 1
AccelStepper motor(motorInterfaceType, STEP, DIR);

void setup() {
  Serial.begin(115200);  // opens serial port, sets data rate to 9600 bps
  pinMode(READY_PIN, OUTPUT);
  delay(5);
  motor.setMaxSpeed(maxSpeed);
  motor.setAcceleration(acceleration);
  motor.setCurrentPosition(0);
  digitalWrite(READY_PIN, HIGH);
}

void loop() {

  if (MOVING_COW >= COW_TOP) {  //move down
    Serial.println("***** TOP ******");
    // motor.setMaxSpeed(maxSpeed);
    MOVE_COW = -1;
    printShit("I received: ", MOVE_COW);
    delay(2000);
  } else if (MOVE_COW == -1) {
    Serial.println("***** BOTTOM ******");
    // motor.setMaxSpeed(minSpeed);
    MOVE_COW = COW_TOP;
    printShit("I received: ", MOVE_COW);
  }

  MOVING_COW = motor.currentPosition();

  // printShit("I received: ", MOVE_COW);
  printShit("Current Position : ", MOVING_COW);

  // delay(500);
  motor.moveTo(MOVE_COW);
  printShit("Move to Position : ", MOVE_COW);

  motor.run();
}

void printShit(String msg, int pos) {
  Serial.println("***********");
  Serial.print(msg);
  Serial.println(pos);
  Serial.println("***********");
}