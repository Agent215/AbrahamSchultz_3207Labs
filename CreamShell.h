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
//dir()
// env()
// cd()
int echo(char* args);
// getUserInput ()
// welcomeMsg ()
// HandleInternal()
// HandleExternal()
string *parseArgs(char* buf) ;
//HandleRedrirect
//HandlePipe()
