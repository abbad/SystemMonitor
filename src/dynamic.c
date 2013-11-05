#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#include "dynamic.h"
#include "shellCalls.h"
#include "utilities.h"

void * getCPULoadAverage(void *output)
{ 
	while(1){
		struct DYNAMICARGS *msg = (struct  DYNAMICARGS *)output;
		runSysCall("cat /proc/loadavg", &msg->cpuLoad);	
		writeData("cpuLoad.log", msg->cpuLoad);
		sleep(*msg->refreshTime);
		
		if (*msg->flag == 1)
			break;
	}
	
	pthread_exit("counter Thread exited normally!"); 
}

void * getFreeRamSize(void *output)
{
	while(1){
		char *temp = malloc(5000);
		struct DYNAMICARGS *msg = (struct  DYNAMICARGS *)output;		
		runSysCall("cat /proc/meminfo | grep  'MemFree:'", &temp);
		writeData("MemFree.log", msg->currentfreeRamSize);
		tokenizeFreeRamInfo(temp, msg->currentfreeRamSize );
		free(temp);
		sleep(*msg->refreshTime);
		
		if (*msg->flag == 1)
			break;
		
	}
	
	pthread_exit("counter Thread exited normally!"); 
}

void * getAvailableDiskSpace(void *output)
{
	while(1){
		struct DYNAMICARGS *msg = (struct  DYNAMICARGS *)output;
		runSysCall("df -h", &msg->diskSpace);
		writeData("diskSpace.log", msg->diskSpace);
		sleep(*msg->refreshTime);
		
		if (*msg->flag == 1)
			break;
	}
	
	pthread_exit("counter Thread exited normally!"); 
}

void * getRunningProcessesInfo(void *output)
{	
	while(1){
		struct DYNAMICARGS *msg = (struct  DYNAMICARGS *)output;	
		msg->runningProcessesInfo = NULL;
		runSysCall("ps -a", &msg->runningProcessesInfo);
		writeData("runningProcessesInfo.txt", msg->runningProcessesInfo);
		sleep(*msg->refreshTime);
		
		if (*msg->flag == 1)
			break;
	}
	
	pthread_exit("counter Thread exited normally!"); 
}
