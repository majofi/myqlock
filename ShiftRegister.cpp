/**
 * ShiftRegister.h
 *
 * Mit der ShiftRegister-Klasse wird das Ausgeben der Bitmuster auf die 
 * 74HC595-Shift-Register gekapselt.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @datum    24.2.2011
 *
 * Versionshistorie:
 * V 1.1:  - Schnelle DigitalWrite-Methoden eingefuehrt.
 *           Ueber die Definition von SHIFTREGISTER_TURBO
 *           kann es eingeschaltet werden.
 */
#include "pins_arduino.h"
#include "ShiftRegister.h"

// #define DEBUG

/**
 * Initialisierung mit den Pins fuer Serial-Data, Serial-Clock und Store-Clock (Latch)
 */
ShiftRegister::ShiftRegister(int dataPin, int clockPin, int latchPin) {
  // slow version
  _dataPin = dataPin;
  _clockPin = clockPin;
  _latchPin = latchPin;
  pinMode(_latchPin, OUTPUT);
  pinMode(_clockPin, OUTPUT);
  pinMode(_dataPin, OUTPUT);

  // fast version
  _dataBit=digitalPinToBitMask(dataPin);
  _dataPort= digitalPinToPort(dataPin);
  _dataOut=portOutputRegister(_dataPort);    

  _clockBit=digitalPinToBitMask(clockPin);
  _clockPort= digitalPinToPort(clockPin);
  _clockOut=portOutputRegister(_clockPort);

  _latchBit=digitalPinToBitMask(latchPin);
  _latchPort= digitalPinToPort(latchPin);
  _latchOut=portOutputRegister(_latchPort);
}

/**
 * Ein WORD (16 Bit) ausgeben
 */
void ShiftRegister::shiftOut(word data) {
#ifdef SHIFTREGISTER_TURBO
  for (word b = 0; b < 16; b++) {
    fastDigitalWriteToClock(LOW);
    if (data & (1 << b)) {
      fastDigitalWriteToData(HIGH);
    } 
    else {
      fastDigitalWriteToData(LOW);
    }
    fastDigitalWriteToClock(HIGH);
  }
#else
  for (word b = 0; b < 16; b++) {
    digitalWrite(_clockPin, LOW);
    if (data & (1 << b)) {
      digitalWrite(_dataPin, HIGH);
    } 
    else {
      digitalWrite(_dataPin, LOW);
    }
    digitalWrite(_clockPin, HIGH);
  }
#endif  
}

/**
 * Vorbereitung fuer die Ausgabe
 */
void ShiftRegister::prepareShiftregisterWrite() {
#ifdef SHIFTREGISTER_TURBO
  // fastDigitalWriteToData(LOW);
  // fastDigitalWriteToClock(LOW);
  fastDigitalWriteToLatch(LOW);
#else
  // digitalWrite(_dataPin, LOW);
  // digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, LOW);
#endif
}

/**
 * Abschliessen der Ausgabe
 */
void ShiftRegister::finishShiftregisterWrite() {
#ifdef SHIFTREGISTER_TURBO
  // fastDigitalWriteToData(LOW);
  // fastDigitalWriteToClock(LOW);
  fastDigitalWriteToLatch(HIGH);
#else
  // digitalWrite(_dataPin, LOW);
  // digitalWrite(_clockPin, LOW);
  digitalWrite(_latchPin, HIGH);
#endif
}

/**
 * Digitale Ausgaenge im Turbo-Mode!
 */
void ShiftRegister::fastDigitalWriteToData(uint8_t val)
{
  if (val == LOW) {
    uint8_t oldSREG = SREG;
    cli();
    *_dataOut &= ~_dataBit;
    SREG = oldSREG;
  } 
  else {
    uint8_t oldSREG = SREG;
    cli();
    *_dataOut |= _dataBit;
    SREG = oldSREG;
  }
}

void ShiftRegister::fastDigitalWriteToLatch(uint8_t val)
{
  if (val == LOW) {
    uint8_t oldSREG = SREG;
    cli();
    *_latchOut &= ~_latchBit;
    SREG = oldSREG;
  } 
  else {
    uint8_t oldSREG = SREG;
    cli();
    *_latchOut |= _latchBit;
    SREG = oldSREG;
  }
}

void ShiftRegister::fastDigitalWriteToClock(uint8_t val)
{
  if (val == LOW) {
    uint8_t oldSREG = SREG;
    cli();
    *_clockOut &= ~_clockBit;
    SREG = oldSREG;
  } 
  else {
    uint8_t oldSREG = SREG;
    cli();
    *_clockOut |= _clockBit;
    SREG = oldSREG;
  }
}

