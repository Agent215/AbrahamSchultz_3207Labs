/*
main.cpp

Abraham Schultz
11/24/2019

This is the file containing the main function for the file system project
at temple university.
This is part of lab 4.

There should be a shell to accept user commands.
This will build a file system on top of a single linux file.
Within this file should be a virtual disk capable of storing files
and directories.


*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include "disk.h"
#include <readline/readline.h>
#include <readline/history.h>



/******************************************************************************/
/*
Main function should act as a shell program to accept commands
we should be able to accept commands:
1. create file
2. delete file
3. open file
4. exit/unmount
*/
int main (int argc , char** argv){

// flags
int running = 0;


while (running == 0) {
//create a string buf to hold user input
char* buf;
buf = NULL;

// get user input and wait for user to hit enter
buf = readline("");
char * DiskName = "testDisk";

//int testDisk = make_disk(DiskName);
open_disk(DiskName);

}
return 0;
}// end main
/******************************************************************************/
