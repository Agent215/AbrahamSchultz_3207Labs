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
