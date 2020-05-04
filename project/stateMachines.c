#include <msp430.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <stdio.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"
#include "shapes.h"

char state = 0;
//Release boolean for jump()
char charging = 0;
//Holder for pause menu
char prevState = 0;
char upper = 0;

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
      drawString5x7(0, 100, "4. Pause Menu", COLOR_WHITE, COLOR_BLACK);
      screenChanged = 0;
    }
    break;
  case 1: //State 1: Count to Three
    if (screenChanged) {
      clearScreen(COLOR_YELLOW);
      drawString5x7(0, 25, "A none, and", COLOR_BLACK, COLOR_YELLOW);
      drawString5x7(0, 50, "A one, and", COLOR_BLACK, COLOR_YELLOW);
      drawString5x7(0, 75, "A two, and", COLOR_BLACK, COLOR_YELLOW);
      drawString5x7(0, 100, "A three, and", COLOR_BLACK, COLOR_YELLOW);
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
      fillRectangle(60, 140, 40, 10, COLOR_BROWN);
      screenChanged = 0;
    }
    jump();
    //led_update();
    break;
  case 3: //State 3: Play Calamari Inkantation
    if (screenChanged) {
      /*fillRectangle(0, 0, screenWidth/2, screenHeight, COLOR_PINK);
	fillRectangle(screenWidth/2, 0, screenWidth/2, screenHeight, COLOR_GREEN);*/
      clearScreen(COLOR_BLACK);
      drawString11x16(20, 20, "CALIMARI", COLOR_PINK, COLOR_BLACK);
      drawString8x12(15, 40, "INKANTATION", COLOR_GREEN, COLOR_BLACK);
      drawString5x7(0, 125, "Pretend you can", 0x1001, COLOR_BLACK);
      drawString5x7(0, 135, "hear music.", 0x1001, COLOR_BLACK);
      drawString5x7(0, 150, "Please.", 0x1001, COLOR_BLACK);
      
      screenChanged = 0;
    }
    inkantate();
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
    fgColor = COLOR_RED; bgColor = COLOR_PURPLE;
    break;
  case 1: //Go to 1, play E4
    //red_on ^= 1; buzzer_set_period(6067);
    y = 50; number = "1";
    fgColor = COLOR_GREEN; bgColor = COLOR_RED;
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
    fillRectangle(100, 25, 120, 120, COLOR_YELLOW);
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
    fillRectangle(posX-size/2, 0, size, 140, COLOR_PURPLE);
  }

  //Display Score
  itoa(10*high_score, scoreDisplay, 10);
  drawString5x7(36, 50, scoreDisplay, COLOR_WHITE, COLOR_PURPLE);

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
    drawSquid(posX, posY, size, COLOR_WHITE);
    break;
  case 1://Charging Squid
    //Increase charge
    if(charge_level < 20) ++charge_level;
    //Indicate level of charge
    if (charge_level > 15) flashing = 1;
    //Draw smaller squid
    if (flashing && isRed) {
      drawSquid(posX, posY+charge_level/6, size-charge_level/3, COLOR_RED);
	isRed = 0;
    } else {
      drawSquid(posX, posY+charge_level/6, size-charge_level/3, COLOR_WHITE);
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
      drawSquid(posX, posY-5*(charge_level-i), size-charge_level/6, COLOR_WHITE);
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
      drawSquid(posX, posY-5*charge_level, size-charge_level/6, COLOR_WHITE);
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
  /*
  //Verses
  static int dooo_do[] = {E4, E4, 0, D4};
  static int scoop[] = {E4, D4, B3, D4};
  static int mount[] = {E4, G4, A4, B4};
  static int end1[] = {A4, 0, A4, G4};
  static int end2[] = {E4, 0, E4, G4};
  static int end3[] = {A4, B4, D5, B4};
  static int end4[] = {A4, G4, E4, D4};
  static int end5[] = {E4, 0, 0, 0};
  */
  //Markers
  static char i = 0;
  static char verse = 0;

  //Current note
  //int note = 0;

  //Sister Markers
  static char red, green;
  //Background
  static int bgColor = COLOR_BLACK;

  //Get next note in song
  switch (verse) {
  case 0: case 4: case 7: //(i == 0) ? (red_on = 0, green_on = 1) : 0;
    (i == 0) ? (red = 0, green = 1) : 0;//Reset for each return
  case 1: case 2: case 5: case 8: case 9:
    //note = dooo_do[i];
    if (i == 0) {//Red and green take turns every second note change
      //red_on ^= 1;
      //green_on ^= 1;
      red^=1; green^=1;
    }
    break;
  case 3: case 10: //Green handles scoop
    //red_on = 0;
    //green_on ^= 1;
    red=0;green^=1;
    //note = scoop[i];
    break;
  case 6: green_on = 0; case 11:
    //red_on ^= 1; //Red handles mount
    red^=1;
    //note = mount[i];
    break;
  case 12:
    //red_on = 1; //Both come in
    //green_on = 1;
    //note = end1[i];
    red=1;green=1;
    break;
  case 13:
    //red_on = 0; //Both go out
    //green_on = 0;
    //note = end2[i];
    red=0;green=0;
    break;
  case 14:
    //red_on ^= 1; //Red flickers
    //note = end3[i];
    red^=1;
    break;
  case 15:
    //green_on ^= 1; //Green flickers
    //note = end4[i];
    green^=1;
    break;
  case 16:
    //red_on = 1; //Both come in
    //green_on = 1;
    //note = end5[i];
    red=1;green=1;
    break;
  }

  //Squid Display
  if (red && !green) {//Only Callie
    drawSquid(96, 80, 40, COLOR_PINK);
    fillRectangle(12, 60, 40, 40, bgColor);
  } else if (!red && green) {//Only Marie
    drawSquid(32, 80, 40, COLOR_GREEN);
    fillRectangle(76, 60, 40, 40, bgColor);
  } else if (red && green) {//Both
    drawSquid(96, 80, 40, COLOR_PINK);
    drawSquid(32, 80, 40, COLOR_GREEN);
  } else {//Neither
    fillRectangle(12, 60, 40, 40, bgColor);
    fillRectangle(76, 60, 40, 40, bgColor);
  }
  //Play note
  //buzzer_set_period(note);
  
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
