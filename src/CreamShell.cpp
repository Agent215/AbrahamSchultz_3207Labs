/*
CreamShell.cpp
Abraham Schultz



./creamShell
CACHE RULES EVERYTHING AROUND ME
*/
#include <stdio.h>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <cstdlib>
#include<sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <sys/types.h>
#include <dirent.h>
#include <limits.h>
#include "CreamShell.h"

using namespace std;


    //declare array to use to pass to exec
      char *argv[1000];


int main ()
        {

      // clear terminal on start
      clear();

      //flag for running
      int running = 0;

      while (running == 0 ){
      // use escape sequence to change terminal color to green for prompt
      printf("\u001b[32m");
      //create a string buf to hold user input
      char* buf;
      printDir();

      //user input is white
      resetColor();
      // get user input and wait for user to hit enter
      buf = readline("");



      // check if echo
      if(strcmp(buf, "echo") == 0){ echo(buf); }
      // parse args
      parseArgs(buf);
      //these will all be under internal command switch
      // if user wants to exit type exit
      if(strcmp(buf, "exit") == 0){  running =1 ;  printf("\ngoodbye \n"); break; }
      else // if user entered clr then clear screen. this will be moved the handleInternal() function
      if(strcmp(buf, "clr") == 0){  clear();}
      else
      if(strcmp(buf, "cd") == 0){  cd(argv);}
      else
      if(strcmp(buf, "dir") == 0){ dir(); }
      else
        // if fork returns 0 then this is child, this will be in the exec arg function later
        if (fork() == 0)
        {

                    // if what the user entered doesnt makes sense
                    if (execvp(argv[0], argv) < 0)
                        {
                         printf("\u001b[31m command not recognized \n");
                         return 0;
                        }                            //we look in the/bin directory where system programs should be
                        execv("/bin", argv);         // child process becomes what user enters in

             //this should never print
             printf("I am the Child! %i\n", getpid());
        } // end if child


        // if fork returns 1 then this is parent
        else
        {
            // wait for child to return
             wait(NULL);
           //  printf("I am the Parent! %i\n",getpid());
        } // end else if parent

        // the readline function mallocs memory we must not forget to free this
        free(buf);

      } // end while

      // change color back to normal on console using escape sequences
      printf("\x1b[0m");
        }  // END MAIN

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
        // this will be the function to tokenize input from the user into discreet arguments

        string* parseArgs(char* buf)
        {
          int i = 0;
          string* returnStr;
          char * cmd;
          cmd = strtok (buf," ");
          // break off individual strings using the space as a delimiter until end of line
          while (cmd != NULL)
            {
            argv[i] = cmd;
           // printf ("%s\n",   cmd);
            cmd = strtok (NULL, " ");
            i ++;
            }
            // set lat array element to null to make exec work correctly
            argv[i] = NULL;
            return returnStr;

        }// end parseArgs
//*****************************************************************************************************************************************
        // function to clear screen
        int clear(){
        //use escape sequence to clear console
        printf("\033[H\033[J") ;
        return 0;
        }// end clear

//*****************************************************************************************************************************************
        //function to echo to shell whatever user types
        int echo(char* args)
        {

         printf("%s",args);

         return 0;
        }// end echo

//*****************************************************************************************************************************************
//function that is equivlent to linux ls command
// this prints out the files in the current working directory.
       int dir()
       {
       struct dirent *ent;  // Pointer for directory entry
        //set color of file txt :))
        printf("\u001b[34m");
        // opendir() returns a pointer of DIR type.
        DIR *newDir = opendir(".");

        if (newDir == NULL)  // opendir returns NULL if couldn't open directory
        {
        printf("\u001b[31m Could not open current directory " );
        return 0;
        } // end if

        int i =0;
        // use readdir to look at current files in directory any print them out
        while ((ent = readdir(newDir)) != NULL)
            {

            if ( !strcmp(ent->d_name, ".") || !strcmp(ent->d_name, "..") )
                {
                     // dont print the current directoy and parent directory
                } else {
                    printf("%s   ", ent->d_name);
                    i++;
                } // end if else


            }// end while

        printf("\n");
        closedir(newDir);
        printf("\u001b[32m");// put color back to green
        return 0;
       }// end dir

//*****************************************************************************************************************************************
// function to change directory
int cd( char *argv[])
{
//cout << "change to this directory " << argv[1] << endl;
 chdir(argv[1]);
}





//*****************************************************************************************************************************************
int printDir(){

 char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL) {
       printf("%s >>$CREAM$HELL$>> ", cwd);
   } else {
       perror(" \u001b[31m  getcwd() error");
       return 1;
   }
} // end printDir

int resetColor(){
       //put color back to white for user input
      printf("\x1b[0m");
}
