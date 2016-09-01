System Monitor version 0.9
----------------------------------------------------------------------------------------

A program that gives you general information about your computer. 
It was written in c. 
Written by Ahmad Abbad

- To compile code on cygwin type gcc src/*.c -I include/ main.c -lncurses

- To compile on Ubuntu type gcc src/*.c -I include/ main.c -lncurses -pthread -D_REENTRANT

- ./a.exe to start the program.




note: on linux remove

'#'include "ncurses/ncurses.h" from main.c 

and replace with 

'#'include "ncurses.h"




