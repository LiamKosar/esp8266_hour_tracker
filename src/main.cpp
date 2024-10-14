#include <Arduino.h>
#include <EEPROM.h>

#define VIBRATION_SENSOR_PIN D2
int motion_detected = 0;
unsigned long total_active_time = 0;
unsigned long previous_total_active_time = 0;

// Vibration sensor reading delay (ms)
int reading_delay = 5000;

//EEPROM write delay (ms)
int write_delay = 60000;

// put function declarations here:
void takeFiveReadings();
void readActiveTime();
void writeActiveTime();
void updateEEPROM();

void setup() {
  readActiveTime();
  Serial.begin(115200);
}

void loop() {
  takeFiveReadings();
  updateEEPROM();
  delay(reading_delay);
}

/*Takes 5 readings, one every 50ms
  If any of them is a 1 (vibration detected), assume that the next period (reading_delay),
  is all vibration (running time)*/
void takeFiveReadings() {

  motion_detected = 0;

  for (int x = 0; x < 5; x++) {
    motion_detected = std::max(motion_detected, digitalRead(VIBRATION_SENSOR_PIN)); 
    delay(reading_delay/50);
  }

  // If there is motion detected, add the interval to total running time
  if (motion_detected == 1) {
    total_active_time += reading_delay + ((reading_delay/50) * 5);
  }

  Serial.println(previous_total_active_time);
  Serial.println(total_active_time);

}

/*
* Reads the value for total run time from EEPROM
* Multiply by 1000, because the value in EEPROM is in seconds
*/
void readActiveTime() {
  EEPROM.begin(4);
  EEPROM.get(0,total_active_time);
  total_active_time *= 1000;
  previous_total_active_time = total_active_time;
  EEPROM.commit();
  EEPROM.end();
}

/**
 * Writes the total active time to EEPROM
 * Converts to seconds (/1000) before storing
 * to save space
*/
void writeActiveTime() {
  EEPROM.begin(4);
  EEPROM.put(0, total_active_time/1000);
  EEPROM.commit();
  EEPROM.end();
}

/**
 * Updates the total running time in EEPROM
 * Only if the difference between the current total time
 * and the previously 'written' total time is more than the write delay
*/
void updateEEPROM() {
  if ((total_active_time - previous_total_active_time) >= (unsigned long)write_delay) {
    writeActiveTime();
    readActiveTime();
    Serial.println("Updated");
    previous_total_active_time = total_active_time;
  }
}