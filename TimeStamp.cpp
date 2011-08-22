/**
 * TimeStamp.cpp
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
#include "TimeStamp.h"

// #define DEBUG

TimeStamp::TimeStamp() {
}

TimeStamp::TimeStamp(MyDCF77 dcf77){
  setFrom(dcf77);
}

TimeStamp::TimeStamp(DS1307 ds1307){
  setFrom(ds1307);
}

int TimeStamp::getMinutes(){
  return _minutes;
}

int TimeStamp::getMinutesOfDay() {
  return _minutes + 60 * _hours;
}

int TimeStamp::getHours(){
  return _hours;
}

int TimeStamp::getDate(){
  return _date;
}

int TimeStamp::getDayOfWeek(){
  return _dayOfWeek;
}

int TimeStamp::getMonth(){
  return _month;
}

int TimeStamp::getYear(){
  return _year;
}

void TimeStamp::setFrom(MyDCF77 dcf77){
  _minutes = dcf77.getMinutes();
  _hours = dcf77.getHours();
  _date = dcf77.getDate();
  _dayOfWeek = dcf77.getDayOfWeek();
  _month=dcf77.getMonth();
  _year=dcf77.getYear();
}

void TimeStamp::setFrom(DS1307 ds1307){
  _minutes = ds1307.getMinutes();
  _hours = ds1307.getHours();
  _date = ds1307.getDate();
  _dayOfWeek = ds1307.getDayOfWeek();
  _month=ds1307.getMonth();
  _year=ds1307.getYear();
}

void TimeStamp::set(int minutes, int hours, int date, int dayOfWeek, int month, int year){
  _minutes = minutes;
  _hours = hours;
  _date = date;
  _dayOfWeek = dayOfWeek;
  _month = month;
  _year = year;
}

/**
 * Die Zeit als String bekommen
 */
char* TimeStamp::asString() {
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

