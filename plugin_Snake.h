/**
 * Snake.h
 * Kleiner Versuch für eine Snake Animation.
 *
 * @mc       Arduino/RBBB
 * @autor    Mathias Fiedler
 * @version  1.0
 * @datum    25.05.2011
 */
#ifndef SNAKE_H
#define SNAKE_H

#include "WProgram.h"
#include "Button.h"

// Anzeige jeden Frame
void showSnake();

// Aktualisierung je nach Intervall
void updateSnake(int timeDiff);

// Initialisieren von Snake
void initSnake(int x, int y);

// Button abfragen
void buttonSnake(Button btn, byte id);

#endif

