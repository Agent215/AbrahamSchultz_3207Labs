/*
testFork.c
Abraham Schultz

this is me testing how fork() and exec(() system calls can be
implemented. In this case the program will try and create a child and automatically exec to
whatever the user has input. at first we will trust, however naively that the user knows exactly what programs
can be run.

to be the
./creamShell
CACHE RULES EVERYTHING AROUND ME
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include<sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "CreamShell.h"


int main ()
        {

      //flag for running
      int running = 0;

      while (running == 0 ){
      //create a string buf to hold user input
      char* buf;
      // get user input and wait for user to hit enter
      buf = readline("\nCREAM$HELL$$$ ");


      // if user wants to exit type exit
      if(strcmp(buf, "exit") == 0){  running =1 ;  printf("\n goodbye \n"); break; }
      else // if user entered clr then clear screen. this will be moved the handleInternal() function
      if(strcmp(buf, "clr") == 0){  clear(); }
      else
        // if fork returns 0 then this is child, this will be in the exec arg function later
        if (fork() == 0)
        {
                    //create an array to hold the arguments to pass to exec
                    // for now we are just taking whatever the user says
                    char *argv[] = { buf, 0 };   //first item is whatever user enters

                    // if what the user entered doesnt makes sense
                    if (execvp(argv[0], argv) < 0)
                        {
                         printf("command not recognized \n");
                         return 0;
                        }                        //we look in the/bin directory where system programs should be
                        execv("/bin", argv);         // child process becomes what user enters in
             //this should never print
             printf("I am the Child! %i\n", getpid());
        } // end if child


        // if fork returns 1 then this is parent
        else
        {
            // wait for child to return
             wait(NULL);
             printf("I am the Parent! %i\n",getpid());
        } // end else if parent

      } // end while



        }  // END MAIN

        // function to clear screen
        int clear(){
        printf("\033[H\033[J") ;
        return 0;
        }





