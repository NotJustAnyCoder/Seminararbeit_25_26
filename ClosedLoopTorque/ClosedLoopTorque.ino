#include <SimpleFOC.h>

// Comms Setup
Commander comms = Commander(Serial, '\n', true);

// Motor Startup
int num_magnet_pairs = 7;
float phase_resistance = 0.05;
int kv = 1900;

// int num_magnet_pairs = 7;
// float phase_resistance = 0.15;
// int kv = 360;
BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs, phase_resistance, kv);

// Driver Starutp
BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(2, 3, 4, 6);

// Magnetic Encoder Startup
MagneticSensorI2C mag_enc = MagneticSensorI2C(0x36, 12, 0x0E, 2);

// Move Setup
float move_value = 2.0;

void setup() {
  Serial.begin(115200);

  // Check nFT Setup
  pinMode(7, INPUT);

  // Commander Startup
  comms.add('M', move);

  // Magnetic Encoder Setup
  mag_enc.init();

  // Driver Setup
  bldc_driver.voltage_power_supply = 15;
  bldc_driver.init();

  // Motor Setup
  bldc_motor.torque_controller = TorqueControlType::voltage;
  bldc_motor.linkSensor(&mag_enc);
  bldc_motor.linkDriver(&bldc_driver);
  bldc_motor.torque_controller = TorqueControlType::voltage;
  bldc_motor.controller = MotionControlType::torque;
  // motor.useMonitoring(Serial);
  bldc_motor.init();
  bldc_motor.initFOC();


}

void loop() {
  // Magnetic Encoder Loop
  mag_enc.update();

  // Motor Loop
  bldc_motor.loopFOC();
  bldc_motor.move(move_value);

  
  // Comms Loop
  comms.run();

  // Check 
  if(digitalRead(7) == LOW){
    Serial.println("! ERROR !");
  }
}

void move(char* move_val_comms){
  move_value = atof(move_val_comms);
}

// Made by Mäx