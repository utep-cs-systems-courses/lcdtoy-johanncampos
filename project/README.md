# Regarding the Assembly

For the translation into assembly, I chose the first method I wrote since it
would likely be the easiest to translate. When committing to GitHub, I only
included the assembly file and its header so that it could be observed and
graded independent of the actual project. This is in part because I believed
that was the new instructions, contrary to the original project instructions,
but also because I ran into a strange error with the assembly.

Specifically, rather than any sort of compilation error, it was the use of the
green_on address. If one was to incorporate the assembly method into the
project, as I had initially done, there would be no errors apart from the
actual behavior of the state machine. Namely, every reference to "&green_on"
is treated by the machine as a reference to "&red_on," thus making it that the
green LED never turns on and the red LED fails to alternate on and off with
each state advance. (To be clear, I do know that the states are advancing
correctly because the C major scale is still coming out of the speaker from 0
to 3; thus, it is simply the LED booleans -- specifically green_on -- that is
the problem.)

As of this submission, I do not know why this is so. Syntactically, the
"count_to_three_assembly.s" file should be an accurate translation of the
original method, switch statement and all. It is simply this oddity regarding
the green_on boolean that is amiss.
Thus, to reiterate, the assembly is complete and syntactically correct (I
presume), but for the sake of demonstration, it is not actually incorporated
into the project.
