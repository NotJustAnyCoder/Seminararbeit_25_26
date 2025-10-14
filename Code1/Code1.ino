// Shows angle of magnetic encoder and status, to show status enter something in the command line
#include <Wire.h>

#define mag_enc_add 0x36
#define status_add 0x0B
#define angle_add 0x0E

void setup() {
  Serial.begin(9600);
  Wire.begin();
}

void loop() {
  Wire.beginTransmission(mag_enc_add);

  if(Serial.available() > 0){
    Serial.readString();
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

  Wire.write(angle_add);
  Wire.endTransmission(false);
  Wire.requestFrom(mag_enc_add, 2);

  if(Wire.available() == 2){
    byte byte_1 = Wire.read();
    byte byte_2 = Wire.read();

    float angle = ((byte_2 << 8) | byte_1) & 0x0FFF;
    float real_angle = angle * (360/4096);

    // Serial.println("Angle: " + String(real_angle));
    Serial.println(real_angle);
  }
  

  delay(100);


}

//Made by Mäx