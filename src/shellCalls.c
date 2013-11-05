#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "shellCalls.h"

/*
	This function will call the shell with the command which is send as a parameter.
*/
void runSysCall(char *command, char **output)
{
	FILE *cmdline = popen(command, "r");
	size_t size = 0;
	while(getdelim(output, &size, 0, cmdline) != -1);
	pclose(cmdline);
}

