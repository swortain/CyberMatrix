#ifndef ACCEL_H
#define ACCEL_H

#include <Adafruit_ADXL345_U.h>
#include <Adafruit_Sensor.h>
#include <Arduino.h>

extern Adafruit_ADXL345_Unified accel;

void displayAccelDetails();
void displayAccelDataRate();
void displayAccelRange();

#endif