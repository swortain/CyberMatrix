/*
 * @Date: 2023-03-16 18:02:38
 * @LastEditors: swortain jianshan_54@126.com
 * @LastEditTime: 2023-03-16 18:12:55
 * @FilePath: \221025_led_matrix\src\matrixPrint.cpp
 * @Description: 
 */
#include "matrixPrint.h"

void PrintTimeSeparator(String _s_pre, String _s) {
    Serial.print(_s_pre);
    Serial.print("==>");
    Serial.println(_s);
    for (byte idx = 0; idx < max(_s_pre.length(), _s.length()); idx++) {
        if (_s_pre[idx] != _s[idx]) {
            ledMatrix.drawDirect();
            ledMatrix.fillRect(idx * MATRIX_UNIT_COL, 0, MATRIX_UNIT_COL,
                               MATRIX_UNIT_ROW, 0x00);
            ledMatrix.setCursor(idx * MATRIX_UNIT_COL, MATRIX_UNIT_ROW - 1);
            ledMatrix.print(_s[idx]);

            Serial.print('\'');
            Serial.print(_s_pre[idx]);
            Serial.print('\'');
            Serial.print("-->");
            Serial.print('\'');
            Serial.println(_s[idx]);
            Serial.print('\'');
            Serial.print("rect: ");
            Serial.print(idx * MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.print(MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.println(MATRIX_UNIT_ROW);
            Serial.print("Cursor: ");
            Serial.print(idx * MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.println(MATRIX_UNIT_ROW - 1);
        }
    }
}

void PrintTimeStr(String _s_pre, String _s) {
    Serial.print(_s_pre);
    Serial.print("==>");
    Serial.println(_s);

    for (byte idx = 0; idx < max(_s_pre.length(), _s.length()); idx++) {
        if (_s_pre[idx] != _s[idx]) {
            ledMatrix.drawBuffer();
            ledMatrix.copyOutputBuffer();

            ledMatrix.fillRect(idx * MATRIX_UNIT_COL, 0, MATRIX_UNIT_COL,
                               MATRIX_UNIT_ROW, 0x00);
            ledMatrix.setCursor(idx * MATRIX_UNIT_COL, MATRIX_UNIT_ROW - 1);
            ledMatrix.print(_s[idx]);

            ledMatrix.drawDirect();
            PointbyPoint(ledMatrix.getOutputLine(), ledMatrix.getOutputLineBuffer(), ledMatrix.getMatrixColumns(), setLineCallback);

            Serial.print('\'');
            Serial.print(_s_pre[idx]);
            Serial.print('\'');
            Serial.print("-->");
            Serial.print('\'');
            Serial.println(_s[idx]);
            Serial.print('\'');
            Serial.print("rect: ");
            Serial.print(idx * MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.print(MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.println(MATRIX_UNIT_ROW);
            Serial.print("Cursor: ");
            Serial.print(idx * MATRIX_UNIT_COL);
            Serial.print(",");
            Serial.println(MATRIX_UNIT_ROW - 1);
        }
    }

    delay(1000);
}

void setLineCallback(byte nLine, byte bPixels) {
    ledMatrix.setLine(nLine, bPixels);
}