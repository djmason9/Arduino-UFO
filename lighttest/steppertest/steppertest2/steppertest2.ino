#include <AccelStepper.h>

const int DIR = 4;
const int STEP = 2;

#define motorInterfaceType 1
AccelStepper motor(motorInterfaceType, STEP, DIR);

long TravelX; 
int moveLoc = -1;
int move_finished=1;

void setup() {
  Serial.begin(9600);
  delay(5); 
  motor.setMaxSpeed(1000);
  motor.setAcceleration(60);
  motor.setSpeed(50);
  motor.moveTo(moveLoc);


  // while (true) {  // Make the Stepper move CW until the switch is deactivated
  //   motor.moveTo(moveLoc);
  //   moveLoc--;
  //   motor.run();
  //   delay(5);
  // }
}

void loop() {
  while (Serial.available()>0)  { // Check if values are available in the Serial Buffer

  move_finished=0;  // Set variable for checking move of the Stepper
  
  TravelX= Serial.parseInt();  // Put numeric value from buffer in TravelX variable
  if (TravelX < 0 || TravelX > 1350) {  // Make sure the position entered is not beyond the HOME or MAX position
    Serial.println("");
    Serial.println("Please enter a value greater than zero and smaller or equal to 1350.....");
    Serial.println("");
  } else {
    Serial.print("Moving stepper into position: ");
    Serial.println(TravelX);
  
  motor.moveTo(TravelX);  // Set new moveto position of Stepper
  
  delay(1000);  // Wait 1 seconds before moving the Stepper
  }
  }

  if (TravelX >= 0 && TravelX <= 1350) {

// Check if the Stepper has reached desired position
  if ((motor.distanceToGo() != 0)) {
    
    motor.run();  // Move Stepper into position
    
  }

// If move is completed display message on Serial Monitor
  if ((move_finished == 0) && (motor.distanceToGo() == 0)) {
    Serial.println("COMPLETED!");
    Serial.println("");
     Serial.println("Enter Travel distance (Positive for CW / Negative for CCW and Zero for back to Home): ");
    move_finished=1;  // Reset move variable
  }
  }
}