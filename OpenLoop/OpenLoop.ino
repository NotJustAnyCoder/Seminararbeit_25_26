#include <SimpleFOC.h>

// Comms Setup
Commander comms = Commander();

// Motor Setup
float motor_voltage = 1.00;
float align_voltage = 3.00;
int num_magnet_pairs = 7;

BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs);

// Driver Setup
float driver_voltage = 15;

BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(2, 3, 4, 6);  // With RPI Pico W, I used the EN pin

// Move Setup
float move_value = 2.00;

void setup() {
  Serial.begin(12500);

  // Commander Startup
  comms.add('M', move);
  comms.add('V', motor_volt);
  comms.add('D', driver_volt);

  // Motor Startup
  bldc_motor.linkDriver(&bldc_driver);
  bldc_motor.voltage_sensor_align = align_voltage;
  bldc_motor.voltage_limit = motor_voltage;
  bldc_motor.controller = MotionControlType::velocity_openloop;
  bldc_motor.init();
  
  // Driver Startup
  SimpleFOCDebug::enable(&Serial);
  bldc_driver.pwm_frequency = 20000;
  bldc_driver.voltage_power_supply = 15;
  bldc_driver.voltage_limit = 12;
  bldc_driver.init();
  bldc_driver.enable();

  Serial.println("--Setup finished: Running Loop--");
  Serial.println("Type Mx to move the motor, Vx to change motor voltage and Vx to change driver voltage.");
}

void loop() {  
  // Motor Loop
  bldc_motor.move(move_value);

  // Comms Loop
  comms.run(Serial);

  // Check 
  if(digitalRead(7) == LOW){
    Serial.println("! ERROR !")
  }
}

void move(char* move_val_comms){
  move_value = atof(move_val_comms);
}

void motor_volt(char* motor_voltage_comms){
  move_value = atof(motor_voltage_comms);
}

void driver_volt(char* driver_val_comms){
  move_value = atof(driver_val_comms);
}

//Made by Mäx