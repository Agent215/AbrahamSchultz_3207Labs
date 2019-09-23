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
// help()
// pause()
int dir();
// env()
int cd( char *argv[]);
int echo(char* args);
// getUserInput ()
// welcomeMsg ()
// HandleInternal()
// HandleExternal()
int parseArgs(char* buf) ;
int printDir();
//execArg()
//handle &
//parseRedrirect
//parsePipe()
int resetColor();
