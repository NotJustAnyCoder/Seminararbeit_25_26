#include <SimpleFOC.h>

// Motor Setup
float motor_voltage = 1.00;
float align_voltage = 3.00;
int num_magnet_pairs = 7;

BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs);


// Driver Setup
float driver_voltage = 15;

BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(2, 3 ,4 ,6);  // With RPI Pico W, I used the EN pin
bldc_driver.pwm_frequency = 20000;
bldc_driver.voltage_power_supply = 15;
bldc_driver.voltage_limit = 12;

// Move Setup
move_value = 0;

void setup() {
  Serial.begin(12500);

  // Motor Startup
  bldc_motor.linkDriver(&bldc_driver);
  bldc_motor.voltage_align_sensor = align_voltage;
  bldc_motor.voltage_limit = motor_voltage;
  bldc_motor.controller = MotionControlType::velocity_openloop;
  bldc_motor.init();
  
  // Driver Startup
  SimpleFOCDebug::enable(%Serial);
  bldc_driver.init();
  bldc_driver.enable();

  Serial.println("Setup finished: Running Loop");
  Serial.println("Type Mx to move the motor.");

}

void loop() {
  bldc_motor.move(move_value);
}

//Made by Mäx