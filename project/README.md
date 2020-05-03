# UPDATE 5/3/20

By experimenting with a test class within the lcdLib directory, I have
determined that the issue in part revolves around the use of the
**enableWDTInterrupts** command. It seems that for some reason (likely involving the
timer), this command and **lcd_init()** cannot coexist. Similarly, as is
demonstrated by this updated rendition of the toyMain class, **led_init()**
and **buzzer_init()** also clash with the **lcd_init()** method.

As of now, my project can act as either the state machine device from Project
2 or as a screen display for this project, but not both at once. Currently, I
have all the old initialization methods commented out, which prevents any of
their features from being used, instead allowing for the screen to update with
presses to each button.

Additionally, in an effort to make a state machine without a WDT interrupt
handler, I've placed the **state_advance()** call and **blink** incrementing
variable within the for loop used for changing the screen. If one were to
uncomment **led_init()** and **buzzer_init()** and comment out **lcd_init**,
this would serve as an okay replacement for the handler, but this does not
allow for the original timing of project 2 to be maintained.

# CURRENT ISSUES

I believe I have properly integrated the LCD library into this project, but I
cannot currently edit the screen using the **clearScreen()** function.

This rendition of the project is currently identitical to my second
submission, save the following files:

- toyMain.c
- stateMachines.c
- Makefile

In my **Makefile**, I incorporate the LCD library into my executable by adding
`../lib/libLcd.a` into the **toy.elf** command as shown below:

```
toy.elf: ${COMMON_OBJECTS} toyMain.o led.o stateMachines.o switches.o buzzer.o wd_interrupt_handler.o p2_interrupt_handler.o ../lib/libLcd.a ../lib/libTimer.a
```

In the two classes, I have included the relevant header files from the LCD
library as shown below:

```
#include <lcdutils.h>
#include <lcddraw.h>
```

Then, in the **main()** method of **toyMain.c**, I have added the line

```
clearScreen(COLOR_YELLOW)
```

Similarly, in the **count_to_three()** method of **stateMachines.c**, I have added
calls to the **clearScreen()** method in each of the cases.

The issues from here are as follows:

1. Everything compiles perfectly, suggesting that I have incorporated the LCD
library and all of its header files and methods correctly.
2. Despite this, when loading the program into my msp430, the screen is not
initially made yellow. Instead, the screen is filled with a colorful static of pixels.
3. I know that the calls to **clearScreen()** in my cases in the
**count_to_three** method() are being implemented, because the program is
slowing down with how much effort the system needs to spend running that
method; however, the screen is not actually changing colors, instead
maintaining the colorful static from before.

I hope that these issues can be resolved swiftly, and that there is simply one
big issue I am failing to notice. In the meantime, I can continue to program
this project as if the screen were working.

Thank you for your time,
Johann Campos
