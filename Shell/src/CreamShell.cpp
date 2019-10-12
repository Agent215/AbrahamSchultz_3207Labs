/*
CreamShell.cpp
Abraham Schultz
last edited 09/29/2019

This is to be my implementation of a Unix style command line interpreter (Shell) program

This file contains the main function which can accept one argument or no arguments when executed.
If any argument is provided by the user we assume that this is the name of a batch file to read input from
instead of from the command line.

if no arguments are provided then we run normally, waiting for user input from the keyboard.

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


      char *args[1000];      // declare array to use to pass to exec
      char *args2[1000];     // declare array to use to pass to exec for second set of commands
      extern int errno;      // errno for use with std error
      char *PATH[1000];      // path string to set the path for exec
      char * filename;
      int batchFile;         // flag to tell shell if we are using a batch file as input
      int execNow;           // flag is 1 if we detect & at end of args
      int multipleArgs;      // 1 if we have more than one argument per line
      int redirecting;        // 1 if redirecting
      int piping;            // 1 if piping
      int isPaused;          // 1 if paused

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************

int main (int argc, char *argv[])
        {

      // init as not using batch file
      batchFile = 0;
	  execNow = 0;
	  isPaused = 0;
	  redirecting = 0;
      char* tmp;    // to hold user input for temp manipulation
      int swh;     //internal command switch variable

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

      while (running == 0 )
        {
           // asdasd
           redirecting = 0;
           execNow = 0;
           int rtrn;


           //create a string buf to hold user input
           char* buf;
           buf = NULL;
           // if we are not using a batch file then print prompt otherwise do nothing
           if (batchFile == 0 && isPaused != 1)
           printDir();

           //user input is white
           resetColor();
           // get user input and wait for user to hit enter
           buf = readline("");  isPaused = 0; // unpause after we take input
           // we gotta check if null before duplicating otherwise we segfault
           if (buf != NULL)
           tmp  = strdup(buf);  // duplicate input
           // if we are using a batch file and it is empty then we are done here
           if(batchFile == 1 && buf == NULL) {break;}


            // parse args
            parseArgs(buf);
            // check for null stuff before going to internal commands switch
            if (buf != NULL && args != NULL)
            swh = intrnFunc( tmp, args);
            if (swh == 0){  // i we used an internal command dont do anything
            }
            else
            if (swh == 2)  // if we reutn 2 then we chose to exit
            {
                 running =1;
            }else
             if (swh == 3){

                isPaused = 1;
            } // if we return 3 then we paused
            else

           if(redirecting == 0 && swh == 1 )      // if we are not redirecting , and its not an internal command
            {   rtrn == execArgs(args);}         // then exec like only one command
           else
           // if we are redirecting then parse the two commands we need to redirect input and output too
//            if (redirecting == 1)
//           {
//             execArgsRedirect(args,args2);
//           }
           // if exec worked
           if (rtrn > 0)
		   {   // check if we waiting
			  if(execNow == 0)
			  {

                    {wait(NULL);}
                    {wait(NULL);}
	          }
			  else { cout << "exec imiditatly" << endl; }
		   }

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
            string firstArg;
            string secondArg;
            //copy the whole line from the user
            char* tmp  = strdup(buf);

               //this is probably no the best way to parse multiple arguments but its a method i have used before
               int check = checkForRedirect(tmp);                // check the input for redirection symbol

               if (check== 0){                                   // 0 = > so we are redirecting

               string s (buf);                                   // convert to c++ style string because we can use fancy c++ std namespace to parse

               int appenRe = s.find(">");                     // index position of redirection symbol in large string


               //  string firstWord = InitData[j].substr(0, InitData[j].find(" "));

               firstArg = s.substr(0, appenRe -1);                  // first arg is from index 0 to where the position of the redirect symbol is
               secondArg =  s.substr(appenRe + 1);                   // second arg is from the redirect symbol to the end of the string

               // debugging
                 printf("first arg is %s", firstArg.c_str());
                 printf("second arg is %s", secondArg.c_str());
            }

            /*
            so because .c_str() function returns a pointer
            and strtok takes the literal string we use a temp variable
            to hold the string containing the first argument up until we hit the indicated delimiter
            */
            char * cmd;
            if (check == 0) {

               tmp = strdup(firstArg.c_str());
               cmd = strtok(tmp," ");
               } else {   cmd = strtok(buf," "); }



            // break off individual strings using the space as a delimiter until end of line
            while (cmd != NULL)
            {
                    args[i] = cmd;
                    if (strcmp(cmd, "&") == 0)
                        {
                        execNow = 1; cout << "detected ampersand" << endl;
                        break;
                        } else{execNow = 0;}
                cmd = strtok (NULL, " ");
                i ++;

            }// end while
             args[i] = NULL;

            // for now only break up second argument when > symbol is found
            if (check == 0)
                {
                    i = 0;
                    // here we break up the second command and its arguments if provided
                    tmp = strdup(secondArg.c_str());
                    cmd = strtok(tmp," ");
                    while (cmd != NULL)
                    {

                            args2[i] = cmd;

                            if (strcmp(cmd, "&") == 0)
                                {
                                execNow = 1; cout << "detected ampersand" << endl;
                                break;
                                }
                        cmd = strtok (NULL, " ");
                        i ++;


                    } // end while
                    args2[i] = NULL;
                    // set lat array element to null to make exec work correctly
                 } // end if

            return 0;

        }// end parseArgs

//*****************************************************************************************************************************************
//*****************************************************************************************************************************************

int execArgs(char * argsIn[]){

 if (fork() == 0)
            {

                        // if what the user entered doesnt makes sense
                        if (execvp(argsIn[0], argsIn) < 0)
                            {
                             //change font to red and print std error
                             printf("\u001b[31m");
                             cout << strerror(errno) << endl;
                              printf("\x1b[0m");
                             exit(10);
                             return 0;

                            }                            //we look in the/bin directory where system programs should be
                              //this should never print
                              printf("I am the Child! %i\n", getpid());

            } // end if child

            else {


                 if(execNow == 0)
			  {

                    {wait(NULL);}
              }
            }

} // end execArgs
//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
//int execRedirect(){}// end execRedirect()


// this function will check is there is a redirect symbol will return
// 0 = >
// 1 = >>
// 2 = <
// 3 = <<
//  -1 = no redirect
int checkForRedirect(char* in){

int returnVal = -1;
char * cmd;
cmd = strtok(in," ");

// we will break apart a copy of the whole line input by the user
 while (cmd != NULL)
            {


                    if (strcmp(cmd, ">") == 0)
                        {
                        returnVal = 0; cout << "> detected" << endl;
                        redirecting =1;
                        return returnVal;

                        }else
                    if(strcmp(cmd, ">>") == 0)
                        {
                        returnVal = 1; cout << ">> detected" << endl;
                        redirecting =1;
                        return returnVal;

                        }
                    else
                    if(strcmp(cmd, "<") == 0)
                        {
                        returnVal = 2; cout << "< detected" << endl;
                        redirecting =1;
                        return returnVal;

                        }
                        else
                    if(strcmp(cmd, ">>") == 0)
                        {
                        returnVal = 3; cout << "<< detected" << endl;
                        redirecting =1;
                        return returnVal;

                        }
                        else {
                            redirecting =0;
                            returnVal = -1;
                        }
                cmd = strtok (NULL, " ");

            }// end while

 return returnVal;

}// end checkForRedirect



//*****************************************************************************************************************************************
//*****************************************************************************************************************************************
// this function should fork and exec twice redirecting the output of the first process to the input of the second.
// takes an array of string where the first is the command the following strings are the arguments for each command
int execArgsRedirect(char * args1In[],char * args2In[]){


int fds[2]; // file descriptors
int count;  //
int fd;     //
char c;     //

pid_t pid1;  // associated pids to wait
pid_t pid2;

 //first child for first command
 if ((pid1 = fork()) == 0)
      {

//
////
//          fd = open(fds[0], O_RDWR | O_CREAT, 0666);
////
////           // open() returns a -1 if an error occurred
//          if (fd < 0) {
//                      printf("Error: %s\n", strerror(errno));
//                      return 0;
//                      }
//
//          dup2(fds[0], 0);
//
////         Don't need stdout end of pipe.
//        close(fds[1]);


                        // if what the user entered doesnt makes sense
                        if (execvp(args1In[0], args1In) < 0)
                            {
                             //change font to red and print std error
                             printf("\u001b[31m");
                             cout << strerror(errno) << endl;
                             exit(10);
                             return 0;

                            }                            //we look in the/bin directory where system programs should be
                              //this should never print
                              printf("I am the Child! %i\n", getpid());
           }
                        else if ((pid2 = fork()) == 0)

             {



                        // if what the user entered doesnt makes sense
                        if (execvp(args2In[0], args2In) < 0)
                            {
                             //change font to red and print std error
                             printf("\u001b[31m");
                             cout << strerror(errno) << endl;
                             exit(10);
                             return 0;

                            }                            //we look in the/bin directory where system programs should be
                              //this should never print
                              printf("I am the Child! %i\n", getpid());

            }// end if child
            else {





                 if(execNow == 0)
			  {

                    {wait(NULL);}
                    {wait(NULL);}
              }
            }





//                waitpid(pid1, NULL, 0);
//                waitpid(pid2, NULL, 0);
//





} // end execArgs



