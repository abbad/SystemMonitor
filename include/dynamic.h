#ifndef DYNAMIC_H_INCLUDED
#define DYNAMIC_H_INCLUDED

struct DYNAMICARGS{
	char *cpuLoad; 
	char *currentfreeRamSize;
	char *minFreeRamSize;
	char *maxFreeRamSize;
	char *avgFreeRamSize;
	char *diskSpace;
	char *runningProcessesInfo;
	int *refreshTime;
	int *line;
	int *flag;
};

void * getCPULoadAverage(void *output);
void * getFreeRamSize(void *output);
void * getAvailableDiskSpace(void *output);
void * getRunningProcessesInfo(void *output);

#endif 