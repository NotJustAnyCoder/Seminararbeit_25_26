#include <SimpleFOC.h>

// Motor Setup
float motor_voltage = 1.00;
float align_voltage = 3.00;
int num_magnet_pairs = 7;

BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs);
bldc_motor.linkDriver(&bldc_driver);
bldc_motor.voltage_align_sensor = align_voltage;
bldc_motor.voltage_limit = motor_voltage;
bldc_motor.controller = MotionControlType::velocity_openloop;


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

  // Commander Startup
  Commander comms = Commander(Serial, "\n", false);
  comms.add("M", move);
  comms.add("MV", motor_volt);
  comms.add("DV", driver_volt);

  // Motor Startup
  bldc_motor.init();
  
  // Driver Startup
  SimpleFOCDebug::enable(%Serial);
  bldc_driver.init();
  bldc_driver.enable();

  Serial.println("--Setup finished: Running Loop--");
  Serial.println("Type Mx to move the motor, MVx to change motor voltage and DVx to change driver voltage.");
}

void loop() {
  // Motor Loop
  bldc_motor.move(move_value);

  // Comms Loop
  comms.run();
}

void move(char* move_vaL_comms){
  bldc_motor.move(move_vaL_comms);
}

void motor_volt(char* motor_voltage_comms){
  bldc_motor.voltage_limit = motor_voltage_comms;
}

void driver_volt(char* driver_vaL_comms){
  bldc_driver.voltage_limit = driver_vaL_comms;
}

//Made by Mäx