
#if defined(ARDUINO) && ARDUINO >= 100
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "TM1640.h"

TM1640::TM1640(byte dataPin, byte clockPin, byte numDigits, boolean activateDisplay, byte intensity)
	: TM16xx(dataPin, clockPin, dataPin, TM1640_MAX_POS, numDigits, activateDisplay, intensity)
{ // NOTE: Like the TM1637, the TM1640 only has DIO and CLK. Therefor the DIO-pin is initialized as strobe in the constructor
	clearDisplay();
	setupDisplay(activateDisplay, intensity);
}

/*
void TM1640::bitDelay()
{
	//delayMicroseconds(5);
}
*/

void TM1640::start()
{	// if needed derived classes can use different patterns to start a command (eg. for TM1637)
	// Datasheet: The starting condition of data input is: when CLK is high, the DIN becomes low from high;
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  bitDelay();
}

void TM1640::stop()
{	// if needed derived classes can use different patterns to stop a command (eg. for TM1637)
	// datasheet: the ending condition is: when CLK is high, the DIN becomes high from low.
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, HIGH);
  bitDelay();
}

void TM1640::send(byte data)
{
	// MOLE 180514: TM1640 wants data and clock to be low after sending the data
	TM16xx::send(data);
  digitalWrite(clockPin, LOW);		// first clock low
  digitalWrite(dataPin, LOW);			// then data low
  bitDelay();
}
