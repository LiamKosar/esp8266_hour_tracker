#include <Arduino.h>
#include <EEPROM.h>
#include <string>

#define VIBRATION_SENSOR_PIN D2
int motion_detected = 0;
unsigned long total_active_time = 0;
unsigned long previous_total_active_time = 0;

int reading_delay = 5000;
int write_delay = 60000;

// put function declarations here:
void takeFiveReadings();
void readActiveTime();
void writeActiveTime();
void updateEEPRON();

void setup() {
  readActiveTime();
  Serial.begin(115200);
}

void loop() {
  takeFiveReadings();
  updateEEPRON();
  delay(reading_delay);
}

void takeFiveReadings() {

  motion_detected = 0;

  for (int x = 0; x < 5; x++) {
    motion_detected = std::max(motion_detected, digitalRead(VIBRATION_SENSOR_PIN)); 
    delay(reading_delay/50);
  }

  if (motion_detected == 1) {
    total_active_time += reading_delay + ((reading_delay/50) * 5);
  }

  Serial.println(previous_total_active_time);
  Serial.println(total_active_time);

}

void readActiveTime() {
  EEPROM.begin(4);
  EEPROM.get(0,total_active_time);
  total_active_time *= 1000;
  previous_total_active_time = total_active_time;
  EEPROM.commit();
  EEPROM.end();
}

void writeActiveTime() {
  EEPROM.begin(4);
  EEPROM.put(0, total_active_time/1000);
  EEPROM.commit();
  EEPROM.end();
}

void updateEEPRON() {
  if ((total_active_time - previous_total_active_time) >= (unsigned long)write_delay) {
    writeActiveTime();
    readActiveTime();
    Serial.println("Updated");
    previous_total_active_time = total_active_time;
  }
}