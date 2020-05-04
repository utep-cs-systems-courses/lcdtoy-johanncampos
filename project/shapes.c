#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <p2switches.h>

void drawX(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;

  for (j = 0; j < len; j++) {
    drawPixel(x-j, y-j, color);
    drawPixel(x-j, y+j, color);
    drawPixel(x-j+len, y-j+len, color);
    drawPixel(x-j+len, y+j-len, color);
  }
}

void drawDiamond(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;
  u_char i;

  for(j = 0; j < len+1; j++){
    for(i = 0; i < size; i++){
      if(i >= len-j && i <= len+j){
	drawPixel(x-len+i,y-len+j,color);
      }
    }
  }
  for(j =len+1; j<size; j++){
    for(i = 0; i < size; i++) {
      if(i >= j-len && i <= size-(j-len)){
	drawPixel(x-len+i,y-len+j,color);
      }
    }
  }
}

void drawDiamondOutline(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;
  
  for (j = 0; j < len; j++) {
    drawPixel(x-j, y-(len-j), COLOR_YELLOW);
    drawPixel(x-j, y+(len-j), COLOR_BLUE);
    drawPixel(x-j+len, y-(len-j)+len, COLOR_RED);
    drawPixel(x-j+len, y+(len-j)-len, COLOR_GREEN);
  }
}

void drawSquare(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;

  for(j = 0; j < size; j++){
    for(int i = 0; i<size; i++) {
      drawPixel(x-len+i, y-len+j, color);
    }
  }
}

void drawSquareOutline(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;

  for (j = 0; j < size; j++) {
    drawPixel(x-len+j, y-len, COLOR_YELLOW);
    drawPixel(x-len, y-len+j, COLOR_BLUE);
    drawPixel(x-len+j, y+len, COLOR_RED);
    drawPixel(x+len, y-len+j, COLOR_GREEN);
  }
}

void drawTriangle(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;

  for(j = 0; j < size; j++){
    for(int i = 0; i < size; i++) {
      if(i>=len-(j/2) && i<=len+(j/2)) {
	drawPixel(x-len+i, y-len+j, color);
      }
    }
  }
}

void drawTriangleOutline(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;
  
  for (j = 0; j < size; j++) {
    drawPixel(x-(j/2), y-(len-j), COLOR_YELLOW);
    drawPixel(x, y-len+j, COLOR_BLUE);
    drawPixel(x-len+j, y+len, COLOR_RED);
    drawPixel(x+(j/2), y-(len-j), COLOR_GREEN);
  }
}

void drawSquid(int x, int y, int size, int color) {
  u_char i, j;
  u_char len = size/2;
  
  //Head (/\)
  for (j = 3; j < size/3; j++){
    for (i = 0; i < size; i++){
      if (i >= len-j && i <= len+j){
	drawPixel(x-len+i,y-len+j,color);
      }
    }
  }
  //Body (||)
  for (j = size/3; j < 2*size/3; j++){
    for (i = 3*len/5; i <= 7*len/5; i++) {
      drawPixel(x-len+i, y-len+j, color);
    }
  }
  //Tentacles (,,)
  for (j = 2*size/3; j < size-3; j++) {
    for (i = len/2; i < 3*len/2; i++) {
      if (i % 3 == 0) {
	drawPixel(x+i-len+1, y-len+j-1, color);
      }
    }
  }
}
