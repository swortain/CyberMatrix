/*
 * @Date: 2023-03-14 10:49:53
 * @LastEditors: swortain jianshan_54@126.com
 * @LastEditTime: 2023-03-14 10:59:26
 * @FilePath: \221025_led_matrix\lib\TMP1075\src\TMP1075.h
 * @Description: 
 */
/**
# ##### BEGIN GPL LICENSE BLOCK #####
#
# Copyright (C) 2020  Patrick Baus
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
# ##### END GPL LICENSE BLOCK #####
*/
#ifndef TMP1075_H
#define TMP1075_H

#include <stdint.h>   // uint16_t, etc.
#include <Wire.h>     // TwoWire

namespace TMP1075 {
  enum ConsecutiveFaults : uint8_t {
    Fault_1 = 0b00,
    Fault_2 = 0b01,
    Fault_3 = 0b10,
    Fault_4 = 0b11,
  };

  enum ConversionTime : uint8_t {
    ConversionTime27_5ms = 0b00,
    ConversionTime55ms = 0b01,
    ConversionTime110ms = 0b10,
    ConversionTime220ms = 0b11,
  };

  enum Offsets: uint8_t {
    OS = 7,
    R  = 5,
    F  = 3,
    POL = 2,
    TM = 1,
    SD = 0,    
  };

  class TMP1075
  {
    public:
      // i2cAdress is the default address when A0, A1 and A2 is tied low
      TMP1075(TwoWire &wire, uint8_t i2cAdress = 0x48);

      void begin();
      int16_t getTemperatureRaw();
      float getTemperatureCelsius();
      void startConversion();
      bool getConversionMode();
      void setConversionMode(const bool isSingleShot=false);
      ConversionTime getConversionTime();
      void setConversionTime(const ConversionTime value=ConversionTime27_5ms);
      ConsecutiveFaults getFaultsUntilAlert();
      void setFaultsUntilAlert(const ConsecutiveFaults value=Fault_1);
      bool getAlertPolarity();
      void setAlertPolarity(const bool isHigh=false);
      bool getAlertMode();
      void setAlertMode(const bool isInterrupt=false);
      int16_t getLowTemperatureLimitRaw();
      float getLowTemperatureLimitCelsius();
      void setLowTemperatureLimitRaw(const int16_t value=0x4b00);
      void setLowTemperatureLimitCelsius(const float value=75.f);
      int16_t getHighTemperatureLimitRaw();
      float getHighTemperatureLimitCelsius();
      void setHighTemperatureLimitRaw(const int16_t value=0x5000);
      void setHighTemperatureLimitCelsius(const float value=80.f);
      uint8_t getDeviceId();
    private:
      TwoWire &wire;
      void writeRegister(const uint8_t reg, const uint16_t value);
      void writeRegister(const uint8_t reg, const uint8_t value);
      float convertToCelsius(const int16_t value);
      int16_t convertFromCelsius(const float value);
      uint16_t readRegister(const uint8_t reg);
      uint8_t i2cAdress;
      uint8_t configRegister;
  };
}
#endif    // TMP1075.h
