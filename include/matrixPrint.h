/*
 * @Date: 2023-03-16 18:02:28
 * @LastEditors: swortain jianshan_54@126.com
 * @LastEditTime: 2023-03-16 18:13:52
 * @FilePath: \221025_led_matrix\include\matrixPrint.h
 * @Description:
 */
#ifndef MATRIXPRINT_H
#define MATRIXPRINT_H
#include <Arduino.h>
#include <TM1640.h>
#include <TM1640Matrix.h>

#include "displayVFX.h"

#define MATRIX_NUMCOLUMNS 35
#define MATRIX_NUMROWS 11
#define MATRIX_UNIT_ROW 11
#define MATRIX_UNIT_COL 7

const byte colPinmap[16] = {10, 9, 0, 8, 1, 7, 6, 2, 5, 3, 4, 0, 0, 0, 0, 0};
const byte rowPinmap[8]  = {6, 5, 7, 2, 4, 1, 3, 0};

extern TM1640Matrix ledMatrix;

void PrintTimeStr(String _s_pre, String _s);
void PrintTimeSeparator(String _s_pre, String _s);

void setLineCallback(byte nLine, byte bPixels);

#endif