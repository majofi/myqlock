/**
 * Helper.h
 * Allgemeine Hilfsfunktionen
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#include "Helper.h"

// #define DEBUG

/**
 * Reihenfolge der Bits in einem Byte umkehren
 */
byte swapBits(byte x_input)
{
  byte tmp = x_input;
  int s = sizeof(x_input) * 8 - 1;
  
  for (x_input >>= 1; x_input; x_input >>= 1)
  {   
    tmp <<= 1;
    tmp |= x_input & 1;
    s--;
  }
  tmp <<= s;
  
  return tmp;
}