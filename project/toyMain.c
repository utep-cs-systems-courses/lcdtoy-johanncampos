#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "led.h"
#include "switches.h"
#include "buzzer.h"

int main(void) {
  configureClocks();
  lcd_init();
  led_init();
  switch_init();
  buzzer_init();
  u_char width = screenWidth, height = screenHeight;
  clearScreen(COLOR_YELLOW);
  drawString5x7(31, 50, "PLAYSTATION", COLOR_WHITE, COLOR_BLACK);
  enableWDTInterrupts();
  or_sr(0x18);
}
