#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "lcdx.h"

char state = 0;
//Release boolean for jump()
char charging = 0;

//Booleans for LCD display
char screenChanged = 0;

void state_advance() {
  switch (state) {
  case 0: //State 0: Home Screen
    /*
    red_on = 1;
    green_on ^= 1;
    green_on ? buzzer_set_period(758) : buzzer_set_period(803);
    led_update();*/
    if (screenChanged) {
      clearScreen(COLOR_BLACK);
      drawString11x16(0, 30, "Home Menu", COLOR_WHITE, COLOR_BLACK);
      drawString8x12(0, 50, "Select mode:", COLOR_WHITE, COLOR_BLACK);
      drawString5x7(0, 70, "1. Count to Three", COLOR_WHITE, COLOR_BLACK);
      drawString5x7(0, 80, "2. Jump", COLOR_WHITE, COLOR_BLACK);
      drawString5x7(0, 90, "3. Inkantation", COLOR_WHITE, COLOR_BLACK);
      drawString5x7(0, 100, "4. Loop de Loop", COLOR_WHITE, COLOR_BLACK);
      screenChanged = 0;
    }
    break;
  case 1: //State 1: Count to Three
    if (screenChanged) {
      clearScreen(COLOR_RED);
      screenChanged = 0;
    }
    count_to_three();
    //led_update();
    break;
  case 2: //State 2: Jump
    //red_on = 0;
    //green_on = 0;
    if (screenChanged) {
      clearScreen(COLOR_PURPLE);
      drawString8x12(0, 20, "SQUID", COLOR_ORANGE, COLOR_PURPLE);
      drawString8x12(5, 35, "JUMP", COLOR_ORANGE, COLOR_PURPLE);
      drawString5x7(0, 50, "Score:", COLOR_ORANGE, COLOR_PURPLE);
      screenChanged = 0;
    }
    jump();
    //led_update();
    break;
  case 3: //State 3: Play Calamari Inkantation
    if (screenChanged) {
      fillRectangle(0, 0, screenWidth/2, screenHeight/2, COLOR_GREEN);
      fillRectangle(screenWidth/2, screenHeight/2, screenWidth, screenHeight, COLOR_PINK);
      screenChanged = 0;
    }
    inkantate();
    //led_update();
    break;
  case 4: //State 4: Loop
    loop();
    //led_update();
    break;
  }
}

void count_to_three() {
  //Counter
  static char num = 0;

  //LCD Display Variables
  static int y = 0;
  static char *number = "0";
  static int fgColor = 0x00;
  static int bgColor = 0x00;
  
  //Counting
  switch (num++) {
  case 0: //Start on 0, play C4
    //red_on = 0; green_on = 0; buzzer_set_period(7644);
    y = 25; number = "0";
    fgColor = COLOR_YELLOW; bgColor = COLOR_PURPLE;
    break;
  case 1: //Go to 1, play E4
    //red_on ^= 1; buzzer_set_period(6067);
    y = 50; number = "1";
    fgColor = COLOR_GREEN; bgColor = COLOR_YELLOW;
    break;
  case 2: //Go to 2, play G4
    //red_on ^= 1; green_on ^= 1; buzzer_set_period(5102);
    y = 75; number = "2";
    fgColor = COLOR_BLUE; bgColor = COLOR_GREEN;
    break;
  case 3: //End on 3, play C5
    //red_on ^= 1; buzzer_set_period(3822);
    y = 100; number = "3";
    fgColor = COLOR_PURPLE; bgColor = COLOR_BLUE;
    break;
  }

  //Draw Current Number
  drawString11x16(100, y, number, fgColor, bgColor);

  //Reset after 3
  if (num == 4) {
    num = 0;
    fillRectangle(100, 25, 120, 120, COLOR_RED);
  }
}

void jump() {//New and improved, simpler to read
  //Game Values
  static int charge_level = 0;
  static char action = 0;
  //Charge Display
  static char flashing = 0;
  static char isRed = 0;
  //Points
  static int i = 0;
  static int score = 0, high_score = 0;
  static char scoreDisplay[3];
  //Squid Measurements
  static int posX = 80, posY = 125, size = 36;

  //Entryway
  if (charging) {
    action = 1;
  }
  //Erase previous squid
  if (action != 0) {
    fillRectangle(posX-size/2, 0, posX+size/2, screenHeight, COLOR_PURPLE);
  }

  //Display Score
  drawString5x7(36, 50, itoa(10*high_score, scoreDisplay, 10), COLOR_WHITE, COLOR_PURPLE);

  //Victory
  if (high_score == 20) {
    drawString8x12(12, 80, "YOU", COLOR_ORANGE, COLOR_PURPLE);
    drawString8x12(10, 95, "WIN!", COLOR_ORANGE, COLOR_PURPLE);
    drawString5x7(0, 110, "Press", COLOR_WHITE, COLOR_PURPLE);
    drawString5x7(0, 120, "Button 2", COLOR_WHITE, COLOR_PURPLE);
    drawString5x7(0, 130, "to play", COLOR_WHITE, COLOR_PURPLE);
    drawString5x7(0, 140, "again.", COLOR_WHITE, COLOR_PURPLE);
    //Hard reset
    charge_level = 0;
    flashing = 0;
    isRed = 0;
    score = 0;
    high_score = 0;
    action = 0;
    state = 0;
  }
  
  //State Machine
  switch (action) {
  case 0://Stationary Squid
    drawTriangle(posX, posY, size, COLOR_WHITE);
    break;
  case 1://Charging Squid
    //Increase charge
    if(charge_level < 20) ++charge_level;
    //Indicate level of charge
    if (charge_level > 15) flashing = 1;
    //Draw smaller squid
    if (flashing && isRed) {
      drawTriangle(posX, posY+charge_level/2, size-charge_level, COLOR_RED);
	isRed = 0;
    } else {
      drawTriangle(posX, posY+charge_level/2, size-charge_level, COLOR_WHITE);
	isRed = 1;
    }
    //Move to jump
    if (!charging) {
      i=charge_level;
      action = 2;
    }
    break;
  case 2://Jumping Squid
    if(i > 0) {
      //Draw higher squid
      drawTriangle(posX, posY-5*(charge_level-i), size-charge_level, COLOR_WHITE);
      --i;
      ++score;
      if (score > high_score){//Max 200 Points
	high_score = score;
      }
    } else { //Move to fall
      score = 0;
      action = 3;
    }
    break;
  case 3://Falling squid
    if (charge_level > 0) {
      //Draw lower squid
      drawTriangle(posX, posY-5*charge_level, size-charge_level, COLOR_WHITE);
      --charge_level;
    } else {//Go to Stationary Squid
      charge_level = 0;
      action = 0;
      flashing = 0;
    }
    break;
  }
}

void inkantate() {
  //Verses
  static int dooo_do[] = {E4, E4, 0, D4};
  static int scoop[] = {E4, D4, B3, D4};
  static int mount[] = {E4, G4, A4, B4};
  static int end1[] = {A4, 0, A4, G4};
  static int end2[] = {E4, 0, E4, G4};
  static int end3[] = {A4, B4, D5, B4};
  static int end4[] = {A4, G4, E4, D4};
  static int end5[] = {E4, 0, 0, 0};

  //Markers
  static char i = 0;
  static char verse = 0;

  //Current note
  int note = 0;

  //Get next note in song
  switch (verse) {
  case 0: case 4: case 7: (i == 0) ? (red_on = 0, green_on = 1) : 0; //Reset for each return
  case 1: case 2: case 5: case 8: case 9:
    note = dooo_do[i];
    if (i == 0) {//Red and green take turns every second note change
      red_on ^= 1;
      green_on ^= 1;
    }
    break;
  case 3: case 10: //Green handles scoop
    red_on = 0;
    green_on ^= 1;
    note = scoop[i];
    break;
  case 6: green_on = 0; case 11:
    red_on ^= 1; //Red handles mount
    note = mount[i];
    break;
  case 12:
    red_on = 1; //Both come in
    green_on = 1;
    note = end1[i];
    break;
  case 13:
    red_on = 0; //Both go out
    green_on = 0;
    note = end2[i];
    break;
  case 14:
    red_on ^= 1; //Red flickers
    note = end3[i];
    break;
  case 15:
    green_on ^= 1; //Green flickers
    note = end4[i];
    break;
  case 16:
    red_on = 1; //Both come in
    green_on = 1;
    note = end5[i];
    break;
  }

  //Play note
  buzzer_set_period(note);
  
  //Go to next note
  ++i;
  if (i == 4) { //Go to next verse
    i = 0;
    (verse == 16) ? (state = 0, verse = 0) : ++verse; //Exit state when song is finished
  }
}

void loop() {
  static char point = 1;
  static char rising = 1;
  
  //Increment/Decrement LEDs and sound
  switch (point) {
  case 1:
    red_on = 0; green_on = 0;
    buzzer_set_period(D4); break;
  case 2:
    red_on = 0; green_on = 1;
    buzzer_set_period(G4); break;
  case 3:
    red_on = 1; green_on = 1;
    buzzer_set_period(B4); break;
  case 4:
    red_on = 1; green_on = 0;
    buzzer_set_period(D5); break;
  }

  //Turn around at peak and valley
  if (rising) {
    if (++point == 4) {
      rising = 0;
    }
  } else {
    if (--point == 1) {
      rising = 1;
    }
  }
}
