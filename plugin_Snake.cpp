/**
 * Snake.cpp
 * Kleiner Versuch für eine Snake Animation.
 *
 * @mc       Arduino/RBBB
 * @autor    Mathias Fiedler
 * @version  1.0
 * @datum    25.05.2011
 */

#include "plugin_Snake.h"
#include "DisplayMatrix.h"

 typedef struct Snake
 {
   word body[10][10];
   int headX;
   int headY;
   int speed;
 };
 
 long c=1;
 int r=0;
 Snake snake;
 
 // Anzeige je Frame
 void showSnake()
 {
  // Die Matrix auf die LEDs multiplexen
  
  //setLED(3,3);
  clearMatrix();
  
   for (int x=0; x<10; x++) {
       for (int y=0; y<10; y++) {
           if(snake.body[x][y] == 1){
             setLED(x,y);
           }
       }
   }
  
  
  //setMatrixOr(r, c);
  writeMatrix();
/*  
  c = c*2;
  
  if(c > 32000){
   c=1;
   r = r+1; 
  }
  
  if(r > 11){
    r=0;
  }
  

  */
//  delay(500);
  return; 
 }
 
 // Statusupdate je Intervall
 void updateSnake(int x_timeDiff)
 {
   
   //grow the snake
   // 0->up, 1->right , 2->down, 3->left
   int direction = random(3);
   
   snake.body[snake.headX][snake.headY] = 1;
   switch(direction)
   {
     case 0:
     snake.headY = snake.headY + 1;
     break;
     case 1:
      snake.headX = snake.headX + 1;
     break;
     case 2:
      snake.headY = snake.headY - 1;
     break;
     case 3:
      snake.headX = snake.headX - 1;
     break;
   }
   
   
   if(snake.headX < 0){
    snake.headX = 0; 
   }
   
   if(snake.headY < 0){
    snake.headY = 0; 
   }
   
   return;
 }
 
 // Initialisieren 
 void initSnake(int x, int y)
 {
   
   
   snake.headX = 5;
   snake.headY = 5;
   
   clearMatrix();
   
   return;
 }
 
 void buttonSnake(Button btn, byte id)
 {
  // return;
 }
