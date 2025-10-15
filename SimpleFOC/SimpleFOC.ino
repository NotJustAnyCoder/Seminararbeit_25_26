// Code documetation at: https://docs.simplefoc.com/code
#include <SimpleFOC.h>

// Config magnetic encoder
int mag_enc_pin = 1;
int mag_enc_res = 12;
#define angle_reg 0x0E;
MagneticSensorSPI mag_enc (mag_enc_pin, mag_enc_res, angle_reg)

// Config BLDC Driver
// ⚠️ Only PWM Pins
int phase_1 = 9;
int phase_2 = 10;
int phase_3 = 11;
int bldc_voltage; //External battery
BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(phase_1, phase_2, phase_3);

// Config BLDC
// XING-E Motor
int xing_pole_pairs = 7; //14 Poles
int xing_phase_resistance = 0.05;
int xing_kv = 1800;
// Badu Motor
int badu_pole_pairs = 7 //14 Poles
int badu_phase_resistance = 0.05;
int badu_kv = 360;
// Ufo Motor
int jufo_pole_paris = 0;
int ufo_phase_resistance = 0.05;
int ufo_kv = 0;

BLDCMotor bldc_motor = BLDCMotor(xing_pole_pairs, xing_phase_resistance, xing_kv);

void setup() {
  // Encoder Setup
  mag_enc.init();

  // BLDC Driver Setup
  bldc_driver.pwm_frequency = 25000;
  bldc_driver.voltage_power_supply = bldc_voltage;
  bldc_driver.voltage_limit = 35;
  bldc_driver.init()

  // BLDC Motor Setup
  bldc_motor.linkSensor(&mag_enc);
  bldc_motor.linkDriver(&bldc_driver);
  bldc_motor.controller = MotionControlType::velocity;
  bldc_motor.init();

  // FOC
  bldc_motor.initFOC();
}

void loop() {
  
}
