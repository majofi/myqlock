/**
 * Seconds.cpp
 * Anzeige des Sekundenanteils der Uhrzeit
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#ifndef SECONDS_H
#define SECONDS_H

#include "WProgram.h"
#include "Button.h"

void showSeconds();
void initSeconds(int x, int y);
void updateSeconds(int timeDiff);
void buttonSeconds(Button btn, byte id);

#endif

