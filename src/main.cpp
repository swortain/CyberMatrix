/*
 * @功能:
 * @Author: swortain
 * @Date: 2022-10-25 23:16:33
 * @LastEditTime: 2023-03-17 16:25:52
 */
#include "main.h"

void setup() {
    Serial.begin(115200);

    // =======================NEOPIXEL=========================
    strip.begin();
    strip.setBrightness(60);
    strip.show();
    // =======================LED_MATRIX=======================
    ledMatrix.printClockDataPin();
    ledMatrix.setFont(&dot11pxFont);
    ledMatrix.setCursor(0, 10);
    ledMatrix.drawDirect();
    ledMatrix.print("Init.");
    // =======================WIFI=============================
    delay(100);
    if (!AutoConfig()) {
        SmartConfig();
    }
    // =======================TIME&NTP=============================
    NTPTime_Init();

    // ======================TEMPERATURE===========================
    Wire.begin(21, 22);
    tmp1075.begin();

    // ======================ACCEL===========================
    if (!accel.begin(0x53))
        Serial.println("ERROR: no ADXL345 detected ... Check your wiring!");
    accel.setRange(ADXL345_RANGE_16_G);
    displayAccelDetails();
    displayAccelDataRate();
    displayAccelRange();
    delay(1000);
    accel.getEvent(&event);
    for (byte i = 0; i < 20; i++) {
        refX += event.acceleration.x;
        refY += event.acceleration.y;
        refZ += event.acceleration.z;
    }
    refX /= 20;
    refY /= 20;
    refZ /= 20;

    Serial.print("refX: ");
    Serial.print(refX);
    Serial.print(", refY: ");
    Serial.print(refY);
    Serial.print(", refZ: ");
    Serial.println(refZ);

    ledMatrix.fillRect(0, 0, 35, 11, 0x00);
    PrintTimeStr("Init.", "Cyber");
    delay(1000);
    PrintTimeStr("Cyber", "MatX");
    delay(3000);

    xTaskCreate(ledTask, "RTOS task for backlight and ledMatrix", 2048, NULL, 1, NULL);
}

void loop() {
    // /* Display the results (acceleration is measured in m/s^2) */
    accel.getEvent(&event);

    // Serial.print(event.acceleration.x);
    // Serial.print(",");
    // Serial.print(event.acceleration.y);
    // Serial.print(",");
    // Serial.print(event.acceleration.z);
    // Serial.println();

    if (abs(event.acceleration.x - refX) > 0.35) {
        // Serial.print("[");
        // Serial.print(millis());
        // Serial.print("] true");
        // Serial.println();
        fsmTick(true);
    } else {
        // Serial.print("[");
        // Serial.print(millis());
        // Serial.print("] false");
        // Serial.println();
        fsmTick(false);
    }
}

void ledTask(void* pvParameters) {
    while (true) {
        if (millis() - stripChangeTime > 10) {
            if (stripVal == 0) stripDir = 1;
            if (stripVal == 0xFF) stripDir = 0;
            stripVal = (stripDir == 1) ? stripVal + 1 : stripVal - 1;
            colorWipe(strip.ColorHSV(stripHue, stripSat, stripValSigmoid[stripVal]));
            stripChangeTime = millis();
        }

        if (doubleTapFlag) {
            // 双击桌面，显示温度
            tmp1075.setConversionTime(TMP1075::ConversionTime220ms);
            float temperature = tmp1075.getTemperatureCelsius();
            Serial.print("Temp: ");
            Serial.println(temperature);

            String temperatureStr = String(temperature).substring(0, 5);
            PrintTimeStr(timeStr_Pre, temperatureStr);
            delay(1000);
            PrintTimeStr(temperatureStr, timeStr_Pre);

            doubleTapFlag = false;
        } else {
            //============================================
            if ((minute_pre != minute()) | (hour_pre != hour())) {
                timeStr = (hour() < 10 ? ("0" + String(hour())) : String(hour())) + ':' + (minute() < 10 ? ("0" + String(minute())) : String(minute()));

                PrintTimeStr(timeStr_Pre, timeStr);
                timeStr_Pre = timeStr;

                minute_pre = minute();
                hour_pre   = hour();
            } else if (second_pre != second()) {
                if (timeStr[2] == ':') {
                    timeStr[2] = ' ';
                    PrintTimeSeparator(timeStr_Pre, timeStr);
                    timeStr_Pre = timeStr;
                } else {
                    timeStr[2] = ':';
                    PrintTimeSeparator(timeStr_Pre, timeStr);
                    timeStr_Pre = timeStr;
                }
                Serial.print("timeStr:");
                Serial.println(timeStr);
                second_pre = second();
            }
        }
    }
}

//============================WS2812========================
void colorWipe(uint32_t color) {
    for (int i = 0; i < strip.numPixels(); i++) {  // For each pixel in strip...
        strip.setPixelColor(i, color);             //  Set pixel's color (in RAM)
    }
    strip.show();  //  Update strip to match
}

void fsmTick(bool level) {
    unsigned long now      = millis();  // current (relative) time in msecs.
    unsigned long waitTime = (now - _startTime);

    switch (_state) {
        case STATE_INIT:

            if (level) {
                _lastState = _state;
                _state     = STATE_DOWN;
                _startTime = now;
                _nClicks   = 0;
            }
            break;
        case (STATE_DOWN):

            if ((!level) && (waitTime < DEBOUNCE_TIME)) {
                // button was released to quickly so I assume some bouncing.
                _lastState = _state;
                _state     = _lastState;

            } else if (!level) {
                _lastState = _state;
                _state     = STATE_UP;
                _startTime = now;
            }
            break;
        case STATE_UP:
            // level is inactive
            if ((level) && (waitTime < DEBOUNCE_TIME)) {
                // button was pressed to quickly so I assume some bouncing.
                _lastState = _state;
                _state     = _lastState;

            } else if (waitTime >= DEBOUNCE_TIME) {
                // count as a short button down
                _nClicks++;
                _lastState = _state;
                _state     = STATE_COUNT;
            }  // if
            break;

        case STATE_COUNT:
            // dobounce time is over, count clicks
            if (level) {
                // button is down again
                _lastState = _state;
                _state     = STATE_DOWN;
                _startTime = now;

            } else if ((waitTime > TAP_TIME) || (_nClicks == 2)) {
                if (_nClicks == 1) {
                } else if (_nClicks == 2) {
                    doubleTapFlag = true;
                    Serial.println("double tap");
                }
                _state     = STATE_INIT;
                _lastState = STATE_INIT;
                _nClicks   = 0;
                _startTime = 0;
            }  // if
            break;
    }
}