#include <msp430.h>
#include "stateMachines.h"

void __interrupt_vec(WDT_VECTOR) WDT() {
  static char blink_count = 0;
  if (++blink_count == 62) {
    state_advance();
    blink_count = 0;
  }
  if (state == 2) { //Special timer for jump (dim)
    state_advance();
    blink_count = 0;
  }
}
