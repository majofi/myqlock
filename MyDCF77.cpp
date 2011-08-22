/**
 * MyDCF77.cpp
 *
 * Klasse fuer die Auswertung des DCF77-Zeitsignals.
 * Diese Klasse geht von einem 'sauberen' Signal aus.
 * Bei schlechten Empfangsbedingungen muesste man eine
 * Unschaerfe zulassen.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @datum    2.3.2011
 *
 * Versionshistorie:
 * V 1.1:  - Fehler in der Array-Laenge und in toString() behoben.
 */
#include "MyDCF77.h"

//#define DEBUG

/**
 * Initialisierung mit dem Pin, an dem das Signal des Empfaengers anliegt
 */
MyDCF77::MyDCF77(int signalPin) {
  _signalPin = signalPin;
#ifndef MyDCF77_SIGNAL_IS_ANALOG
  pinMode(_signalPin, INPUT);
#endif

  for(int i=0; i<MYDCF77_TELEGRAMMLAENGE; i++) {
    _bits[i] = 0;
  }
  _bitsPointer = 0;
  _oldSignalState = false;
  _signalStartedAtMillis = 0;
  _signalStoppedAtMillis = 0;
}

/**
 * Liegt ein Signal vom Empfaenger an?
 */
boolean MyDCF77::signal() {
#ifdef MyDCF77_SIGNAL_IS_ANALOG
  return analogRead(_signlPin) > MyDCF77_ANALOG_SIGNAL_TRESHOLD;
#else
  return digitalRead(_signalPin) == HIGH;
#endif
}

/**
 * Aufsammeln der Bits des DCF77-Signals. Im Prinzip ein grosser Zustandsautomat.
 * Zurueckgegeben wird ein Wahrheitswert. 
 * TRUE bedeutet, das Zeittelegramm wurde korrekt ausgewertet, die Zeitdaten 
 * koennen mit den Gettern abgerufen werden.
 * FALSE bedeutet, die Auswertung laeuft oder war falsch, die Getter liefern
 * alte Informationen.
 */
boolean MyDCF77::poll() {
  boolean retVal = false;

  if(signal()) {
    if(!_oldSignalState) {
      // vorher kein Signal -> ansteigende Flanke -> Sekundenbeginn
      _oldSignalState = true;
      _signalStartedAtMillis = millis();
    }
  } 
  else { // if signal()
    if (_oldSignalState) {
      // vorher Signal -> abfallende Flanke -> Bitinfo fertig (Signaldauer 100ms = 0; 200ms = 1)
      unsigned long duration = millis()-_signalStartedAtMillis;
      if(duration > MYDCF77_200MS_TRESHOLD) {
        // Bit ist 1
#ifdef DEBUG
        Serial.print("1");
#endif
        if(_bitsPointer < MYDCF77_TELEGRAMMLAENGE) {
          _bits[_bitsPointer] = 1;
        }
        _bitsPointer++;
        _oldSignalState = false;
        _signalStoppedAtMillis = millis();
      } 
      else if (duration > MYDCF77_100MS_TRESHOLD) {
        // Bit ist 0
#ifdef DEBUG
        Serial.print("0");
#endif
        if(_bitsPointer < MYDCF77_TELEGRAMMLAENGE) {
          _bits[_bitsPointer] = 0;
        }
        _bitsPointer++;
        _oldSignalState = false;
        _signalStoppedAtMillis = millis();
      } 
      else {
        // Schrott empfangen
#ifdef DEBUG
        Serial.print("-");
#endif
      }
    }
  }

  // Synczeitpunkt erreicht oder Telegrammlaenge korrekt...
  if(millis()-_signalStoppedAtMillis > MYDCF77_SYNC_PAUSE_TRESHOLD) {
#ifdef DEBUG
    Serial.print("\nDuration (pause): ");
    Serial.println(millis()-_signalStoppedAtMillis);
    Serial.print("Bitcount: ");
    Serial.println(_bitsPointer);
#endif

    if(_bitsPointer == MYDCF77_TELEGRAMMLAENGE) {
      retVal = decode();
    }

    // Bitarray und Pointer zuruecksetzen...
    for(int i=0; i<MYDCF77_TELEGRAMMLAENGE; i++) {
      _bits[i] = 0;
    }
    _bitsPointer = 0;
    _oldSignalState = false;
    _signalStartedAtMillis = millis();
    _signalStoppedAtMillis = millis();
    
#ifdef DEBUG
    // Von oben nach unten lesen. Zuerst Bitnummer, dann Bedeutung.
    Serial.println("          1111111111222222222233333333334444444444555555555");
    Serial.println("01234567890123456789012345678901234567890123456789012345678");
    Serial.println("---------------RAZZASMMMMMMMPSSSSSSPDDDDDDWWWMMMMMYYYYYYYYP");
    Serial.println("                1122 12481241124812212481212412481124812483");
    Serial.println("                         000     00     00       0    0000 ");
#endif    
  }

  return retVal;
}

/**
 * Decodierung des Telegramms...
 */
boolean MyDCF77::decode() {
  int c = 0; // bitcount for checkbit
  boolean ok = true;

#ifdef DEBUG
  Serial.println("Decoding telegram...");
#endif

  if(_bits[20] != 1) {
    ok = false;
#ifdef DEBUG
    Serial.println("Check-bit S failed.");
#endif
  }

  if(_bits[17] == _bits[18]) {
    ok = false;
#ifdef DEBUG
    Serial.println("Check Z1 != Z2 failed.");
#endif
  }

  //
  // minutes
  //
  _minutes = 0;
  c = 0;
  if(_bits[21] == 1) {
    c++;
    _minutes += _bits[21] * 1;
  }
  if(_bits[22] == 1) {
    c++;
    _minutes += _bits[22] * 2;
  }
  if(_bits[23] == 1) {
    c++;
    _minutes += _bits[23] * 4;
  }
  if(_bits[24] == 1) {
    c++;
    _minutes += _bits[24] * 8;
  }
  if(_bits[25] == 1) {
    c++;
    _minutes += _bits[25] * 10;
  }
  if(_bits[26]) {
    c++;
    _minutes += _bits[26] * 20;
  }
  if(_bits[27]) {
    c++;
    _minutes += _bits[27] * 40;
  }
#ifdef DEBUG
  Serial.print("Minutes: ");
  Serial.println(_minutes);
#endif
  if((c + _bits[28]) % 2 != 0) {
    ok = false;
#ifdef DEBUG
    Serial.println("Check-bit P1: minutes failed.");
#endif
  }

  //
  // hour
  //
  _hours = 0;
  c = 0;
  if(_bits[29] == 1) {
    c++;
    _hours += _bits[29] * 1;
  }
  if (_bits[30] == 1) {
    c++;
    _hours += _bits[30] * 2;
  }
  if (_bits[31] == 1) {
    c++;
    _hours += _bits[31] * 4;
  }
  if (_bits[32] == 1) {
    c++;
    _hours += _bits[32] * 8;
  }
  if (_bits[33] == 1) {
    c++;
    _hours += _bits[33] * 10;
  }
  if (_bits[34] == 1) {
    c++;
    _hours += _bits[34] * 20;
  }
#ifdef DEBUG
  Serial.print("Hours: ");
  Serial.println(_hours);
#endif
  if((c + _bits[35]) % 2 != 0) {
    ok = false;
#ifdef DEBUG
    Serial.println("Check-bit P2: hours failed.");
#endif
  }

  //
  // date
  //
  _date = 0;
  c = 0;
  if(_bits[36] == 1) {
    c++;
    _date += _bits[36] * 1;
  }
  if(_bits[37] == 1) {
    c++;
    _date += _bits[37] * 2;
  }
  if(_bits[38] == 1) {
    c++;
    _date += _bits[38] * 4;
  }
  if(_bits[39] == 1) {
    c++;
    _date += _bits[39] * 8;
  }
  if(_bits[40] == 1) {
    c++;
    _date += _bits[40] * 10;
  }
  if(_bits[41] == 1) {
    c++;
    _date += _bits[41] * 20;
  }
#ifdef DEBUG
  Serial.print("Date: ");
  Serial.println(_date);
#endif

  //
  // day of week
  //
  _dayOfWeek = 0;
  if(_bits[42] == 1) {
    c++;
    _dayOfWeek += _bits[42] * 1;
  }
  if(_bits[43] == 1) {
    c++;
    _dayOfWeek += _bits[43] * 2;
  }
  if(_bits[44] == 1) {
    c++;
    _dayOfWeek += _bits[44] * 4;
  }
#ifdef DEBUG
  Serial.print("Day of week: ");
  Serial.println(_dayOfWeek);
#endif

  //
  // month
  //
  _month = 0;
  if(_bits[45] == 1) {
    c++;
    _month += _bits[45] * 1;
  }
  if(_bits[46] == 1) {
    c++;
    _month += _bits[46] * 2;
  }
  if(_bits[47] == 1) {
    c++;
    _month += _bits[47] * 4;
  }
  if(_bits[48] == 1) {
    c++;
    _month += _bits[48] * 8;
  }
  if(_bits[49] == 1) {
    c++;
    _month += _bits[49] * 10;
  }
#ifdef DEBUG
  Serial.print("Month: ");
  Serial.println(_month);
#endif

  //
  // year
  //
  _year = 0;
  if(_bits[50] == 1) {
    c++;
    _year += _bits[50] * 1;
  }
  if(_bits[51] == 1) {
    c++;
    _year += _bits[51] * 2;
  }
  if(_bits[52] == 1) {
    c++;
    _year += _bits[52] * 4;
  }
  if(_bits[53] == 1) {
    c++;
    _year += _bits[53] * 8;
  }
  if(_bits[54] == 1) {
    c++;
    _year += _bits[54] * 10;
  }
  if(_bits[55] == 1) {
    c++;
    _year += _bits[55] * 20;
  }
  if(_bits[56] == 1) {
    c++;
    _year += _bits[56] * 40;
  }
  if(_bits[57] == 1) {
    c++;
    _year += _bits[57] * 80;
  }
  _year += 2000;
  if (_year < 2011) {
    ok = false;
#ifdef DEBUG
    Serial.print("Check year >= 2011 failed.");
#endif
  }
#ifdef DEBUG
  Serial.print("Year: ");
  Serial.println(_year);
#endif

  if((c + _bits[58]) % 2 != 0) {
    ok = false;
#ifdef DEBUG
    Serial.println("Check-bit P3: date failed.");
#endif
  }

  if (!ok) {
    // discard date...
    _minutes = 0;
    _hours = 0;
    _date = 0;
    _dayOfWeek = 0;
    _month = 0;
    _year = 0;
  }

  return ok;
}

/**
 * Das Zeittelegramm als String bekommen
 */
char* MyDCF77::asString() {
  _cDateTime[0] = 0;
  char temp[5];

  // build the string...        
  if (_hours < 10) {
    sprintf(temp, "0%d:", _hours);
    strncat(_cDateTime, temp, strlen(temp));
  } 
  else {
    sprintf(temp, "%d:", _hours);
    strncat(_cDateTime, temp, strlen(temp));
  }

  if (_minutes < 10) {
    sprintf(temp, "0%d ", _minutes);
    strncat(_cDateTime, temp, strlen(temp));
  } 
  else {
    sprintf(temp, "%d ", _minutes);
    strncat(_cDateTime, temp, strlen(temp));
  }

  if (_date < 10) {
    sprintf(temp, "0%d.", _date);
    strncat(_cDateTime, temp, strlen(temp));
  } 
  else {
    sprintf(temp, "%d.", _date);
    strncat(_cDateTime, temp, strlen(temp));
  }

  if (_month < 10) {
    sprintf(temp, "0%d.", _month);
    strncat(_cDateTime, temp, strlen(temp));
  } 
  else {
    sprintf(temp, "%d.", _month);
    strncat(_cDateTime, temp, strlen(temp));
  }

  sprintf(temp, "%d", _year);
  strncat(_cDateTime, temp, strlen(temp));

  return _cDateTime;
}

//
// Getter
//
int MyDCF77::getMinutes() {
  return _minutes;
}

int MyDCF77::getHours() {
  return _hours;
}

int MyDCF77::getDate() {
  return _date;
}

int MyDCF77::getDayOfWeek() {
  return _dayOfWeek;
}

int MyDCF77::getMonth() {
  return _month;
}

int MyDCF77::getYear() {
  return _year;
}

