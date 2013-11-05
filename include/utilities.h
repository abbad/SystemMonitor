#ifndef UTILITIES_H_INCLUDED
#define UTILITIES_H_INCLUDED

extern char * trimwhitespace(char *str);
extern char * concat();
extern void tokenizeHDInfo(char *str, char *input);
extern void removeSubstring(char *s,const char *toremove);
extern void writeData(char *fileName, char *data);
extern void writeToLog(char *file);
extern void tokenizeFreeRamInfo(char *str, char *input);
#endif 