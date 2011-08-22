/**
 * MyDCF77.h
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
#ifndef MYDCF77_H
#define MYDCF77_H

// Bei einer Verwendung eines analogen Pins die
// naechsten zwei Zeilen einkommentieren.
// #define MyDCF77_SIGNAL_IS_ANALOG
// #define MyDCF77_ANALOG_SIGNAL_TRESHOLD 800

// Als Synchronisierungs-Marke wird eine Pause von 1 Sekunde laenge 
// 'gesendet'. Allerdings haben wir ja vorher noch eine Absenkung vom 
// Checkbit P3 gehabt, daher 1800.
#define MYDCF77_SYNC_PAUSE_TRESHOLD 1700
#define MYDCF77_200MS_TRESHOLD 190
#define MYDCF77_100MS_TRESHOLD 90
#define MYDCF77_TELEGRAMMLAENGE 59

#include "WProgram.h"

class MyDCF77 {
public:
  MyDCF77(int signalPin);

  boolean poll();

  int getMinutes();
  int getHours();

  int getDate();
  int getDayOfWeek();
  int getMonth();
  int getYear();

  char* asString();

private:
  int _signalPin;

  int _minutes;
  int _hours;

  int _date;
  int _dayOfWeek;
  int _month;
  int _year;

  char _cDateTime[17];

  byte _bits[MYDCF77_TELEGRAMMLAENGE];
  int _bitsPointer;

  boolean signal();
  boolean decode();

  // Hilfsvariablen fuer den Zustandsautomaten
  boolean _oldSignalState;
  unsigned long _signalStartedAtMillis;
  unsigned long _signalStoppedAtMillis;
};

#endif

