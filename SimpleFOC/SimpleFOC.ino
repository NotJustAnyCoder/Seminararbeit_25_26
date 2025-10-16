// Code documetation at: https://docs.simplefoc.com/code
#include <SimpleFOC.h>

// Config magnetic encoder
int mag_enc_res = 12;
int enc_bites = 2;
#define enc_add 0x36
#define angle_reg 0x0E
MagneticSensorI2C mag_enc = MagneticSensorI2C(enc_add, mag_enc_res, angle_reg, enc_bites);

// Config BLDC Driver
// ⚠️ Only PWM Pins
int phase_1 = 9;
int phase_2 = 10;
int phase_3 = 11;
int bldc_voltage = 19; //External battery
BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(phase_1, phase_2, phase_3, 1);

// Config BLDC
// XING-E Motor
int xing_pole_pairs = 7; //14 Poles
int xing_phase_resistance = 0.05;
int xing_kv = 1800;
// Badu Motor
int badu_pole_pairs = 7; //14 Poles
int badu_phase_resistance = 0.05;
int badu_kv = 360;
// Ufo Motor
int ufo_pole_pairs = 7;
int ufo_phase_resistance = 0.05;
int ufo_kv = 1900;

BLDCMotor bldc_motor = BLDCMotor(ufo_pole_pairs, ufo_phase_resistance, ufo_kv);

void setup() {
  Serial.begin(115200);


  SimpleFOCDebug::enable(&Serial);

  // Encoder Setup
  mag_enc.init();

  // BLDC Driver Setup
  bldc_driver.pwm_frequency = 25000;
  bldc_driver.voltage_power_supply = bldc_voltage;
  bldc_driver.voltage_limit = 35;
  bldc_driver.init();

  // BLDC Motor Setup
  bldc_motor.linkSensor(&mag_enc);
  bldc_motor.linkDriver(&bldc_driver);

  bldc_motor.useMonitoring(Serial);;

  // bldc_motor.controller = MotionControlType::velocity;
  bldc_motor.controller = MotionControlType::angle_openloop;

  bldc_motor.init();

  // FOC
  bldc_motor.initFOC();
}

void loop() {
  mag_enc.update();
  Serial.println(mag_enc.getAngle());

  // FOC
  bldc_motor.loopFOC();

  // Move Motor with velocity of 2rad/s
  bldc_motor.move(20);

  // Monitoring
  bldc_motor.monitor();
}