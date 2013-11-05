#include <stdio.h>
#include <sys/unistd.h>
#include <string.h>
#include <sys/utsname.h>
#include <stdlib.h>
#include <sys/resource.h> 

#include "utilities.h"
#include "static.h"
#include "shellCalls.h"


void getHostName(char *hostname)
{
	int res = gethostname(hostname, 50);
	
	if(res == -1)
		perror("HostName error");
}

char * getKernelNameAndRelease()
{
	struct utsname buff;
	int res = uname(&buff);
	
	if(res == -1)
		perror("kernelName:");
	
	return concat(buff.sysname, buff.release);
}

char * getVendorOfTheProcesses()
{	
	char * input = malloc(100);
	runSysCall("cat /proc/cpuinfo | grep -i 'vendor'", &input);
	
	// do some parsing
	char delims[] = ":";
	char * result = malloc(500);
	result = strtok(input, delims );
	
	result = strtok( NULL, delims );
	result = strtok( NULL, delims );
	
	return 	result;
}

void getNumberOfprocessors(char *output)
{
	
	long nprocs = -1;
	long nprocs_max = -1;
	#ifdef _WIN32
	#ifndef _SC_NPROCESSORS_ONLN
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	#define sysconf(a) info.dwNumberOfProcessors
	#define _SC_NPROCESSORS_ONLN
	#endif
	#endif
	#ifdef _SC_NPROCESSORS_ONLN
	nprocs = sysconf(_SC_NPROCESSORS_ONLN);
	if (nprocs < 1)
	{ 
		perror("CPUNumber:");
		
	}
	nprocs_max = sysconf(_SC_NPROCESSORS_CONF);
	if (nprocs_max < 1)
	{
		perror("CPUNumber:");
		
	}
	
	sprintf(output, "%ld", nprocs_max);
	
	#else
	fprintf(stderr, "Could not determine number of CPUs");
		
	#endif
}

char * getCPUSpeed()
{
	char * input = malloc(100);
	runSysCall("cat /proc/cpuinfo | grep -i 'cpu MHz'", &input);
	
	// do some parsing
	char delims[] = ":";
	char * result;
	result = strtok(input, delims );
	
	result = strtok( NULL, delims );
	result = strtok( NULL, delims );
	
	return result;
}

char * getTotalRamSize()
{
	char * result = malloc(5000);
	char *temp = malloc(5000);
	runSysCall("cat /proc/meminfo | grep -i 'memtotal'", &temp);
	tokenizeFreeRamInfo(temp, result);
	free(temp);
	return result;
}

char * getTotalHardDiskSize(char *command)
{
	char * result = 0;
	runSysCall(command, &result);
	
	return result;
}

char * getModelNameOfEachProcessor()
{
	char * input = malloc(5000);
	char * result =  malloc(5000);
	runSysCall("cat /proc/cpuinfo | grep -i 'name'", &input);
	
	char delims[] = ":";

	result = strtok(input, delims );	
	result = strtok( NULL, delims );
	
	return result;
}
