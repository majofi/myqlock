/**
 * BinaryClock.h
 * Anzeige der Uhrzeit in Binär
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#ifndef BINARYCLOCK_H
#define BINARYCLOCK_H

#include "WProgram.h"
#include "Button.h"

void showBinary();
void showBinary2();
void initBinary(int x, int y);
void updateBinary(int timeDiff);
void buttonBinary(Button btn, byte id);

#endif

