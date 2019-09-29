/*
CreamShell.cpp
Abraham Schultz

This is to be my implementation of a unix style command line interpreter (Shell) program




./creamShell
CACHE RULES EVERYTHING AROUND ME
/CACHE REDUCES EXTRA ALLOTMENT OF MEMORY
*/
#include <stdio.h>
#include <string.h>
#include <string>
#include <fcntl.h>
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
#include <errno.h>
#include "CreamShell.h"

using namespace std;


      //declare array to use to pass to exec
      char *args[1000];
      // errno for use with std error
      extern int errno;
      // path string to set the path for exec
      char *PATH[1000];
      char * filename;

      // flag to tell shell if we are using a batch file as input
      int batchFile;

int main (int argc, char *argv[])
        {

      // init as not using batch file
      batchFile = 0;

      // first check if user provided batch file
      // if the user provides any arg with the initial execution assume that it is a batch file name
      if (argc >1)
      {

          //set flag
          batchFile =1;

          filename = argv[1];
          int newstdin = open(filename,O_RDONLY);

              //returns a -1 if an error occurred
              if (newstdin < 0) {
              printf("Error: %s\n", strerror(errno));
              return 0;
            }

          close(0);
          dup(newstdin);
          close(newstdin);
      }// end if



      // clear terminal on start
      clear();

      //flag for running
      int running = 0;

      while (running == 0 ){
      // use escape sequence to change terminal color to green for prompt
      printf("\u001b[32m");
      //create a string buf to hold user input
      char* buf;

      // if we are not using a batch file then print prompt otherwise do nothing
      if (batchFile == 0)
      printDir();



      //user input is white
      resetColor();
      // get user input and wait for user to hit enter
      buf = readline("");

      // if we are using a batch file and it is empty then we are done here
      if(batchFile == 1 && buf == NULL) {return 0;}


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
      if(strcmp(buf, "cd") == 0){  cd(args);}
      else
      if(strcmp(buf, "dir") == 0){ dir(); }
      else
         //if fork returns 0 then this is child, this will be in the exec arg function later
        if (fork() == 0)
        {

                    // if what the user entered doesnt makes sense
                    if (execvp(args[0], args) < 0)
                        {
                         //change font to red and print std error
                         printf("\u001b[31m");
                         cout << strerror(errno) << endl;
                         return 0;
                        }                            //we look in the/bin directory where system programs should be
                       	  //this should never print
                          printf("I am the Child! %i\n", getpid());
                          //execv("/bin", argv);         // child process becomes what user enters in

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
      return 0;
        }  // END MAIN

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
        // this will be the function to tokenize input from the user into discreet arguments

        int parseArgs(char* buf)
        {
           int i = 0;

           char * cmd;
           cmd = strtok (buf," ");
          // break off individual strings using the space as a delimiter until end of line
           while (cmd != NULL)
            {
            args[i] = cmd;
           // printf ("%s\n",   cmd);
            cmd = strtok (NULL, " ");
            i ++;
            }
            // set lat array element to null to make exec work correctly
            args[i] = NULL;
            return 0;

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
        //set color of file txt :)
        printf("\u001b[34m");
        // opendir() returns a pointer of DIR type.
        DIR *newDir = opendir(".");

        if (newDir == NULL)  // opendir returns NULL if couldn't open directory
        {
                        //change font to red and print std error
                         printf("\u001b[31m");
                         cout << strerror(errno) << endl;

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

        printf("\n\u001b[32m");// put color back to green
        closedir(newDir);
        return 0;

       }// end dir

//*****************************************************************************************************************************************
// function to change directory
int cd( char *args[])
{
//cout << "change to this directory " << argv[1] << endl;
 if (chdir(args[1]) == 0 )
    {
    // we found target directory

    } else
    {
         //change font to red and print std error
        printf("\u001b[31m");
        cout << strerror(errno) << endl;
    }
 return 0;
}// end cd





//*****************************************************************************************************************************************
int printDir(){

   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {
            printf("%s >>$CREAM$HELL$>>  ", cwd);
        }   else
        {
            //change font to red and print std error
            printf("\u001b[31m");
            cout << strerror(errno) << endl;
            return 1;
        }// end else
} // end printDir


//*****************************************************************************************************************************************
int resetColor(){
       //put color back to white for user input
      printf("\x1b[0m");
      return 0;
}// end resetColor
