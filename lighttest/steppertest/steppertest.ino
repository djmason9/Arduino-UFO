#include <TMCStepper.h>
#include <AccelStepper.h>
// #define EN_PIN 14   // Enable
#define DIR_PIN 4   // Direction
#define STEP_PIN 2  // Step


#define SERIAL_PORT Serial2  // TMC2208/TMC2224 HardwareSerial port
//#define DRIVER_ADDRESS 0b00 // TMC2209 Driver address according to MS1 and MS2

#define R_SENSE 0.11f  // Match to your driver
#define motorInterfaceType 1
TMC2208Stepper driver(&SERIAL_PORT, R_SENSE);  // Hardware Serial
AccelStepper motor = AccelStepper(motorInterfaceType, STEP_PIN, DIR_PIN);

void setup() {
  // pinMode(EN_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  // digitalWrite(EN_PIN, LOW);  // Enable driver in hardware

  // Enable one according to your setup
  //SPI.begin();                    // SPI drivers

  Serial.begin(115200);
  // SERIAL_PORT.begin(115200, SERIAL_8N1, 33, 25);  // HW UART drivers
  //driver.beginSerial(115200);     // SW UART drivers

  driver.begin();          //  SPI: Init CS pins and possible SW SPI pins
                           // UART: Init SW UART (if selected) with default 115200 baudrate
  driver.toff(5);          // Enables driver in software
  driver.rms_current();    // Set motor RMS current
  driver.microsteps(200);  // Set microsteps to 1/16th


  //driver.en_pwm_mode(true);       // Toggle stealthChop on TMC2130/2160/5130/5160
  driver.en_spreadCycle(true);  // Toggle spreadCycle on TMC2208/2209/2224
  //driver.pwm_autoscale(true);     // Needed for stealthChop
  Serial.println("Got here.");


  delay(5);
  motor.setMaxSpeed(1000);
  motor.setAcceleration(500);
  motor.setSpeed(50);
}

bool shaft = false;

void loop() {

   if (motor.distanceToGo() == 0) {
        motor.disableOutputs();
        delay(500);
        motor.move(1600); // Move 100mm
        motor.enableOutputs();
    }
    motor.run();
}
