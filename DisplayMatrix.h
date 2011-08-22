/**
 * DisplayMatrix.h
 * Mit dieser Datei werden die Matrixausgaben gesteuert.
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller
 * @version  1.0
 * @datum    20.3.2011
 */
#ifndef DISPLAYMATRIX_H
#define DISPLAYMATRIX_H

#include "WProgram.h"

void initMatrix(int x, int y);
void setMatrix(char y, word data);
void setLED(int x, int y);
void setMatrixOr(char y, word data);
void writeLine(int x0, int y0, int x1, int y1);
word getMatrix(char y);
word* getMatrixPtr();
void writeMatrix();
byte getMatrixSize();
void clearMatrix();

#endif

