/*
 * @功能:
 * @Author: swortain
 * @Date: 2023-02-25 21:24:17
 * @LastEditTime: 2023-03-13 20:00:26
 */
#include "TM1640Matrix.h"
// #define DEBUG

TM1640Matrix::TM1640Matrix(void** pTM1640s, byte nCols, byte nRows, byte nRowNum, byte nColNum, const byte* rowPinMap, const byte* colPinmap, byte nDirection)
    : Adafruit_GFX(nCols, nRows) {
    _pTM1640s    = (TM1640**)pTM1640s;
    _nCols       = nCols;            // 35
    _nRows       = nRows;            // 11
    _nRowNum     = nRowNum;          // 1
    _nColNum     = nColNum;          // 5
    _nRowPerUnit = nRows / nRowNum;  // 11
    _nColPerUnit = nCols / nColNum;  // 7
    _nDirection  = nDirection;
    _rowPinmap   = rowPinMap;
    _colPinmap   = colPinmap;

    // Serial.begin(115200);
    // Serial.print("Init Matrix: ");
    // Serial.print("_nCols: ");
    // Serial.print(_nCols);
    // Serial.print(",_nRows: ");
    // Serial.print(_nRows);
    // Serial.print(",_nRowNum: ");
    // Serial.print(_nRowNum);
    // Serial.print(",_nColNum: ");
    // Serial.print(_nColNum);
    // Serial.print(",_nRowPerUnit: ");
    // Serial.print(_nRowPerUnit);
    // Serial.print(",_nColPerUnit: ");
    // Serial.print(_nColPerUnit);
    // Serial.println();
}

void TM1640Matrix:: setLine(TM1640* _pTM1640, byte nLine, byte bPixels) {
    if (_nDirection == MATRIX_DIRECTION_ROW2COL) {
    } else if (_nDirection == MATRIX_DIRECTION_COL2ROW) {
        _pTM1640->setSegments(bPixels, nLine);
    } else {
        Serial.println("Error: Matrix Direction Error.");
        return;
    }
}
void TM1640Matrix::setLine(byte nLine, byte bPixels) {
    if (_nDirection == MATRIX_DIRECTION_ROW2COL) {
    } else if (_nDirection == MATRIX_DIRECTION_COL2ROW) {
        byte rowIdx = 0; //FIXME: 对于二维矩阵需要补全计算
        byte    colIdx  = nLine / _nRowPerUnit;
        TM1640* _tm1640 = ((TM1640*)_pTM1640s + rowIdx * _nColNum + colIdx);

        _tm1640->setSegments(bPixels, nLine % _nRowPerUnit);
    } else {
        Serial.println("Error: Matrix Direction Error.");
        return;
    }
}

void TM1640Matrix::setAll(bool fOn) {
    if (_nDirection == MATRIX_DIRECTION_ROW2COL) {
    } else if (_nDirection == MATRIX_DIRECTION_COL2ROW) {
        for (byte rowIdx = 0; rowIdx < _nRowNum; rowIdx++) {
            for (byte colIdx = 0; colIdx < _nColNum; colIdx++) {
                TM1640* _tm1640 = ((TM1640*)_pTM1640s + rowIdx * _nColNum + colIdx);
                for (byte nLine = 0; nLine < _nRows; nLine++) {
                    for (byte nLine = 0; nLine < _nCols; nLine++) {
                        setLine(_tm1640, nLine, fOn ? 0xFF : 0);
                    }
                }
            }
        }
        for (byte nLine = 0; nLine < TM1640_MATRIX_MAXCOLUMNS; nLine++) {
            _outputLine[nLine] = fOn ? 0xFF : 0x00;
        }
    } else {
        Serial.println("Error: Matrix Direction Error.");
        return;
    }
}

void TM1640Matrix::displayBuffer() {}

void TM1640Matrix::drawPixel(int16_t x, int16_t y, uint16_t c) {
    bool fOn = c >= 0xff ? true : false;
    if ((x >= _nCols) || (y >= _nRows)) {
        Serial.println("Error: [drawPixel]x or y overstep the boundary");
        return;
    }

    if (_nDirection == MATRIX_DIRECTION_ROW2COL) {
    } else if (_nDirection == MATRIX_DIRECTION_COL2ROW) {
        byte rowIdx = x / _nColPerUnit;
        byte colIdx = y / _nRowPerUnit;

        TM1640* _tm1640 = ((TM1640*)_pTM1640s + colIdx * _nColNum + rowIdx);
        byte    bufferIdx;
        bufferIdx = _colPinmap[y % _nRowPerUnit] +
                    _nRowPerUnit * (colIdx * _nColNum + rowIdx);

        if (bufferIdx > TM1640_MATRIX_MAXCOLUMNS) {
            Serial.println("Error: [drawPixel]_outputLine is out of boundary");
            return;
        }

#ifdef DEBUG
        Serial.print("[drawPixel]");
        Serial.print("rowIdx: ");
        Serial.print(rowIdx);
        Serial.print(", colIdx: ");
        Serial.print(colIdx);
        Serial.print(", y % _nRowPerUnit: ");
        Serial.print(y % _nRowPerUnit);
        Serial.print(", x % _nColPerUnit: ");
        Serial.print(x % _nColPerUnit);
        Serial.print(", _colPinmap[y % _nRowPerUnit]: ");
        Serial.print(_colPinmap[y % _nRowPerUnit]);
        Serial.print(", _rowPinmap[x % _nColPerUnit]: ");
        Serial.print(_rowPinmap[x % _nColPerUnit]);
        Serial.print(", bufferIdx: ");
        Serial.print(bufferIdx);
        Serial.print(", buffer[idx]: ");
        Serial.print(_outputLine[bufferIdx]);
        Serial.print(", drawPixelOutputEnable:");
        Serial.print(drawPixelOutputEnable);
        Serial.println();
#endif

        if (drawPixelOutputEnable) {
            byte btColumn = _outputLine[bufferIdx];
            if (fOn) {
                btColumn = btColumn | _BV(_rowPinmap[x % _nColPerUnit]);

            } else {
                btColumn = btColumn & ~_BV(_rowPinmap[x % _nColPerUnit]);
            }
            _outputLine[bufferIdx] = btColumn;

            setLine(_tm1640, _colPinmap[y % _nRowPerUnit], btColumn);

        } else {
            byte btColumn = _outputLineBuffer[bufferIdx];
            if (fOn) {
                btColumn = btColumn | _BV(_rowPinmap[x % _nColPerUnit]);

            } else {
                btColumn = btColumn & ~_BV(_rowPinmap[x % _nColPerUnit]);
            }
            _outputLineBuffer[bufferIdx] = btColumn;
        }
    } else {
        Serial.println("Error: Matrix Direction Error.");
        return;
    }
}

bool TM1640Matrix::getPixel(int16_t x, int16_t y) {
    if ((x >= _nCols) || (y >= _nRows)) {
        return 0;
    }
    byte rowIdx    = x / _nColPerUnit;
    byte colIdx    = y / _nRowPerUnit;
    byte bufferIdx = _colPinmap[y % _nRowPerUnit] +
                     _nRowPerUnit * (colIdx * _nColNum + rowIdx);

#ifdef DEBUG
    Serial.print("[getPixel]x: ");
    Serial.print(x);
    Serial.print(',');
    Serial.print(", y: ");
    Serial.print(y);
    Serial.print(", rowIdx: ");
    Serial.print(rowIdx);
    Serial.print(", colIdx: ");
    Serial.print(colIdx);
    Serial.print(", _nColPerUnit: ");
    Serial.print(_nColPerUnit);
    Serial.print(", _nRowPerUnit: ");
    Serial.print(_nRowPerUnit);
    Serial.print(", bufferIdx: ");
    Serial.print(bufferIdx);
    Serial.print(", _outputLine[bufferIdx]: ");
    Serial.print(_outputLine[bufferIdx]);
    Serial.println();
#endif

    return ((_outputLine[bufferIdx] & _BV(_rowPinmap[x % _nColPerUnit])) != 0);
}

void TM1640Matrix::printClockDataPin() {
    for (byte rowIdx = 0; rowIdx < _nRowNum; rowIdx++) {
        for (byte colIdx = 0; colIdx < _nColNum; colIdx++) {
            TM1640* _tm1640 = ((TM1640*)_pTM1640s + rowIdx * _nColNum + colIdx);
            Serial.print("Row: ");
            Serial.print(rowIdx);
            Serial.print(", Col: ");
            Serial.print(colIdx);
            Serial.print(", DataPin: ");
            Serial.print(_tm1640->getDataPin());
            Serial.print(", ClockPin: ");
            Serial.print(_tm1640->getClockPin());
            Serial.print(", Addr: ");
            Serial.print((int)_tm1640, HEX);
            Serial.println();
        }
    }
}
