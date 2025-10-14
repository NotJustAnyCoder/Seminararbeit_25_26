// Code documetation at: https://docs.simplefoc.com/code
#include <SimpleFOC.h>

// Config magnetic encoder
int mag_enc_pin = 1;
int mag_enc_res = 12;

#define angle_reg 0x0E;

MagneticSensorSPI mag_enc (mag_enc_pin, mag_enc_res, angle_reg)

// Config BLDC
// ⚠️ Only PWM Pins
int phase_1 = 9;
int phase_2 = 10;
int phase_3 = 11;

int bldc_voltage; //External battery

BLDCDriver3PWM bldc_driver = BLDCDriver3PWM(phase_1, phase_2, phase_3);

void setup() {
  // Encoder Setup
  mag_enc.init();

  // BLDC Driver Setup
  bldc_driver.pwm_frequency = 25000;
  bldc_driver.voltage_power_supply = bldc_voltage;
  bldc_driver.voltage_limit = 35;
  bldc_driver.init()
}

void loop() {
  // put your main code here, to run repeatedly:

}
