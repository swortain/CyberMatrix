#ifndef TM1640_h
#define TM1640_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM16xx.h"

#define TM1640_MAX_POS 16

class TM1640 : public TM16xx
{
  public:
		// Instantiate a TM1640 module specifying data and clock pins, number of digits, the display state, the starting intensity (0-7).
  	TM1640(byte dataPin, byte clockPin, byte numDigits=16, boolean activateDisplay = true, byte intensity = 7);

  protected:
    //virtual void bitDelay();
    virtual void start();
    virtual void stop();
    virtual void send(byte data);
};

#endif
