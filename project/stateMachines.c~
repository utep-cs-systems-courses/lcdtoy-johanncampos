#include <msp430.h>
#include "stateMachines.h"
#include "led.h"
#include "buzzer.h"

char state = 0;
//Release boolean for jump()
char charging = 0;

void state_advance() {
  switch (state) {
  case 0: //State 0: Home Screen
    red_on = 1;
    green_on ^= 1;
    green_on ? buzzer_set_period(758) : buzzer_set_period(803);
    led_update();
    break;
  case 1: //State 1: Count to Three
    count_to_three();
    led_update();
    break;
  case 2: //State 2: Jump
    red_on = 0;
    green_on = 0;
    jump();
    led_update();
    break;
  case 3: //State 3: Play Calamari Inkantation
    inkantate();
    led_update();
    break;
  case 4: //State 4: Loop
    loop();
    led_update();
    break;
  }
}

void count_to_three() {
  //Counter
  static char num = 0;

  //Counting
  switch (num++) {
  case 0: //Start on 0, play C4
    red_on = 0; green_on = 0; buzzer_set_period(7644); break;
  case 1: //Go to 1, play E4
    red_on ^= 1; buzzer_set_period(6067); break;
  case 2: //Go to 2, play G4
    red_on ^= 1; green_on ^= 1; buzzer_set_period(5102); break;
  case 3: //End on 3, play C5
    red_on ^= 1; buzzer_set_period(3822); break;
  }
  
  //Reset after 3
  if (num == 4) {
    num = 0;
  }
}

void jump() {
  //Time Counter
  static char sec_fraction = 0;

  //Charge Counter & Accompanying Sound
  static char charge_level = 0; //Max 12
  static int charge_noise[] = {B3, D4, E4, G4, A4, B4, D5}; //Borrowed notes from song to indicate rising charge level
  static int note_index = 0;
  static int charge_brightness = 32; //LED on every 32 250ths of a second

  //Jump Noise
  static int bounce[] = {D4, G4, 0};
  static int bounce_index = 0;
  
  //Charging Mode
  if (charging) {
    //Reset bounce noise
    bounce_index = 0;
    //Every half second, increase charge
    if (sec_fraction % 125 == 0 && charge_level < 6) {
      ++charge_level; //Increase charge level after half a second
      ++note_index; //Go up a note
      if (charge_brightness != 1) {
	charge_brightness /= 2; //Exponentially increase LED_brightness
      }
      sec_fraction = 0; //Reset time counter
    }
    //Shine green LED fraction of the time
    if (sec_fraction % charge_brightness == 0) {
      green_on = 1;
    } else {
      green_on = 0;
    }
    //Blink red LED when at 2/3 charge level
    if (charge_level > 3) {
      if (sec_fraction < 62) {
	red_on = 1;
      } else {
	red_on = 0;
      }
    }
    //Update buzzer
    buzzer_set_period(charge_noise[note_index]);
  } else { //Reset charge attributes and time counter upon release
    charge_level = 0;
    note_index = 0;
    charge_brightness = 32;
    //Make bounce noise to indicate release
    if (sec_fraction % 62 == 0 && bounce_index < 3) {
      buzzer_set_period(bounce[bounce_index]);
      ++bounce_index;
      sec_fraction = 0;
    }
  }
  
  ++sec_fraction;//Keep track of time in place of wd_interrupt_handler
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
