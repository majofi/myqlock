/**
 * Blank.cpp
 * Leeren Bildschirm ausgeben
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#ifndef BLANK_H
#define BLANK_H

#include "WProgram.h"
#include "Button.h"

void showBlank();
void initBlank(int x, int y);
void updateBlank(int timeDiff);
void buttonBlank(Button btn, byte id);

#endif

