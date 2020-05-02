#ifndef buzzer_included
#define buzzer_included

//Cycles for notes for Calamari Inkantation (all flat)
#define B3 8581
#define D4 7216
#define E4 6426
#define G4 5405
#define A4 4816
#define B4 4290
#define D5 3608

void buzzer_init();
void buzzer_set_period(short cycles);

#endif
