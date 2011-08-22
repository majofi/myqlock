/**
 * TimeStamp.h
 * Klasse fuer die Kapselung eines Zeitstempels.
 *
 * @mc       Arduino/RBBB
 * @autor    Christian Aschoff / caschoff _AT_ mac _DOT_ com
 * @version  1.1
 * @datum    2.3.2011
 *
 * Versionshistorie:
 * V 1.1:  - Feher in toString() behoben.
 */
#ifndef TIMESTAMP_H
#define TIMESTAMP_H

#include "WProgram.h"
#include "MyDCF77.h"
#include "DS1307.h"

class TimeStamp {
public:
  TimeStamp();
  TimeStamp(MyDCF77 dcf77);
  TimeStamp(DS1307 ds1307);

  int getMinutes();
  int getMinutesOfDay();
  int getHours();

  int getDate();
  int getDayOfWeek();
  int getMonth();
  int getYear();

  void setFrom(MyDCF77 dcf77);
  void setFrom(DS1307 ds1307);
  void set(int minutes, int hours, int date, int dayOfWeek, int month, int year);

  char* asString();

private:
  int _minutes;
  int _hours;

  int _date;
  int _dayOfWeek;
  int _month;
  int _year;
  
  char _cDateTime[17];
};

#endif


