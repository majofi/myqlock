/**
 * ClockHandler.h
 * Datei um die Uhrzeiten zu Handeln
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    20.3.2011
 */
#ifndef CLOCKHANDLER_H
#define CLOCKHANDLER_H

#include "WProgram.h"

void initClockHandler();
void updateClockHandler();
void setUpdateFromRtc(boolean update);

void readTimeOnly();
void writeTime();

void setSeconds(byte seconds);
void setMinutes(byte minutes);
void incMinutes();
void setHours(byte hours);
void incHours();
void setDayOfWeek(byte dayOfWeek);
void setDate(byte date);
void setMonth(byte month);
void setYear(byte year);

byte getSeconds();
byte getMinutes();
byte getHours();
byte getDayOfWeek();
byte getDate();
byte getMonth();
byte getYear();

#endif

