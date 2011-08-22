/**
 * pluginLoader.cpp
 * Funktionen für den Aufruf einzelner Plugins
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller
 * @version  1.0
 * @datum    21.3.2011
 */

// #define DEBUG

#include "PluginLoader.h"

// Functionpointer für die pluginVerwaltung
typedef struct s_pluginPointer
{
  void (*initPtr)(int x, int y);
  void (*updatePtr)(int timeDiff);
  void (*showPtr)();
  void (*buttonPtr)(Button btn, byte id);
};

// Anzahl registriete Plugins
int pluginCount = 0;
s_pluginPointer *pluginPointer;

/***
 * Pluginverwaltung
 * Hier sind die Funktionen, die für das Registrieren und Ausführen der der Plugins notwendig sind
 */
void registerPlugin(void (*x_initPtr)(int x, int y), void (*x_updatePtr)(int timeDiff), void (*x_showPtr)(), void (*x_buttonPtr)(Button btn, byte id))
{
  pluginPointer = (s_pluginPointer *) realloc(pluginPointer, (pluginCount + 1) * sizeof(s_pluginPointer));
  pluginPointer[pluginCount].initPtr   = x_initPtr;
  pluginPointer[pluginCount].updatePtr = x_updatePtr;
  pluginPointer[pluginCount].showPtr   = x_showPtr;
  pluginPointer[pluginCount].buttonPtr = x_buttonPtr;
  
  pluginCount++;
  
  return;
}

void callPluginInit(int x, int y)
{
  int i = 0;
  for (i=0;i<pluginCount;i++)
  {
    pluginPointer[i].initPtr(x, y);
  }
  
  return;
}

void callPluginUpdate(int mode, int timeDiff)
{
  return pluginPointer[mode].updatePtr(timeDiff);
}

void callPluginShow(int mode)
{
  return pluginPointer[mode].showPtr();
}

void callPluginButton(int mode, Button btn, byte id)
{
    return pluginPointer[mode].buttonPtr(btn, id);
}

int getPluginCount()
{
  return pluginCount;
}
