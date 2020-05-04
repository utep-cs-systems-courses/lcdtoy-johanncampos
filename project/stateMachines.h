#ifndef stateMachine_included
#define stateMachine_included

extern char state;
extern char charging;
extern char screenChanged;

void state_advance();

void count_to_three();

void jump();

void inkantate();

void loop();

#endif
