#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "led.h"
#include "buzzer.h"
#include "stateMachines.h"

//Variable previously in wdt_interrupt_handler
long blink = 0;

int main(void) {
  configureClocks();
  lcd_init();
  //led_init();
  p2sw_init(15);
  //buzzer_init();
  //enableWDTInterrupts();
  or_sr(0x8); //GIE (enable interrupts)

  clearScreen(COLOR_BLACK);
  drawString11x16(0, 30, "Home Menu", COLOR_WHITE, COLOR_BLACK);
  drawString8x12(0, 50, "Select mode:", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(0, 70, "1. Count to Three", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(0, 80, "2. Jump", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(0, 90, "3. Inkantation", COLOR_WHITE, COLOR_BLACK);
  drawString5x7(0, 100, "4. Pause Menu", COLOR_WHITE, COLOR_BLACK);
  
  for(;;){
    
    u_int p2val = p2sw_read();
    
    //Detect button presses
    char sw1_state_down = (p2val & 1) ? 0 : 1;
    char sw2_state_down = (p2val & 2) ? 0 : 1;
    char sw2_state_up = (p2val & 2) ? 1 : 0;
    char sw3_state_down = (p2val & 4) ? 0 : 1;
    char sw4_state_down = (p2val & 8) ? 0 : 1;

    //Colors for Pause Menu
    u_char fgColor = COLOR_BLUE;
    u_char bgColor = COLOR_RED;

    //Pause Menu Options
    if (state == 4) {
      if (sw1_state_down) {
	state = prevState;
      }
      if (sw2_state_down) {
	state = 0;
      }
      if (sw3_state_down) {
	if (upper) {
	  drawString8x12(31, 70, "CONTINUE", bgColor, fgColor);
	  fillRectangle(31, 84, 36, 15, bgColor);
	  drawString8x12(31, 84, "EXIT", fgColor, bgColor);
	  upper = 0;
	} else {
	  fillRectangle(31, 70, 72, 15, bgColor);
	  drawString8x12(31, 70, "CONTINUE", fgColor, bgColor);
	  drawString8x12(31, 84, "EXIT", bgColor, fgColor);
	  upper = 1;
	}
      }
      if (sw4_state_down) {
	state = prevState;
      }
    }
    //Normal Buttons
    else {
      if (sw1_state_down) { //Count to three
	state = 1;
	screenChanged = 1;
      }
      if (sw2_state_down) { //Jump
	if (state != 2) {
	  screenChanged=1;
	}
	else {
	  charging = 1;
	}
	state = 2;
      }
      if (sw2_state_up) {//Special jump release
	charging = 0;
      }
      if (sw3_state_down) { //Play Calimari Inkantation
	state = 3;
	screenChanged = 1;
      }
      if (sw4_state_down) { //Pause Menu
	prevState = state;
	state = 4;
	drawString8x12(31, 70, "CONTINUE", fgColor, bgColor);
	drawString8x12(31, 84, "EXIT", fgColor, bgColor);
      }
    }

    //Replace wdt_interrupt_handler
    blink++;
    if(blink == 0xffff || (state == 2 && blink ==0x0010)){//Proper tempo
      state_advance();
      blink = 0;
    }
  }
}
