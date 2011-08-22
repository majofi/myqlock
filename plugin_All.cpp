/**
 * All.cpp
 * Alle LEDs leuchten lassen
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#include "plugin_All.h"
#include "DisplayMatrix.h"

int All_matrixX;
int All_matrixY;

// Initialisierung
void initAll(int x, int y)
{
  All_matrixX = x;
  All_matrixY = y;
  
  return;
}

// Update
void updateAll(int timeDiff)
{
  return;
}
  
// Anzeigefunktion
void showAll()
{ 
  clearMatrix();
  byte matrixSize = getMatrixSize(); 
  for (int i = 0; i < matrixSize; i++) 
  {
    setMatrix(i, 65535);
  }
  writeMatrix();  
  
  return;
}

void buttonAll(Button btn, byte id)
{

}