/*
 * File: lcdx.v
 * Purpose: To test any and all LCD-related methods
 */

#include <libTimer.h>
#include "lcdutils.h"
#include "lcddraw.h"
#include <p2switches.h>
#include "shapes.h"

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
