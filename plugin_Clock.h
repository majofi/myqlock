/**
 * Clock.h
 * Anzeige der Uhrzeit
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    20.3.2011
 */
#ifndef CLOCK_H
#define CLOCK_H

#include "WProgram.h"
#include "Button.h"

void showClock();
void initClock(int x, int y);
void updateClock(int timeDiff);
void buttonClock(Button btn, byte id);

#endif

