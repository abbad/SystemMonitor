#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <time.h>      
#include <signal.h> 

#include "ncurses/ncurses.h"
#include "static.h"
#include "dynamic.h"

void * showDynamicInformation(void *args);
void launchRefreshThread();
void drawMenu(int line);
void executeOption(int line);
void showMainMenu();
void cleanWindow();
void escapePressed();
void getStaticInformation();
void getDynamicInformation();
void fillColors();
void monitorDynamicInformation();
void allocateMemory();
void writeStaticInformationToLog();
void writeAllToLog(int signo);
void removeTempFiles();

char hostName[50], *kernelName, numberOfProcesses[5], *vendor, *CpuSpeed, *ramSize, *modelName, *hdSize; // static

struct DYNAMICARGS args; 

void removeTempFile()
{

	remove("time.txt");
	remove("static.txt");
	remove("runningProcessesInfo.txt");
	remove("diskSpace.txt");
	remove("MemFree.txt");
	remove("cpuLoad.txt");
}

void writeAllToLog(int signo)
{
	remove("log.txt");
	*args.flag = 1;
	clear();
	writeToLog("time.txt");
	writeToLog("static.txt");
	writeToLog("runningProcessesInfo.txt");
	writeToLog("diskSpace.txt");
	writeToLog("MemFree.txt");
	writeToLog("cpuLoad.txt");
	removeTempFile();
	
	endwin();
	exit(1);
} 
 
/* based on line information will be showen */
void * showDynamicInformation(void *args)
{
	int min = atoi(ramSize);
	int curr, max = 0, count = 1, total = 0;
	while(1){
		struct DYNAMICARGS *msg = (struct  DYNAMICARGS *)args;
		
		curr = atoi(msg->currentfreeRamSize);
		
		if (*msg->line == 4){
			// Available disk space
			attron(COLOR_PAIR(4));
			mvprintw(11, 0, "%s", msg->diskSpace);
			attroff(COLOR_PAIR(4));
			
			// RAM Free Size
			attron(COLOR_PAIR(3));
			mvprintw(11, 53, "Current Free Ram: %s kb", msg->currentfreeRamSize);
			if (min > curr){
				memcpy(msg->minFreeRamSize, msg->currentfreeRamSize, strlen(msg->currentfreeRamSize) + 1);
				min = atoi(msg->currentfreeRamSize);
			}
			
			if( max < curr)
			{
				memcpy(msg->maxFreeRamSize, msg->currentfreeRamSize, strlen(msg->currentfreeRamSize) + 1);
				max = atoi(msg->currentfreeRamSize);
			}
			mvprintw(12, 53, "Minimum Free Ram: %s kb", msg->minFreeRamSize);
			mvprintw(13, 53, "Maximum Free Ram: %s kb", msg->maxFreeRamSize);
			mvprintw(14, 53, "Avg Free Ram: %d kb", total / count);
			
			total += curr;
			count++;
			
			attroff(COLOR_PAIR(3));
			
			// CPU load AVG
			attron(COLOR_PAIR(2));
			mvprintw(15, 106, "Cpu Load Avg: %s", msg->cpuLoad);
			attroff(COLOR_PAIR(2));
			
			attron(COLOR_PAIR(5));
			mvprintw(20, 0, "%s", msg->runningProcessesInfo);
			attroff(COLOR_PAIR(5));
		}
		if (*msg->line == 6){
			mvprintw(0,0, "%s", msg->cpuLoad);
		}if (*msg->line == 7){
			mvprintw(1, 0,"Current Free Ram: %s kb", msg->currentfreeRamSize);
		}if (*msg->line  == 8){
				mvprintw(0, 0, msg->diskSpace );
		}if (*msg->line == 9){
				mvprintw(0,0, "%s", msg->runningProcessesInfo);
		}
		
		refresh();
		sleep(*msg->refreshTime);
	}
	
}

void escapePressed(){
	clear();
	*args.line = 0;
	showMainMenu();
}

void fillColors(){
	int x, y;
	
    init_pair(1, COLOR_BLACK, COLOR_YELLOW);
	init_pair(2, COLOR_BLACK, COLOR_GREEN);
	init_pair(3, COLOR_BLACK, COLOR_RED);
	init_pair(4, COLOR_BLACK, COLOR_WHITE);
	init_pair(5, COLOR_BLACK, COLOR_BLUE);
    
	attron(COLOR_PAIR(1));
	for (x = 0; x < 10; x++)
	{
		for (y = 0; y < 160; y++)
		{
			mvprintw(x, y, " ");
		}
		
	}
	attroff(COLOR_PAIR(1));
    
	attron(COLOR_PAIR(4));
	for (x = 10; x < 20; x++)
	{
		for (y = 0; y < 53; y++)
		{
			mvprintw(x, y, " ");
		}
		
	}
	attroff(COLOR_PAIR(4));
	
	attron(COLOR_PAIR(3));
	for (x = 10; x < 20; x++)
	{
		for (y = 53; y < 106; y++)
		{
			mvprintw(x, y, " ");
		}
		
	}
	
	attroff(COLOR_PAIR(3));

	attron(COLOR_PAIR(2));
	
	for (x = 10; x < 20; x++)
	{
		for (y = 106; y < 160; y++)
		{
			mvprintw(x, y, " ");
		}
	}
	
	attroff(COLOR_PAIR(2));
    
	attron(COLOR_PAIR(5));
	for (x = 20; x < 40; x++)
	{
		for (y = 0; y < 160; y++)
		{
			mvprintw(x, y, " ");
		}
		
	}
	attroff(COLOR_PAIR(5));

}

/* this function will gather all static information and assign them to global variables*/
void getStaticInformation()
{
	getHostName(hostName);
	kernelName = getKernelNameAndRelease();
	getNumberOfprocessors(numberOfProcesses);
	vendor = getVendorOfTheProcesses();
	CpuSpeed = getCPUSpeed();
	modelName = getModelNameOfEachProcessor();
	ramSize = getTotalRamSize();
	tokenizeHDInfo(getTotalHardDiskSize("df -h"), hdSize);
}

void getDynamicInformation()
{	
	runSysCall("cat /proc/loadavg", &args.cpuLoad);	
	runSysCall("cat /proc/meminfo | grep  'MemFree:'", &args.currentfreeRamSize);
	runSysCall("df -h", &args.diskSpace);
	runSysCall("ps -a", &args.runningProcessesInfo);
}

void launchRefreshThread()
{
	void *threadResult;
	pthread_t pthreadIds[1];
	int res;

	res = pthread_create(&pthreadIds[0], NULL, showDynamicInformation, (void *)(&args)); 
	
	if(res != 0){ 
		  perror("pthread_create"); 
		  exit(1); 
	} 
}

void monitorDynamicInformation()
{
	void *threadResult;
	pthread_t pthreadIds[4];
	int res;
	
	res = pthread_create(&pthreadIds[0], NULL, getCPULoadAverage, (void *)(&args)); 
	
	if(res != 0){ 
		  perror("pthread_create"); 
		  exit(1); 
	} 
	
	res = pthread_create(&pthreadIds[1], NULL, getFreeRamSize, (void *)(&args)); 

	if(res != 0){ 
		  perror("pthread_create"); 
		  exit(1); 
	} 

	res = pthread_create(&pthreadIds[2], NULL, getAvailableDiskSpace, (void *)(&args)); 

	if(res != 0){ 
		  perror("pthread_create"); 
		  exit(1); 
	} 

	res = pthread_create(&pthreadIds[3], NULL, getRunningProcessesInfo, (void *)(&args)); 

	if(res != 0){ 
		  perror("pthread_create"); 
		  exit(1); 
	} 	
}

void showStaticInformation(int line)
{
	
	int l = line;
	if ( l == 4){	
		start_color();
		fillColors();	
		
		attron(COLOR_PAIR(1));
		mvprintw(0, 0, "%s", hdSize);
		mvprintw(0, 60, "Host Name: %s", hostName);	
		mvprintw(1, 60, "Kernel Name and Release:" );
		mvprintw(2, 60, "%s", kernelName);
		mvprintw(3, 60, "Number of processors: %s", numberOfProcesses);
		mvprintw(4, 60, "CPU Vendor Name: %s", vendor);
		mvprintw(5, 60, "CPU Model Name: %s", modelName);
		mvprintw(6, 60, "Total Free Ram: %s", ramSize);
		int temp = atoi(numberOfProcesses);
		int counter;
		for (counter = 0; counter < temp; counter++){
			mvprintw(counter, 130, "CPU %d Speed:%s", counter + 1, CpuSpeed);
		}
		
	attroff(COLOR_PAIR(1));
		
	}else if (l == 5){
		mvprintw(0, 0, "Host Name: %s", hostName);	
		mvprintw(1, 0, "Kernel Name and Release: %s", kernelName);
		mvprintw(2, 0, "Number of processors: %s", numberOfProcesses);
		mvprintw(3, 0, "CPU Vendor Name: %s", vendor);
		mvprintw(4, 0, "CPU Model Name:%s", modelName);
		mvprintw(5, 0, "Total Ram Size: %s kb", ramSize);
		mvprintw(6, 0, "Hard disk size");
		mvprintw(7, 0, "%s", hdSize);
		int temp = atoi(numberOfProcesses);
		int i = 8;
		int counter;
		for (counter = 1; counter <= temp; counter++){
			mvprintw(i, 60, "CPU %d Speed:%s", counter, CpuSpeed);
			i++;
		}
		
	}else if (l == 6)
	{
		mvprintw(1, 0, "Number of processors: %s", numberOfProcesses);
		mvprintw(2, 0, "CPU Vendor Name: %s", vendor);
		mvprintw(3, 0, "CPU Model Name: %s", modelName);
		int temp = atoi(numberOfProcesses);
		int i = 4;
		int counter;
		for (counter = 1; counter <= temp; counter++){
			mvprintw(i, 60, "CPU %d Speed: %s ", counter, CpuSpeed);
			i++;
		}
	}else if (l == 7)
	{
		mvprintw(0,0, "Total Ram Size: %s kb", ramSize);

	}
	refresh();
	
	
}

void showMainMenu()
{
	int line = 4;
	int key;
	
	printw("\t System monitor");
	mvprintw(12, 0, "Move arrow up/down to highlight, then press Return");
	drawMenu(line); 
	
	while(1){
		
		key = getch();
		if(key == KEY_UP){ 
			line = keyup(line); 
			drawMenu(line);
		} 
 
		if(key == KEY_DOWN){ 
			line = keydown(line); 
			drawMenu(line); 
		}
		
		if(key == 10){ 
			executeOption(line);	
		}		
	}

}

int keyup(int line){ 
	int l = line; 

	if(line > 4) 
		l--; 
	
	if(line == 4)
		l = 10;

	return l; 
} 
 
int keydown(int line)
{ 
	int l = line; 

	if(line < 10) 
		l++; 

	if(line == 10)
		l = 4;

	return l; 
} 

void executeOption(int line)
{
	cleanWindow();
	int l = line;
	args.line = &l;

	showStaticInformation(l);
	
	if(l == 10){
		removeTempFile();
		endwin();
		exit(1);
	}
	
	int key;
	while(1)
	{
		key = getchar();
		if (key == 27){
			refresh();
			escapePressed();
		}
	}
		
}

void cleanWindow(){
	clear();
}

void drawMenu(int line)
{
	
	mvprintw(2, 0, "Options:");
	if (line == 4){
		attron(A_STANDOUT); 
		mvprintw(4, 0, "Show Static and Dynamic Information");
		attroff(A_STANDOUT); 
		mvprintw(5, 0, "Show Static Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 5){
		attron(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		attroff(A_STANDOUT); 
		mvprintw(4, 0, "Show Static and Dynamic Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 6){
		attron(A_STANDOUT);
		mvprintw(6, 0, "Show CPU Information");
		attroff(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		mvprintw(4, 0, "Show Static and Dynamic Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 7){
		attron(A_STANDOUT);
		mvprintw(7, 0, "Show Ram Information");
		attroff(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(4, 0, "Show Static and Dynamic Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 8){
		attron(A_STANDOUT);
		mvprintw(8, 0, "Show Disk Information");
		attroff(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(4, 0, "Show Static and Dynamic Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 9){
		attron(A_STANDOUT);
		mvprintw(9, 0, "Show Processes Information");
		attroff(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(4, 0, "Show Static and Dynamic Information");
		mvprintw(10, 0, "Quit");
	}
	if (line == 10){
		attron(A_STANDOUT);
		mvprintw(10, 0, "Quit");
		attroff(A_STANDOUT);
		mvprintw(5, 0, "Show Static Information");
		mvprintw(6, 0, "Show CPU Information");
		mvprintw(7, 0, "Show Ram Information");
		mvprintw(8, 0, "Show Disk Information");
		mvprintw(9, 0, "Show Processes Information");
		mvprintw(4, 0, "Show Static and Dynamic Information");
	}
	
	refresh();
}

int main(int argc, char *argv[])
{
	
	// signals 
	signal(SIGINT, writeAllToLog);
	signal(SIGKILL, writeAllToLog);
	signal(SIGHUP , writeAllToLog);
	
	long time_taken = clock();
	char *time = (char *) malloc(100);
	snprintf(time, 100, "time started: %d", time_taken);
	writeData("time.txt", time);
	allocateMemory();
	
	int temp = 1;
	
	if(argc == 2){
		temp = atoi(argv[1]);
		args.refreshTime = &temp;
	}else {
		args.refreshTime = &temp;
	}
	
	// set flag to 0 
	*args.flag = 0;
	
	getStaticInformation();	
	writeStaticInformationToLog();
	getDynamicInformation();
	monitorDynamicInformation();
	launchRefreshThread();
	initscr();
	cbreak();
	keypad(stdscr, TRUE); 
	cleanWindow();
	noecho();
	showMainMenu();	
	
	free(time);

	return 0;
}

void allocateMemory()
{
	args.refreshTime =  malloc(sizeof(int));
	args.line =  malloc(sizeof(int));
	args.flag =  malloc(sizeof(int));
	args.cpuLoad =	(char *) malloc(100);
	args.currentfreeRamSize = (char *) malloc(4000);
	args.minFreeRamSize = (char *) malloc(4000);
	args.maxFreeRamSize = (char *) malloc(4000);
	args.diskSpace = (char *) malloc(4000);
	args.runningProcessesInfo = (char *) malloc(4000);
	modelName = (char *) malloc(1000);
	kernelName = (char *) malloc(500);
	vendor = (char *) malloc(500);
	CpuSpeed = (char *) malloc(500);
	ramSize = (char *) malloc(100);
	hdSize = (char *) malloc(4000);	
}

void writeStaticInformationToLog()
{
	writeData("static.txt", modelName);
	writeData("static.txt", kernelName);
	writeData("static.txt", vendor);
	writeData("static.txt", CpuSpeed);
	writeData("static.txt", ramSize);
	writeData("static.txt", hdSize);
}