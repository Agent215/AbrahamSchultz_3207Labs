/*
internalFunctions.cpp
Abraham Schultz
last edited 09/29/2019

This is the file containing the internal command event handlers
for my Unix style CLI program (command line interpreter ).

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


//*****************************************************************************************************************************************
        // function to clear screen, using ANSI escape sequences
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
//function that is equivalent to Linux ls command
// this prints out the files in the current working directory.
       int dir()
       {
        struct dirent *ent;      // Pointer for directory entry

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
//function to print the directory name and prompt the user
int printDir(){

   char cwd[PATH_MAX];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {


	         // use escape sequence to change terminal color to green for prompt
	        printf("\u001b[32m");
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
// this is a helper function to set terminal text back to white using ANSI escape sequences
int resetColor(){
       //put color back to white for user input
      printf("\x1b[0m");
      return 0;
}// end resetColor

//*****************************************************************************************************************************************
// function to change directory
//utilizes the chdir() function and gets passes a directory name to change too
int cd( char *args[])
{

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
// function to list all the linux environment variables
// print the environment strings
/* •Current user
 * •User’s home path
 * •Shell name
 * •OS type
 * •Hostname
 * •Directories to search to find an executable.
 */

void envr(){

char* Path;
char* User;
char* Shell;
char* OsType;
char* Host;


 Path = getenv ("PATH");
 User = getenv("USER");
 Shell = getenv ("SHELL");
 OsType = getenv ("OSTYPE");
 Host = getenv("HOST");


printf ("PATH=%s\nUSER=%s\nSHELL=%s\nOSTYPE=%s\nHOST=%s\n",Path,User,Shell,OsType,Host);


}// end env
