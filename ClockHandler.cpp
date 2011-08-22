/**
 * ClockHandler.cpp
 * Datei um die Uhrzeiten zu Handeln
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    20.3.2011
 */
#include <Wire.h>
#include "ClockHandler.h"
#include "DisplayMatrix.h"
#include "DS1307.h"
#include "MyDCF77.h"
#include "DCF77Helper.h"

#define DEBUG

/**
 * Die Real-Time-Clock
 */
DS1307 ds1307(0x68);

/**
 * Die Funkuhr.
 */
#define dcf77Led      8
#define dcf77Signal   9
MyDCF77 dcf77(dcf77Signal);
DCF77Helper dcf77Helper;
/**
 * Das Rechtecksignal der RTC fuer den Interrupt
 */
#define rtcSQWLed     4
#define rtcSQWSignal  2


// Hilfsvariable, da I2C und Interrupts nicht zusammenspielen
volatile boolean needsUpdateFromRtc = true;

/**
 * Aenderung der Anzeige als Funktion fuer den Interrupt, der ueber das SQW-Signal 
 * der Real-Time-Clock gesetzt wird. Da die Wire-Bibliothek benutzt wird, kann man
 * den Interrupt nicht direkt benutzen, sondern muss eine Hilfsvariable setzen, die
 * dann in loop() ausgewertet wird.
 */
void updateFromRtc() {
  needsUpdateFromRtc = true;
}

void setUpdateFromRtc(boolean update)
{
  needsUpdateFromRtc = update;
}

// Initialisierung der BinärUhr
void initClockHandler()
{
  // starte Wire-Library als I2C-Bus Master
  Wire.begin();
  
  // DCF77-Pins konfigurieren
  pinMode(dcf77Signal, INPUT);
  digitalWrite(dcf77Signal, HIGH);
  pinMode(dcf77Led, OUTPUT);
  digitalWrite(dcf77Led, HIGH);

  // DS1307-Pins konfigurieren
  pinMode(rtcSQWSignal, INPUT);
  digitalWrite(rtcSQWSignal, HIGH);
  pinMode(rtcSQWLed, OUTPUT);
  digitalWrite(rtcSQWLed, HIGH);

  // 1Hz SQW-Signal einschalten
  ds1307.readTimeOnly();
  ds1307.writeTime();

  // den Interrupt konfigurieren
  // nicht mehr CHANGE, das sind 2 pro Sekunde
  // RISING ist einer pro Sekunde, das reicht
  attachInterrupt(0, updateFromRtc, RISING);
  
  return;
}

// Update der Uhr-Anzeige
void updateClockHandler()
{  
  // via Interrupt gesetzt ueber Flanke des SQW-Signals von der RTC
  if (needsUpdateFromRtc) {
    ds1307.readTimeOnly();
    needsUpdateFromRtc = false;
  }
  
  // Status-LEDs ausgeben
  digitalWrite(dcf77Led, digitalRead(dcf77Signal));
  digitalWrite(rtcSQWLed, digitalRead(rtcSQWSignal));

  // DCF77-Empfaenger abfragen
  if(dcf77.poll()) {
    Serial.print("Captured: ");
    Serial.println(dcf77.asString());

    if(dcf77.getYear() > 2010) {
      dcf77Helper.addSample(dcf77, ds1307);
      // stimmen die Abstaende im Array?
      if(dcf77Helper.samplesOk()) {
        ds1307.setSeconds(0);
        ds1307.setMinutes(dcf77.getMinutes());
        ds1307.setHours(dcf77.getHours());
        // wir setzen auch das Datum, dann kann man, wenn man moechte,
        // auf das Datum eingehen (spezielle Nachrichten an speziellen
        // Tagen).
        ds1307.setDate(dcf77.getDate());
        ds1307.setDayOfWeek(dcf77.getDayOfWeek());
        ds1307.setMonth(dcf77.getMonth());
        // die DS1307 moechte das Jahr zweistellig
        int y = dcf77.getYear();
        while (y > 100) {
          y = y-100;
        }
        ds1307.setYear(y);

        ds1307.writeTime();
#ifdef DEBUG
        Serial.println("DCF77-Time written to DS1307.");
#endif    
      } 
      else {
#ifdef DEBUG
        Serial.println("DCF77-Time trashed because wrong distances between timestamps.");
#endif    
      }
    }
  }
  return;
}

// Funktionen von der DS1307 Klasse. Einfach durchreichen
void readTimeOnly()
{
  ds1307.readTimeOnly();
  return;
}

void writeTime()
{
  ds1307.writeTime();
  return;
}

void setSeconds(byte seconds)
{
  ds1307.setSeconds(seconds);
  return;
}

void setMinutes(byte minutes)
{
  ds1307.setMinutes(minutes);
  return;
}

void incMinutes()
{
  ds1307.incMinutes();
  return;
}

void setHours(byte hours)
{
  ds1307.setHours(hours);
  return;
}

void incHours()
{
  ds1307.incHours();
  return;
}

void setDayOfWeek(byte dayOfWeek)
{
  ds1307.setDayOfWeek(dayOfWeek);
  return;
}

void setDate(byte date)
{
  ds1307.setDate(date);
  return;
}

void setMonth(byte month)
{
  ds1307.setMonth(month);
  return;
}

void setYear(byte year)
{
  ds1307.setYear(year);
  return;
}

byte getSeconds()
{
  return ds1307.getSeconds();
}

byte getMinutes()
{
  return ds1307.getMinutes();
}

byte getHours()
{
  return ds1307.getHours();
}

byte getDayOfWeek()
{
  return ds1307.getDayOfWeek();
}

byte getDate()
{
  return ds1307.getDate();
}

byte getMonth()
{
  return ds1307.getMonth();
}

byte getYear()
{
  return ds1307.getYear();
}
