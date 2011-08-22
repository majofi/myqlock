/**
 * Pong.cpp
 * Kleine Klasse zum Anzeigen von Pong.
 *
 * @mc       Arduino/RBBB
 * @autor    Benedikt Gerlich
 * @version  1.0
 * @datum    18.3.2011
 */

#include "plugin_Pong.h"
#include "DisplayMatrix.h"

#define PADDLEWIDTH 4

typedef struct Pong_Paddle
{
  int posX;
  int posY;
};

typedef struct Pong_Ball
{
  int posX;
  int posY;
  int angle;
  int speed;
};

typedef struct Pong_Game
{
  Pong_Paddle paddleUp;
  Pong_Paddle paddleDown;
  Pong_Ball ball;
  int xScreenMax;
  int yScreenMax;
};

Pong_Game pong;

enum Pong_Hit
{
  NO_HIT,
  HIT_UPPER_PADDLE,
  HIT_LOWER_PADDLE,
  HIT_RIGHT,
  HIT_LEFT,
  HIT_UP,
  HIT_DOWN
};

// Prüfen was an der Übergebenen Stelle getroffen worden wäre
Pong_Hit PongCheckHit(int x, int y)
{
  // Bildschirmränder prüfen
  if(x < 0)
  {
    return HIT_LEFT;
  }
  if(y < 0)
  {
    return HIT_UP;
  }
  if(x > pong.xScreenMax - 1)
  {
    return HIT_RIGHT;
  }
  if(y > pong.yScreenMax - 1)
  {
    return HIT_DOWN;
  }
  
  // Eines der Paddles?
  // Oberes Paddle
  if(y == pong.paddleUp.posY)
  {
    // Y-Koordinate stimmt, nun prüfen ob der Ball im Bereich des oberen Paddles liegt
    if(x <= pong.paddleUp.posX + (PADDLEWIDTH - 1) && x >= pong.paddleUp.posX)
    {
      return HIT_UPPER_PADDLE;
    }
  }

  // Unteres Paddle
  if(y == pong.paddleDown.posY)
  {
    // Y-Koordinate stimmt, nun prüfen ob der Ball im Bereich des oberen Paddles liegt
    if(x <= pong.paddleDown.posX + (PADDLEWIDTH - 1) && x >= pong.paddleDown.posX)
    {
      return HIT_LOWER_PADDLE;
    }
  }

  return NO_HIT;  
}

void PongMovePaddles(int ballX, int ballY)
{
  // Update Paddle Pos
  pong.paddleUp.posX = ballX - (PADDLEWIDTH / 2);
  if(ballX <= pong.paddleDown.posX + (PADDLEWIDTH - 1) && ballX >= pong.paddleDown.posX)
  {
    // Handling für verlieren hier
  }else
  {
    if(ballX < pong.paddleDown.posX)
    {
      pong.paddleDown.posX--;
    }else
    {
      pong.paddleDown.posX++;
    }
  }

  // Die Paddles dürfen natürlich nicht über den Bildschirmrand reichen
  // Prüfung rechte Seite
  if(pong.paddleUp.posX + PADDLEWIDTH > pong.xScreenMax - 1)
  {
    pong.paddleUp.posX = pong.xScreenMax - PADDLEWIDTH;
  }
  if(pong.paddleDown.posX + PADDLEWIDTH > pong.xScreenMax - 1)
  {
    pong.paddleDown.posX = pong.xScreenMax - PADDLEWIDTH;
  }

  // Prüfung linke Seite
  if(pong.paddleUp.posX <= 0)
  {
    pong.paddleUp.posX = 0;
  }
  if(pong.paddleDown.posX <= 0)
  {
    pong.paddleDown.posX = 0;
  }
  
  return;
}

// Anzeige je Frame
void showPong()
{
  int i;
  
  clearMatrix();
  
  // Linie für das obere Paddle anzeigen
  writeLine(pong.paddleUp.posX, pong.paddleUp.posY, pong.paddleUp.posX + PADDLEWIDTH - 1, pong.paddleUp.posY);

  // Linie für das untere Paddle anzeigen
  writeLine(pong.paddleDown.posX, pong.paddleDown.posY, pong.paddleDown.posX + PADDLEWIDTH - 1, pong.paddleDown.posY);  
  
  // Ball anzeigen  
  setLED(pong.ball.posX, pong.ball.posY);
  
  // Die Matrix auf die LEDs multiplexen
  writeMatrix();
  
  return;
}

// Statusupdate je Intervall
void updatePong(int x_timeDiff)
{
  boolean hitLower=false;
  boolean hitUpper=false;
  boolean hitLeft=false;
  boolean hitRight=false;
  boolean done=false;
  
  int newX;
  int newY;

  while(!done)
  {
    // Ballposition aktualisieren
    switch(pong.ball.angle)
    {
      // Oben
      case 0: newY = pong.ball.posY - pong.ball.speed;
              newX = pong.ball.posX;
              break;
      // Oben rechts
      case 1: newY = pong.ball.posY - pong.ball.speed;
              newX = pong.ball.posX + pong.ball.speed;
              break;
      // Unten rechts
      case 2: newY = pong.ball.posY + pong.ball.speed;
              newX = pong.ball.posX + pong.ball.speed;
              break;      
      // Unten
      case 3: newY = pong.ball.posY + pong.ball.speed;
              newX = pong.ball.posX;
              break;
      // Unten Links
      case 4: newY = pong.ball.posY + pong.ball.speed;
              newX = pong.ball.posX - pong.ball.speed;
              break;
      // Oben Links
      case 5: newY = pong.ball.posY - pong.ball.speed;
              newX = pong.ball.posX - pong.ball.speed;            
              break;
    }

    PongMovePaddles(newX, newY);
    
    // Hat der Ball etwas getroffen?
    switch(PongCheckHit(newX, newY))
    {
      case NO_HIT:
           pong.ball.posX = newX;
           pong.ball.posY = newY;    
           done=true;
           break;
      case HIT_UP:
      case HIT_UPPER_PADDLE:        
           pong.ball.angle = random(2, 5);
           break;
      case HIT_DOWN:     
      case HIT_LOWER_PADDLE:        
           // Hack
           pong.ball.angle = random(0, 3);
           if(pong.ball.angle == 2) pong.ball.angle = 5;  
           break;
      case HIT_RIGHT:        
           if(pong.ball.angle == 2)
           {
             pong.ball.angle = 4;
           }else if(pong.ball.angle == 1)
           {
             pong.ball.angle = 5;
           }    
           break;
      case HIT_LEFT:       
           if(pong.ball.angle == 4)
           {
             pong.ball.angle = 2;
           }else if(pong.ball.angle == 5)
           {
             pong.ball.angle = 1;
           }
           break;
    }

    PongMovePaddles(newX, newY);
  }
  
  return;
}

// Initialisieren des Pong Spiels
void initPong(int x, int y)
{
  memset(&pong, 0, sizeof(pong));
  
  // Abmessungen des Bildschirms übernehmen
  pong.xScreenMax = x;
  pong.yScreenMax = y;
  
  // Oberes Paddle startet in der Mitte
  pong.paddleUp.posX = pong.xScreenMax / 2  - (PADDLEWIDTH / 2);
  pong.paddleUp.posY = 0;

  // Unteres Paddle startet ebenfalls in der Mitte
  pong.paddleDown.posX = pong.xScreenMax / 2 - (PADDLEWIDTH / 2);
  pong.paddleDown.posY = pong.yScreenMax - 1;
  
  // Der Ball startet in der Mitte, Richtung zufällig
  pong.ball.posX = (pong.xScreenMax / 2);
  pong.ball.posY = (pong.yScreenMax / 2) - 1;
  pong.ball.speed = 1;
  pong.ball.angle = random(0, 6);
  
  return;
}

void buttonPong(Button btn, byte id)
{

}
