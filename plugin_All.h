/**
 * All.cpp
 * Alle LEDs leuchten lassen
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#ifndef ALL_H
#define ALL_H

#include "WProgram.h"
#include "Button.h"

void showAll();
void initAll(int x, int y);
void updateAll(int timeDiff);
void buttonAll(Button btn, byte id);

#endif

