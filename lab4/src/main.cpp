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
#include <vector>
#include <queue>
#include <list>
#include <stdio.h>
#include "disk.h"
#include "header.h"

#define MAX_OPEN_FILE   64

using namespace std;
/******************************************************************************/
struct FAT fat;                                                              ///
struct File root_dir; 														 ///
int fileDesCount;                                                            ///
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

//allocate memory
//FAT fat = new FAT;
//File root_dir = new File;

// start with no open files
fileDesCount = 0;
// flags
int running = 0;


/******************************************************************************/
printf("\n***WELCOME MY VIRTUAL FILE SYSTEM***"                               ///
        "\n>*******************************\n"                                ///
        "\n Use at your own risk...\n"                                        ///
        "\n List of Commands supported:"                                      ///
        "\n exit  // exits"                                                   ///
        "\n new  // creates new disk"                                         ///
        "\n read  // opens and reads contents of a file"                      ///
        "\n mount  // mounts disk contents of a file"                         ///
        "\n>*******************************"                                  ///
        "\n");                                                                ///
/******************************************************************************/
while (running == 0) {
//create a string buf to hold user input
char* buf;
buf = NULL;

// prompt user
printf("Please type a command \n" );

// get user input and wait for user to hit enter
buf = readline("");

// below we will check for matching words. this probably should be contained in a parse input function
if(strcmp(buf, "exit")== 0)
{

printf("Goodbye\n" );
running = 1;

}else
if(strcmp(buf, "new")== 0)
{

    // prompt user
    printf("Please type name of disk\n" );
    // get user input and wait for user to hit enter
    buf = readline("");
    // make new disk using user input as name
    make_fs(buf);
}
else
if(strcmp(buf, "read")== 0)
{

    // prompt user
    printf("Please type name of file to read\n" );
    // get user input and wait for user to hit enter
    buf = readline("");
    // make new disk using user input as name
    readFile(buf);

    printFat();
    block_write(0,"testing data");
    char * tmp;
    tmp = (char *)malloc(1 * sizeof(char));
    block_read(0, tmp);
    printf("data from block 1 testing :\n %s \n",tmp);

 //   free (tmp);
}
else
if(strcmp(buf, "mount")== 0)
{
   //  mount_fs(buf);
}
} // end while



return 0;
}// end main
/******************************************************************************/
//                      FUNCTIONS                                            ///
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

/*
function to initialize the first block of memory on disk with FAT
and also to add the root directory
*/
int initBootSector (){

	struct Block superBlock;

	fat.TotalBlocks = DISK_BLOCKS;
	fat.UnusedBlocks = DISK_BLOCKS;
	root_dir.FileSize = 32;           // allocate 32 bytes to root directory
	root_dir.isDir = 0; // is dir
	root_dir.startingAddr = 0;
	root_dir.filePointer = 0;
	// use strncpy to make sure no buffer overflow
	strncpy(root_dir.filename, "/", sizeof(root_dir.filename) - 1);
	superBlock.isUsed = 1;
	superBlock.blockNum = 1;
	root_dir.blockList.push_back(superBlock);    // add super block to root dir meta data
	fat.UnusedBlocks = DISK_BLOCKS - 1;
	fat.FAT.push_back(root_dir); // add root dir to fat table

	// write to disk // testing for debugging
    //	block_write(superBlock.blockNum, "testing data");
	printf("creating super block at block %i \n file descriptor count %i \n", superBlock.blockNum, fileDesCount);

	//debugging
	//printFat();



return 0;
} // end initBootSector
/******************************************************************************/
  int umount_fs(char *disk_name)
  {
      return 0;
  }
/******************************************************************************/
/*
This function should check to see if given file exists, then if it does
check to see if we have available file descriptors, if we do
then assign a file descriptor to file and increment count for file descriptor
*/
  int fs_open(char *name){

  // check if file exists
  for (int i =0 ; i < fat.FAT.size(); i ++){

   if (strcmp(fat.FAT.at(i).filename, name)== 0 )
        {
         printf("\n opening file : %s... \n", name);
        // check if we have enough file descriptors
        if (fileDesCount <MAX_OPEN_FILE)
            {
                // set file descriptor + 1
                fileDesCount ++;
                fat.FAT.at(i).filedes = fileDesCount;
                printf("setting file descriptor %i \n", fileDesCount);
            }else {return 1;}
        } // end if

  } // end for
    return 0;
  } // end fs_open
/******************************************************************************/
/*
This function should check if file is open and then close by setting file des to -1
and returning 0.
other wise return -1
*/
  int fs_close(int fildes)
  {
  int returnvalue = -1;
  // check if file exists/ is open
  for (int i =0 ; i < fat.FAT.size(); i ++){

   if (fat.FAT.at(i).filedes == fildes )
        {

            //return -1 and set file to closed
            returnvalue =fat.FAT.at(i).filedes = -1;
            return 0;
        } // end if

  } // end for
  printf("failed to close file. file does not exist or is not open \n");
  return -1;

  };
/******************************************************************************/
  int fs_create(char *name);
/******************************************************************************/
  int fs_delete(char *name);
/******************************************************************************/
  int fs_mkdir(char *name);
/******************************************************************************/

  int fs_read(int fildes, void *buf, size_t nbyte)
              {
	  char * tmp;
	  tmp = (char *)malloc(1 * sizeof(char));


   //check if file exists
   for (int i =0 ; i < fat.FAT.size(); i ++){

   if (fat.FAT.at(i).filedes == fildes )
        {

            printf("found file with descriptor %i", fildes);
//
//            //return try and read
          if (block_read(1 , (char*)tmp) > 0)
                {
                     printf("\n\n date from read: %p",tmp);
                    return 0;
                } else {
                  printf("\n\n problem with reading file %i",fildes);
                }

        } // end if

  } // end for
  printf("file descriptor count %i \n", fileDesCount);
  printf("failed read \n");
  return -1;
              }
/******************************************************************************/
  int fs_write(int fildes, void *buf, size_t nbyte);
/******************************************************************************/
/*
This function grabs the correct file from FAT using the file descriptor
then returns the file size. this only works if the file des exists,ie
that the file has been opened.
*/
  int fs_get_filesize(int fildes)
  {
  int returnvalue = -1;
  // check if file exists/ is open
  for (int i =0 ; i < fat.FAT.size(); i ++)
    {

        if (fat.FAT.at(i).filedes == fildes )
        {
            returnvalue =fat.FAT.at(i).FileSize;
            return returnvalue;
        } // end if

  } // end for
  printf("failed to get file size. file does not exist or is not open \n");
  return returnvalue;
  }
/******************************************************************************/
  int fs_lseek(int fildes, off_t offset);
/******************************************************************************/
  int fs_truncate(int fildes, off_t length);
/******************************************************************************/
/*
testing function for reading contents of file
*/
int readFile(char * name)
{
void * tmp;
//open file
fs_open(name);
int des;
       for (int i =0 ; i < fat.FAT.size(); i ++)
        {

           // get file descriptor
            if (strcmp(fat.FAT.at(i).filename, (char*)name)== 0 )
                {
                    des =fat.FAT.at(i).filedes;

                } // end if

        } // end for

    fs_read(des, tmp,BLOCK_SIZE );
    printf("file descriptor count %i \n", fileDesCount);
return 0;
} // end openFile

/******************************************************************************/

//testing function
int printFat()
{

 printf("size: %i , Is Dir: %i , Strt Addr: %i  , pointer : %i \n",root_dir.FileSize,root_dir.isDir,root_dir.startingAddr,root_dir.filePointer);

  printf("size of FAT %i \n\n ", fat.FAT.size());
  printf("used blocks : %i \n" , fat.UnusedBlocks);
for (int i =0 ; i < fat.FAT.size(); i ++)
 {

     printf("element in FAT: %i \n  ", i );
     printf("***********************\n  " );
     printf("FILE NAME: %s \n  ",fat.FAT.at(i).filename );
     printf("FILE SIZE: %d \n  ",fat.FAT.at(i).FileSize );
     printf("FILE DES: %i \n  ",fat.FAT.at(i).filedes );
     printf("FILE START ADDR: %i \n  ",fat.FAT.at(i).startingAddr );
     printf("FILE PARENT: %i \n\n\n  ",fat.FAT.at(i).parent );


  } // end for]
  return 0;
}
