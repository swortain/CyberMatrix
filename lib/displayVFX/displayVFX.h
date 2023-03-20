/*
 * @Date: 2023-03-10 14:50:20
 * @LastEditors: swortain jianshan_54@126.com
 * @LastEditTime: 2023-03-16 15:01:07
 * @FilePath: \221025_led_matrix\lib\DisplayVFX\displayVFX.h
 * @Description:
 */

#ifndef _DISPLAYVFX_H
#define _DISPLAYVFX_H

#include <Adafruit_GFX.h>
#include <Arduino.h>

typedef void (*_setLineCallback)(byte, byte);

bool PointbyPoint(byte* linesNow, byte* linesToDraw, byte lineNum, _setLineCallback _setLineCallback);

#endif