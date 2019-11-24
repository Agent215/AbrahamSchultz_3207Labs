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
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <stdio.h>
#include "disk.h"
#include "header.h"
/******************************************************************************/
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


printf("\n***WELCOME MY VIRTUAL FILE SYSTEM***"
        "\n>*******************************"
        "\n Use at your own risk..."
        "\n List of Commands supported:"
        "\n exit  // exits"
        "\n new  // creates new disk"
        "\n>*******************************"
        "\n");



// flags
int running = 0;


while (running == 0) {
//create a string buf to hold user input
char* buf;
buf = NULL;

// prompt user
printf("Please type a command \n" );

// get user input and wait for user to hit enter
buf = readline("");

if(strcmp(buf, "exit")== 0)
{
 break;
 return 0;
}else
if(strcmp(buf, "new")== 0)
{

    // prompt user
    printf("Please type name of disk\n" );

    // get user input and wait for user to hit enter
    buf = readline("");
    make_fs(buf);
}

} // end while
return 0;
}// end main
/******************************************************************************/
//                      FUNCTIONS
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
  int mount_fs(char *disk_name)
  {
    int returnValue;
    returnValue = open_disk(virtualDisk);       // call open disk function passing user argument
   if(returnValue == 0){

    printf("mounted disk %s successfully \n",virtualDisk);
    return 0;
  }   else {

    printf("failed to mount disk \n");
    return 1;
  }
  };
/******************************************************************************/
  int umount_fs(char *disk_name);
  int fs_open(char *name);
  int fs_close(int fildes);
  int fs_create(char *name);
  int fs_delete(char *name);
  int fs_mkdir(char *name);
  int fs_read(int fildes, void *buf, size_t nbyte);
  int fs_write(int fildes, void *buf, size_t nbyte);
  int fs_get_filesize(int fildes);
  int fs_lseek(int fildes, off_t offset);
  int fs_truncate(int fildes, off_t length);



