// Shows angle of magnetic encoder and status
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

  Wire.write(angle_add);
  Wire.endTransmission(false); //MUY IMPORTANTNE
  Wire.requestFrom(mag_enc_add, 1);

  if(Wire.available() == 1){
    byte status = Wire.read();

    if(status & 0x20) Serial.println("MD");
    if(status & 0x10) Serial.println("MC");
    if(status & 0x08) Serial.println("ML");

  }
  delay(100);

  /*
  Wire.beginTransmission(mag_enc_add);

  if(Serial.available() == 1){
    Wire.write(status_add);
    Wire.requestFrom(mag_enc_add, 1);

    if(Wire.available()==1){
      int byte_val = Wire.read();
      Serial.println("Status_val: " + String(byte_val));
    }
  }
  `*/
  
}


//Made by Mäx