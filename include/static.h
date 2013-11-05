#ifndef STATIC_H_INCLUDED
#define STATIC_H_INCLUDED

extern void getHostName(char *output);
extern char * getKernelNameAndRelease();
extern void getNumberOfprocessors(char *output);
extern char * getVendorOfTheProcesses();
extern char * getModelNameOfEachProcessor();
extern char * getCPUSpeed();
extern char * getTotalRamSize();
extern char * getTotalHardDiskSize(char *command);

#endif 