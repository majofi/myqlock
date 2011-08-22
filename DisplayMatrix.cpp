/**
 * DisplayMatrix.h
 * Mit dieser Datei werden die Matrixausgaben gesteuert.
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller
 * @version  1.0
 * @datum    20.3.2011
 */
 
 /*
 * Wer einen LDR (an A3) installiert hat, kann diese Zeile auskommentieren und hat dann eine von
 * der Umgebungshelligkeit abhaengige Helligkeit.
 */
//#define ENABLE_LDR
 
#include "DisplayMatrix.h"
#include "ShiftRegister.h"

// Der serielle Ausgang zu den Multiplexern
#define outputEnablePin   3

// Der Helligkeitssensor
#define brightnessInputPin    A3

// Die Helligkeit (1-9)
int brightness = 9;

ShiftRegister shiftRegister(10, 12, 11);

// Die Matrix, eine Art Bildschirmspeicher
#define MATRIXSIZE 16
word matrix[MATRIXSIZE];

// Abmessungen der Matrix
int display_matrixX;
int display_matrixY;

void initMatrix(int x, int y)
{
  display_matrixX = x;
	display_matrixY = y;
  
  // Setze Output-Enable der Shift-Register auf LOW->enabled
  pinMode(outputEnablePin, OUTPUT);
  analogWrite(outputEnablePin, brightness);
  
  // Helligkeitssensor konfigurieren
  pinMode(brightnessInputPin, INPUT);
  // digitalWrite(brightnessInputPin, HIGH);
  
  clearMatrix();
}

// Die Matrix loeschen
void clearMatrix() {
  for (int i = 0; i < MATRIXSIZE; i++) {
    matrix[i] = 0;
  }
}

void setMatrix(char y, word data)
{
  // Parameter pr�fen
  if (y < 0 || y > display_matrixY)
  {
    return;
  }
  
  matrix[y] = data;
  
  return;
}

void setMatrixOr(char y, word data)
{
  // Parameter pr�fen
  if (y < 0 || y > display_matrixY)
  {
    return;
  }
  
  matrix[y] |= data;
  
  return;
}

void setLED(int x, int y)
{	
  if(x > display_matrixX || y > display_matrixY || x < 0 || y < 0)
  {
    return;
  }
  
	switch (x)
	{
	case 0:  matrix[y] |= 0b1000000000000000;
			 break;
	case 1:	 matrix[y] |= 0b0100000000000000;
			 break;
	case 2:	 matrix[y] |= 0b0010000000000000;
			 break;
	case 3:  matrix[y] |= 0b0001000000000000;
			 break;
	case 4:  matrix[y] |= 0b0000100000000000;
			 break;
	case 5:  matrix[y] |= 0b0000010000000000;
			 break;
	case 6:  matrix[y] |= 0b0000001000000000;
			 break;
	case 7:  matrix[y] |= 0b0000000100000000;
			 break;
	case 8:  matrix[y] |= 0b0000000010000000;
			 break;
	case 9:  matrix[y] |= 0b0000000001000000;
			 break;
	case 10: matrix[y] |= 0b0000000000100000;
			 break;
	}
  
  return;
}

void writeLine(int x0, int y0, int x1, int y1)
{
  int dx =  abs(x1-x0), sx = x0<x1 ? 1 : -1;
  int dy = -abs(y1-y0), sy = y0<y1 ? 1 : -1; 
  int err = dx+dy, e2; /* error value e_xy */
 
  for(;;){  /* loop */
    setLED(x0,y0);
    if (x0==x1 && y0==y1) break;
    e2 = 2*err;
    if (e2 >= dy) { err += dy; x0 += sx; } /* e_xy+e_x > 0 */
    if (e2 <= dx) { err += dx; y0 += sy; } /* e_xy+e_y < 0 */
  }
}

word getMatrix(char y)
{
  if (y < 0 || y > display_matrixY)
  {
    return 0;
  }
  
  return matrix[y];
}

word* getMatrixPtr()
{
	return &matrix[0];
}

byte getMatrixSize()
{
  return MATRIXSIZE;
}

/**
 * Die Matrix ausgeben
 */
void writeMatrix() {
  word row = 1;
  int brightnessFactor = 50;
  
  #ifdef ENABLE_LDR
    brightness = (1024 - analogRead(brightnessInputPin)) / 114 + 1;
  #endif
  
  for (word k = 0; k < 11; k++) { // wir brauchen keine 16... 
    // Gegen das Glimmen alle LEDs einmal aus...
    shiftRegister.prepareShiftregisterWrite();
    shiftRegister.shiftOut(65535);
    shiftRegister.shiftOut(row);
    shiftRegister.finishShiftregisterWrite();    
#ifdef ENABLE_LDR
    // hier kann man versuchen, das Taktverhaeltnis zu aendern (Auszeit)...
    delayMicroseconds((10-brightness)*brightnessFactor);
#endif

    // Jetzt die Daten...
    shiftRegister.prepareShiftregisterWrite();
    shiftRegister.shiftOut(~matrix[k]);
    shiftRegister.shiftOut(row);
    shiftRegister.finishShiftregisterWrite();    
#ifdef ENABLE_LDR
    // hier kann man versuchen, das Taktverhaeltnis zu aendern (Anzeit)...
    delayMicroseconds(brightness*brightnessFactor);
#else
    delayMicroseconds(500);
#endif

    row = row << 1;
  }
}
