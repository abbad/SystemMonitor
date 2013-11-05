System Monitor version 0.9
----------------------------------------------------------------------------------------
- To compile code on cygwin type gcc src/*.c -I include/ main.c -lncurses

- To compile on Ubuntu type gcc src/*.c -I include/ main.c -lncurses -pthread -D_REENTRANT

- ./a.exe to start the program.

- Always run in full screen. 




note: on linux remove

#include "ncurses/ncurses.h" from main.c 

and replace with 

#include "ncurses.h"


