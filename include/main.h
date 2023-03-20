/*
 * @功能:
 * @Author: swortain
 * @Date: 2023-02-17 21:05:23
 * @LastEditTime: 2023-03-17 16:28:07
 */

#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <TMP1075.h>
#include <Wire.h>

#include "accel.h"
#include "font.h"
#include "matrixPrint.h"
#include "ntpTime.h"
#include "smartConfig.h"

//=======================Macro===============================
#define LED_PIN 4
#define LED_COUNT 5

#define DEBOUNCE_TIME 200
#define TAP_TIME 1000

// define FiniteStateMachine
enum stateMachine_t : int {
    STATE_INIT     = 0,
    STATE_DOWN     = 1,
    STATE_UP       = 2,
    STATE_COUNT    = 3,
    STATE_PRESS    = 6,
    STATE_PRESSEND = 7,
    UNKNOWN        = 99
};

//==============================LED==================================
TM1640 ledModule[MATRIX_NUMROWS / MATRIX_UNIT_ROW]
                [MATRIX_NUMCOLUMNS / MATRIX_UNIT_COL] = {
                    {TM1640(18, 19), TM1640(16, 17), TM1640(27, 14),
                     TM1640(25, 26), TM1640(32, 33)}};

TM1640Matrix ledMatrix((void**)ledModule, MATRIX_NUMCOLUMNS, MATRIX_NUMROWS,
                       MATRIX_NUMROWS / MATRIX_UNIT_ROW,
                       MATRIX_NUMCOLUMNS / MATRIX_UNIT_COL, rowPinmap,
                       colPinmap, MATRIX_DIRECTION_COL2ROW);

// Declare our NeoPixel strip object:
Adafruit_NeoPixel  strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ400);
uint8_t            stripDir;
unsigned long      stripChangeTime;
const uint16_t     stripHue             = 0xE38D;
const uint8_t      stripSat             = 0xB5;
PROGMEM const byte stripValSigmoid[256] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 6, 6, 6, 7, 7, 8, 8, 9, 9, 10, 11, 11, 12, 13, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 28, 29, 31, 33, 35, 36, 38, 40, 43, 45, 47, 49, 52, 54, 57, 60, 62, 65, 68, 71, 75, 78, 81, 84, 88, 91, 95, 99, 102, 106, 110, 113, 117, 121, 125, 129, 133, 136, 140, 144, 148, 152, 155, 159, 162, 166, 169, 173, 176, 179, 183, 186, 189, 192, 194, 197, 200, 202, 205, 207, 210, 212, 214, 216, 218, 220, 222, 223, 225, 227, 228, 230, 231, 232, 233, 235, 236, 237, 238, 239, 240, 241, 241, 242, 243, 244, 244, 245, 245, 246, 247, 247, 248, 248, 248, 249, 249, 250, 250, 250, 251, 251, 251, 251, 252, 252, 252, 252, 252, 253, 253, 253, 253, 253, 253, 253, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 254, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255, 255};
uint8_t            stripVal;

int    second_pre, minute_pre, hour_pre;
String timeStr = "00:00", timeStr_Pre;

//=======================Temperature=======================================
TMP1075::TMP1075 tmp1075 = TMP1075::TMP1075(Wire, 0x48);

//===========================ACCEL============================
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(12345);
sensors_event_t          event;
float                    refX, refY, refZ = 0;

stateMachine_t _state     = STATE_INIT;
stateMachine_t _lastState = STATE_INIT;
unsigned long  _startTime;
int            _nClicks      = 0;
bool           doubleTapFlag = false;

//============================Functions============================
void fsmTick(bool level);
void colorWipe(uint32_t color);
void ledTask(void* pvParameters);
