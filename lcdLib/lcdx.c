/*
 * File: lcdx.v
 * Purpose: To test any and all LCD-related methods
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <p2switches.h>
#include "lcdx.h"

int
main()
{
  configureClocks();
  lcd_init();
  p2sw_init(15);
  //enableWDTInterrupts(); //This is the problem
  or_sr(0x8);
  
  clearScreen(COLOR_BLACK);

  drawString11x16(40, 30, "SONY", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(18, 50, "PLAYSTATION", COLOR_WHITE, COLOR_BLACK);

  for(;;){
    u_int p2val = p2sw_read();

    char sw1_state_down = (p2val & 1) ? 0 : 1;
    char sw2_state_down = (p2val & 2) ? 0 : 1;
    char sw2_state_up = (p2val & 2) ? 1 : 0;
    char sw3_state_down = (p2val & 4) ? 0 : 1;
    char sw4_state_down = (p2val & 8) ? 0 : 1;

    if(sw1_state_down) {
      drawX(75, 75, 25, COLOR_BLUE);
    } else{
      drawX(75, 75, 25, COLOR_BLACK);
    }
    if(sw2_state_down) {
      drawDiamond(50, 75, 25, COLOR_RED);
    } else{
      drawDiamond(50, 75, 25, COLOR_BLACK);
    }
    if(sw3_state_down) {
      drawSquare(50, 100, 25, COLOR_PINK);
    } else{
      drawSquare(50, 100, 25, COLOR_BLACK);
    }
    if(sw4_state_down) {
      drawTriangle(75, 100, 25, COLOR_GREEN);
    } else{
      drawTriangle(75, 100, 25, COLOR_BLACK);
    }
  }
}

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
  int i;
  /*
  for (j = 0; j < len; j++) {
    drawPixel(x-j, y-(len-j), COLOR_YELLOW);
    drawPixel(x-j, y+(len-j), COLOR_BLUE);
    drawPixel(x-j+len, y-(len-j)+len, COLOR_RED);
    drawPixel(x-j+len, y+(len-j)-len, COLOR_GREEN);
  }
  */
  for(j=0;j<len+1;j++){
    for(i = 0; i < size; i++){
      if(i>=len-j && i<=len+j){
	drawPixel(x-len+i,y-len+j,color);
      }
    }
  }
  for(j=len+1;j<size;j++){
    for(i = 0; i < size; i++) {
      if(i>=j-len && i<=size-(j-len)){
	drawPixel(x-len+i,y-len+j,color);
      }
    }
  }
}

void drawSquare(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;
  /*
  for (j = 0; j < size; j++) {
    drawPixel(x-len+j, y-len, COLOR_YELLOW);
    drawPixel(x-len, y-len+j, COLOR_BLUE);
    drawPixel(x-len+j, y+len, COLOR_RED);
    drawPixel(x+len, y-len+j, COLOR_GREEN);
  }
  */
  for(j=0;j<size;j++){
    for(int i = 0; i<size; i++) {
      drawPixel(x-len+i, y-len+j, color);
    }
  }
}

void drawTriangle(int x, int y, int size, int color) {
  u_char j;
  u_char len = size/2;
  /*
  for (j = 0; j < size; j++) {
    drawPixel(x-(j/2), y-(len-j), COLOR_YELLOW);
    drawPixel(x, y-len+j, COLOR_BLUE);
    drawPixel(x-len+j, y+len, COLOR_RED);
    drawPixel(x+(j/2), y-(len-j), COLOR_GREEN);
  }
  */
  for(j=0;j<size;j++){
    for(int i = 0; i < size; i++) {
      if(i>=len-(j/2) && i<=len+(j/2)) {
	drawPixel(x-len+i, y-len+j, color);
      }
    }
  }
}
