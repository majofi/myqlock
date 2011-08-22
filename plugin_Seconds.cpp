/**
 * Seconds.cpp
 * Anzeige des Sekundenanteils der Uhrzeit
 *
 * @mc       Arduino/RBBB
 * @autor    Andreas Muttscheller / Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */
#include "plugin_Seconds.h"
#include "ClockHandler.h"
#include "DisplayMatrix.h"

const byte Seconds_ziffern[10][7] = {
  {
    0b00001110,
    0b00010001,
    0b00011001,
    0b00010101,
    0b00010011,
    0b00010001,
    0b00001110
  }
  , {
    0b00000111,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00000110,
    0b00000010
  }
  , {
    0b00011111,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
    0b00010001,
    0b00001110
  }
  , {
    0b00001110,
    0b00010001,
    0b00000001,
    0b00000010,
    0b00000100,
    0b00000010,
    0b00011111
  }
  , {
    0b00000010,
    0b00000010,
    0b00011111,
    0b00010010,
    0b00001010,
    0b00000110,
    0b00000010
  }
  , {
    0b00001110,
    0b00010001,
    0b00000001,
    0b00000001,
    0b00011110,
    0b00010000,
    0b00011111
  }
  , {
    0b00001110,
    0b00010001,
    0b00010001,
    0b00011110,
    0b00010000,
    0b00001000,
    0b00000110
  }
  , {
    0b00001000,
    0b00001000,
    0b00001000,
    0b00000100,
    0b00000010,
    0b00000001,
    0b00011111
  }
  , {
    0b00001110,
    0b00010001,
    0b00010001,
    0b00001110,
    0b00010001,
    0b00010001,
    0b00001110
  }
  , {
    0b00001100,
    0b00000010,
    0b00000001,
    0b00001111,
    0b00010001,
    0b00010001,
    0b00001110
  }
};

int Seconds_matrixX;
int Seconds_matrixY;
byte Seconds_hours;
byte Seconds_min;
byte Seconds_sec;

// Initialisierung der Sekundenuhr
void initSeconds(int x, int y)
{
  Seconds_matrixX = x;
  Seconds_matrixY = y;
  
  return;
}

// Update der Sekundenuhr-Anzeige
void updateSeconds(int timeDiff)
{
  // Uhrzeit von der RTC ermitteln
  Seconds_hours = getHours();
  Seconds_min = getMinutes();
  Seconds_sec = getSeconds();

  return;
}
  
// Anzeigefunktion der Sekundenuhr
void showSeconds()
{  
  clearMatrix();
  for (int i = 0; i < 7; i++) {
    setMatrixOr(1 + i, Seconds_ziffern[Seconds_sec / 10][i] << 11);
    setMatrixOr(1 + i, Seconds_ziffern[Seconds_sec % 10][i] << 5);
  }
  
  // Die Matrix auf die LEDs multiplexen
  writeMatrix();
  
  return;
}

void buttonSeconds(Button btn, byte id)
{

}
