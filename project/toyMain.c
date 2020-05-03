#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include "led.h"
#include "buzzer.h"
#include "stateMachines.h"

//Variable previously in wdt_interrupt_handler
int blink = 0;

int main(void) {
  configureClocks();
  lcd_init();
  //led_init();
  p2sw_init(15);
  //buzzer_init();
  
  clearScreen(COLOR_YELLOW);
  drawString5x7(31, 50, "PLAYSTATION", COLOR_RED, COLOR_YELLOW);
  //enableWDTInterrupts();
  or_sr(0x8); //GIE (enable interrupts)
  
  for(;;){
    u_int p2val = p2sw_read();
    
    //Detect button presses
    char sw1_state_down = (p2val & 1) ? 0 : 1;
    char sw2_state_down = (p2val & 2) ? 0 : 1;
    char sw2_state_up = (p2val & 2) ? 1 : 0;
    char sw3_state_down = (p2val & 4) ? 0 : 1;
    char sw4_state_down = (p2val & 8) ? 0 : 1;

    if (sw1_state_down) { //Count to three
      state = 1;
      drawString8x12(31, 70, "STATE 1", COLOR_WHITE, COLOR_BLACK);
      drawString8x12(31, 84, "STATE 2", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 96, "STATE 3", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 110, "STATE 4", COLOR_YELLOW, COLOR_YELLOW);
    }
    if (sw2_state_down) { //Jump
      state = 2;
      charging = 1;
      drawString8x12(31, 70, "STATE 1", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 84, "STATE 2", COLOR_WHITE, COLOR_BLACK);
      drawString8x12(31, 96, "STATE 3", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 110, "STATE 4", COLOR_YELLOW, COLOR_YELLOW);
    }
    if (state == 2 && sw2_state_up) {//Special jump release
      charging = 0;
    }
    if (sw3_state_down) { //Play Calimari Inkantation
      state = 3;
      drawString8x12(31, 70, "STATE 1", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 84, "STATE 2", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 96, "STATE 3", COLOR_WHITE, COLOR_BLACK);
      drawString8x12(31, 110, "STATE 4", COLOR_YELLOW, COLOR_YELLOW);
    }
    if (sw4_state_down) {
      state = 4;
      drawString8x12(31, 70, "STATE 1", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 84, "STATE 2", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 96, "STATE 3", COLOR_YELLOW, COLOR_YELLOW);
      drawString8x12(31, 110, "STATE 4", COLOR_WHITE, COLOR_BLACK);
    }

    //Replace wdt_interrupt_handler (theoretically)
    blink++;
    if(blink == 31000){//Still too fast
      state_advance();
      blink = 0;
    }
  }
}
