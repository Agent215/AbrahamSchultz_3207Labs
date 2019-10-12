/*
CreamShell.h
Abraham Schultz
Last edited 9/20/2019

This is the header file for the CLI (command line interpreter ) Lab 2
for temple university's 3207 OS class




*/
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include<sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;
//**********************************************************************************************************
//***************function declarations**********************************************************************
//**********************************************************************************************************
int clear();
int help();
// pause()
int dir();
void envr();
int cd( char *argv[]);
int echo(char* args);
int execArgsRedirect(char * args1In[],char * args2In[]);
int parseArgs(char* buf) ;
int printDir();
int execArgs(char * args[]);
//handle &
int checkForRedirect(char* buf);
int execRedirect();
//parsePipe()
int resetColor();
int intrnFunc(char* cmd ,char * args[]);
