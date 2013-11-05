#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "utilities.h"

char *trimwhitespace(char *str)
{
	char *end;

	// Trim leading space
	while(isspace(*str)) str++;

	if(*str == 0)  // All spaces?
	return str;

	// Trim trailing space
	end = str + strlen(str) - 1;
	while(end > str && isspace(*end)) end--;

	// Write new null terminator
	*(end+1) = 0;

	return str;
}

void removeSubstring(char *s,const char *toremove)
{
  while( s=strstr(s,toremove) )
    memmove(s,s+strlen(toremove),1+strlen(s+strlen(toremove)));
}

void tokenizeHDInfo(char *str, char *input)
{
	char *token, *temp;
	token = (char *) malloc(1000);
	temp = (char *) malloc(1000);
	const char s[2] = ":";
	
	token = strtok(str, s); // c
	
	while(token != NULL )
	{	
		strcpy(temp, token); //c
		strcat(input, trimwhitespace(temp));
		strcat(input, " ");
		token = strtok(NULL, s);	
	}
	
	free(temp);
	temp = NULL;
	free(token);
}

char * concat(char *s1, char *s2)
{
    char *result = malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
    //in real code you would check for errors in malloc here
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void tokenizeFreeRamInfo(char *str, char *input)
{
	char *token;
	token = (char *) malloc(1000);	
	const char s[2] = " ";
	token = strtok(str, s); 
	token = strtok (NULL, s);
	strcpy(input, token);
	
}

void writeData(char *fileName, char *data)
{
	char *tempData = (char *) malloc(5000);
	
	FILE *fp;
	fp=fopen(fileName, "a+");
	
	if (fp == NULL) {
        perror("Failed to open file");
    }
	
	strcpy(tempData, data);
	strcat(tempData, "\r\n");
	fputs(tempData, fp);
	
	free(tempData);
	if(fclose(fp) == -1)
	{
		perror("file close");
	}
	
}

void writeToLog(char *file)
{
	int c; 
	FILE *fin, *fout; 
 
	fin = fopen(file,"r"); 
	fout = fopen("log.txt","a+"); 
 
	if(fin != NULL && fout != NULL){ 
	while((c = fgetc(fin)) != EOF) 
		fputc(c,fout); 

		fclose(fin); 
		fclose(fout); 
	} 
		else 
	perror("open error"); 
	
}