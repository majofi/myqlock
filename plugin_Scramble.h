/**
 * Scramble.cpp
 * LEDs zufällig leuchten lassen
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#ifndef SCRAMBLE_H
#define SCRAMBLE_H

#include "WProgram.h"
#include "Button.h"

void showScramble();
void initScramble(int x, int y);
void updateScramble(int timeDiff);
void buttonScramble(Button btn, byte id);

#endif

