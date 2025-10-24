#include <SimpleFOC.h>
#include <Wire.h>

// Comms Setup
Commander comms = Commander(Serial, '\n', true);

// Motor Setup
float motor_voltage = 7.0;
float align_voltage = 3.00;
int num_magnet_pairs = 7;
float phase_res = 0.05;
int kv = 1900;

float move_value = 0;
BLDCMotor bldc_motor = BLDCMotor(num_magnet_pairs, phase_res, kv);

// Driver Setup
float driver_voltage = 4.0;
BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(2, 3, 4, 6);  // With RPI Pico W, I used the EN pin

// Encoder Setup
MagneticSensorI2C mag_enc = MagneticSensorI2C(0x36, 12, 0x0E, 2);

void setup() {
  Serial.begin(112500);

  // Command Setup
  comms.add('M', move);
  comms.add('V', volt);

  
  // Check nFT Setup //
  pinMode(7, INPUT);

  // Mag Enc Setup //
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
  mag_enc.init(&Wire);

  bldc_motor.linkSensor(&mag_enc);

  // Driver Startup //
  SimpleFOCDebug::enable(&Serial);
  bldc_driver.voltage_power_supply = 15;
  bldc_driver.voltage_limit = driver_voltage;
  bldc_driver.init();
  // bldc_driver.pwm_frequency = 20000;

  // Motor Startup
  bldc_motor.linkDriver(&bldc_driver);
  // bldc_motor.voltage_sensor_align = align_voltage;
  bldc_motor.voltage_limit = motor_voltage;
  bldc_motor.controller = MotionControlType::angle;

  bldc_motor.PID_velocity.P = 0.2;
  bldc_motor.PID_velocity.I = 20;
  bldc_motor.PID_velocity.D = 0.001;

  bldc_motor.PID_velocity.output_ramp = 1000;

  bldc_motor.LPF_velocity.Tf = 0.01;

  bldc_motor.P_angle.P = 20;


  bldc_motor.velocity_limit = 4;



  bldc_motor.init();
  bldc_motor.initFOC();


  Serial.println("Finisehd Setup");
  delay(1000);
}


void loop() {  
  // Comms Loop
  comms.run();

  // Motor loop
  bldc_motor.loopFOC();
  bldc_motor.move(move_value);
  // Serial.println(move_value);

  bldc_driver.voltage_limit = driver_voltage;

  // Mag Enc update
  mag_enc.update();
  Serial.println(bldc_motor.shaftAngle());
  // Serial.println(mag_enc.getVelocity());

  // Check 
  if(digitalRead(7) == LOW){
    Serial.println("! ERROR !");
  }
}

void move(char* move_val_comms){
  move_value = atof(move_val_comms);
}

void volt(char* d){
  driver_voltage = atof(d);
}

// Made by Mäx