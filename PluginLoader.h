/**
 * pluginLoader.h
 * Funktionen für den Aufruf einzelner Plugins
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller
 * @version  1.0
 * @datum    21.3.2011
 */
#ifndef PLUGINLOADER_H
#define PLUGINLOADER_H

#include "WProgram.h"
#include "Button.h"

void registerPlugin(void (*x_initPtr)(int x, int y), void (*x_updatePtr)(int timeDiff), void (*x_showPtr)(), void (*x_buttonPtr)(Button btn, byte id));
void callPluginInit(int x, int y);
void callPluginUpdate(int mode, int timeDiff);
void callPluginShow(int mode);
void callPluginButton(int mode, Button btn, byte id);
int getPluginCount();

#endif

