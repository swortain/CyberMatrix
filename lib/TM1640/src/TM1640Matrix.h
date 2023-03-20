/*
 * @功能:
 * @Author: swortain
 * @Date: 2023-02-25 21:24:17
 * @LastEditTime: 2023-03-13 15:34:06
 */
#ifndef _TM1640_MATRIX_H
#define _TM1640_MATRIX_H

#include <Adafruit_GFX.h>

#include "TM1640.h"

#define TM1640_MATRIX_MAXCOLUMNS 60

#define MATRIX_DIRECTION_COL2ROW 0
#define MATRIX_DIRECTION_ROW2COL 1

class TM1640Matrix : public Adafruit_GFX {
   public:
    TM1640Matrix(void** pTM1640s, byte nCols, byte nRows, byte nRowNum, byte nColNum, const byte* rowPinMap, const byte* colPinmap, byte nDirection);
    void         setLine(byte nLine, byte bPixels);
    void         setAll(bool fOn);
    void         drawPixel(int16_t x, int16_t y, uint16_t color);
    bool         getPixel(int16_t x, int16_t y);
    void         printClockDataPin();
    void         displayBuffer();
    inline byte  getNumRows() { return (_nRows); }
    inline byte  getNumCols() { return (_nCols); }
    inline void  drawDirect() { drawPixelOutputEnable = true; }
    inline void  drawBuffer() { drawPixelOutputEnable = false; }
    inline void  copyOutputBuffer() { memcpy(_outputLineBuffer, _outputLine, sizeof(_outputLine)); }
    inline byte* getOutputLine() { return _outputLine; }
    inline byte* getOutputLineBuffer() { return _outputLineBuffer; }
    inline byte  getMatrixColumns() { return TM1640_MATRIX_MAXCOLUMNS; }

        protected : void setLine(TM1640* _pTM1640, byte nLine, byte bPixels);
    TM1640**    _pTM1640s;
    byte        _nCols;
    byte        _nRows;
    byte        _nDirection;
    byte        _nRowNum;
    byte        _nColNum;
    byte        _nColPerUnit;
    byte        _nRowPerUnit;
    const byte* _rowPinmap;
    const byte* _colPinmap;
    byte        _outputLine[TM1640_MATRIX_MAXCOLUMNS]       = {0};
    byte        _outputLineBuffer[TM1640_MATRIX_MAXCOLUMNS] = {0};
    bool        drawPixelOutputEnable                       = true;
};
#endif
