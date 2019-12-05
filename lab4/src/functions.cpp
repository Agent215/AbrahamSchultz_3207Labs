/*
functions.cpp

Abraham Schultz
12/3/2019

contains extra utility functions as needed  file system implementation over a virtual disk.
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <string>
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <vector>
#include <queue>
#include <list>
#include <stdio.h>
#include "disk.h"
#include "header.h"

using namespace std;
/******************************************************************************/
/******************************************************************************/
// function to make a blank virtual disk ready to be mounted and used.
int make_fs(char *virtualDisk) {

  int returnValue;

  make_disk(virtualDisk);     //call make disk function passing user argument
  returnValue = open_disk(virtualDisk);       // call open disk function passing user argument
  initBootSector ();

  if(returnValue == 0){

    printf("made disk %s successfully \n",virtualDisk);
    return 0;
  }   else {

    printf("failed to create disk \n");
    return 1;
  }
} // end initdisk
/******************************************************************************/
/******************************************************************************/
  int mount_fs(char *disk_name)
  {
    int returnValue;
    returnValue = open_disk(disk_name);       // call open disk function passing user argument

    // we will also want to load the meta data for the FAT table from the super block
    // this should probably be its own function

    if(returnValue == 0){

    printf("mounted disk %s successfully \n",disk_name);
    return 0;
  }   else {

    printf("failed to mount disk \n");
    return 1;
  }
  };
/******************************************************************************/
/******************************************************************************/
// in this function we will write back all the FAT meta data to the super block
// and make sure all open files are closed
// we will then close the disk
 int umount_fs(char *disk_name)
  {
	// write meta data to first block in virtual disk memoroy
	writeMetaData();
	// close the disk that is currently open
	int val = close_disk();

	//print to terminal on failure or success
	if (val == 0){printf("disk closed correctly \n"); return 0;}
        else {return -1;}

  }
/******************************************************************************/
/******************************************************************************/
// use to clear screen when needed
int clearScreen(){
//use escape sequence to clear console
printf("\033[H\033[J");
return 0;
}
/******************************************************************************/
/******************************************************************************/
// print program instructions
int help() {
/******************************************************************************/
printf("\n***WELCOME MY VIRTUAL FILE SYSTEM***"                               ///
        "\n>*******************************\n"                                ///
        "\n Use at your own risk...\n\n"                                      ///
        "\n List of Commands supported:\n"                                    ///
        "\n exit  // exits"                                                   ///
        "\n new  // creates new disk"                                         ///
        "\n read  // opens and reads contents of a file"                      ///
        "\n mount  // mounts disk contents of a file"                         ///
        "\n unmount  // unmounts disk contents of a file"                     ///
        "\n create  // creates a file with given name"                        ///
        "\n mkdir // make a directory "                                       ///
        "\n delete  // deletes a file with given name\n"                      ///
        "\n List of Commands not yet supported:\n"                            ///
        "\n write // write some text data to file"                            ///
        "\n truncate // truncate size of file"                                ///
        "\n seek // set the disk pointer to a spot in memory"                 ///
        "\n>*******************************"                                  ///
        "\n\n");                                                                ///
/******************************************************************************/
return 0;
} //end help

/******************************************************************************/
/******************************************************************************/
// print the cwd as message prompt
int promtUser(char * wd){

char cwd[1000];
   if (getcwd(cwd, sizeof(cwd)) != NULL)
        {

            printf("%s >> %s ", cwd , wd);
        }   else
        {

            cout << strerror(errno) << endl;
            printf("\x1b[0m");
            return 1;
        }// end else
}
