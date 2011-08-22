/**
 * Blank.cpp
 * Leeren Bildschirm ausgeben
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#include "plugin_Blank.h"
#include "DisplayMatrix.h"

int Blank_matrixX;
int Blank_matrixY;

// Initialisierung
void initBlank(int x, int y)
{
  Blank_matrixX = x;
  Blank_matrixY = y;
  
  return;
}

// Update
void updateBlank(int timeDiff)
{
  return;
}
  
// Anzeigefunktion
void showBlank()
{  
  clearMatrix();
  writeMatrix();
  
  return;
}

void buttonBlank(Button btn, byte id)
{

}
