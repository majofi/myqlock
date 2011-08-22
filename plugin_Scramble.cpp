/**
 * Scramble.cpp
 * LEDs zufällig leuchten lassen
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#include "plugin_Scramble.h"
#include "DisplayMatrix.h"

int Scramble_matrixX;
int Scramble_matrixY;

int count;

// Initialisierung
void initScramble(int x, int y)
{
  Scramble_matrixX = x;
  Scramble_matrixY = y;
  count = 0;
  
  return;
}

// Update
void updateScramble(int timeDiff)
{
  count += timeDiff;
  return;
}
  
// Anzeigefunktion
void showScramble()
{ 
  // nur alle 1 sek updaten
  if (count < 1000)
  {
    writeMatrix();
    return;
  }
  
  clearMatrix();
  
  count = 0;
  byte matrixSize = getMatrixSize(); 
  for (int i = 0; i < matrixSize; i++) 
  {
    setMatrix(i, random(65536));
  }
  
  // Die Matrix auf die LEDs multiplexen
  writeMatrix();
  
  return;
}

void buttonScramble(Button btn, byte id)
{

}
