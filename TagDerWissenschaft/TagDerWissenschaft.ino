// Type Mx to move the motor, Vx to change motor voltage and Dx to change driver voltage.

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

// Encoder Setup
#define mag_enc_add 0x36
#define status_add 0x0B
#define angle_add 0x0E


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
  bldc_driver.voltage_power_supply = 15;
  bldc_driver.voltage_limit = driver_voltage;
  bldc_driver.init();

  // Motor Startup
  bldc_motor.linkDriver(&bldc_driver);
  bldc_motor.voltage_limit = motor_voltage;
  bldc_motor.controller = MotionControlType::velocity_openloop;
  bldc_motor.init();

  // Encoder Startup
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();
}

void loop() {  
  // Motor Loop
  bldc_motor.move(move_value);

  // Comms Loop
  comms.run();

  // Encoder Loop:
  if(Serial.available() > 0){
    Serial.readString();
    Wire.beginTransmission(mag_enc_add);
    Wire.write(status_add);
    Wire.endTransmission(false); //MUY IMPORTANTNE
    Wire.requestFrom(mag_enc_add, 1);

    if(Wire.available() == 1){
      byte status = Wire.read();

      if(status & 0x20) Serial.println("✅ Magnet detected.");
      if(status & 0x10) Serial.println("⚠️ Magnet too weak.");
      if(status & 0x08) Serial.println("⚠️ Magnet too strong.");
    }
  }

  Wire.beginTransmission(mag_enc_add);
  Wire.beginTransmission(mag_enc_add);
  Wire.write(angle_add);
  Wire.endTransmission(false);
  Wire.requestFrom(mag_enc_add, 2);

  if(Wire.available() == 2){
    byte byte_1 = Wire.read();
    byte byte_2 = Wire.read();

    float angle = ((byte_1 << 8) | byte_2) & 0x0FFF;
    float real_angle = angle * (360.0/4096.0);

    Serial.println(real_angle);
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