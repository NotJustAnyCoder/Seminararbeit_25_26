#include <SimpleFOC.h>

// Comms Setup
Commander comms = Commander(Serial, '\n', true);

// Motor Setup
float motor_voltage = 1.0;
float align_voltage = 3.00;
int num_magnet_pairs = 7;

BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs);

// Driver Setup
float driver_voltage = 1.0;

BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(2, 3, 4, 6);  // With RPI Pico W, I used the EN pin

// Move Setup
float move_value = 2.0;

void setup() {
  Serial.begin(12500);

  // Check nFT Setup
  pinMode(7, INPUT);

  // Commander Startup
  comms.add('M', move);
  comms.add('V', motor_volt);
  comms.add('D', driver_volt);

  // Driver Startup
  SimpleFOCDebug::enable(&Serial);
  // bldc_driver.pwm_frequency = 20000;
  bldc_driver.voltage_power_supply = 15;
  bldc_driver.voltage_limit = driver_voltage;
  bldc_driver.init();
  // bldc_driver.enable();

  // Motor Startup
  bldc_motor.linkDriver(&bldc_driver);
  // bldc_motor.voltage_sensor_align = align_voltage;
  bldc_motor.voltage_limit = motor_voltage;
  bldc_motor.controller = MotionControlType::velocity_openloop;
  bldc_motor.init();

  Serial.println("--Setup finished: Running Loop--");
  Serial.println("Type Mx to move the motor, Vx to change motor voltage and Dx to change driver voltage.");
}

void loop() {  
  // Motor Loop
  bldc_motor.move(move_value);
  // Serial.println(move_value);

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

void motor_volt(char* motor_voltage_comms){
  motor_voltage = atof(motor_voltage_comms);
}

void driver_volt(char* driver_voltage_comms){
  driver_voltage = atof(driver_voltage_comms);
}

//Made by Mäx