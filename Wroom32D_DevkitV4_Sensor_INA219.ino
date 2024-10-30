#include <Wire.h>
#include <Adafruit_INA219.h>

Adafruit_INA219 ina219;

float voltagem_V = 0;
float corrente_A = 0;


void setup() {
  Serial.begin(115200);
  ina219.begin();
  Serial.println("Conectado ao sensor INA219");
  delay(10);
}

void loop() {
  measureValues();
  // Other operations you might want to perform in the loop
}

void measureValues() {
  voltagem_V = ina219.getBusVoltage_V();
  corrente_A = ina219.getCurrent_mA();
  Serial.print("V:");
  Serial.println(voltagem_V);
  Serial.print("mah:");
  Serial.println(corrente_A/1000);
  delay(2000);
}