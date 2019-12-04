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

#define MAX_OPEN_FILE   64

using namespace std;

/******************************************************************************/
/******************************************************************************/
struct FAT fat;                                                              ///
struct File root_dir; 														 ///
int fileDesCount;                                                            ///
char * cwd  ;   /* keep track of what directory we are in */                 ///
int filep ;/* number of last block written to*/                              ///
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

// start with no open files
fileDesCount = 0;
// flag for running while loop
int running = 0;
char * tmp;
//use escape sequence to clear console
clearScreen();
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
        "\n delete  // deletes a file with given name\n"                      ///
        "\n List of Commands not yet supported:\n"                            ///
        "\n write // write some text data to file"                            ///
        "\n truncate // truncate size of file"                                ///
        "\n seek // set the disk pointer to a spot in memory"                 ///
        "\n mkdir // make a directory "                                       ///
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
        //  readFile(buf);

        printFat();
        // block_write(0,"testing data");
        tmp = (char *)malloc(1 * sizeof(char));
        block_read(0, tmp);
        printf("data from block 1 testing :\n %s \n",tmp);

    }
    else
    if(strcmp(buf, "mount")== 0)
    {

         // prompt user
        printf("Please type name of the disk to mount\n" );
        // get user input and wait for user to hit enter
        buf = readline("");
        // mount disk
        mount_fs(buf);
    }
    else
    if(strcmp(buf, "unmount")== 0)
    {
        // prompt user
        printf("Please type name of the disk to unmount\n" );
        // get user input and wait for user to hit enter
        buf = readline("");
        // unmount disk
        umount_fs(buf);
    }
    else
    if(strcmp(buf, "create")== 0)
    {

        // prompt user
        printf("Please type name of the new file to make\n" );
        // get user input and wait for user to hit enter
        buf = readline("");
        // make new file using user input as name
        fs_create(buf);
    }
    else
    if(strcmp(buf, "delete")== 0)
    {

        // prompt user
        printf("Please type name of the new file to delete\n" );
        // get user input and wait for user to hit enter
        buf = readline("");
        // make new file using user input as name
        fs_delete(buf);

    }
     else
    if(strcmp(buf, "print")== 0)
    {

       // for easy testing
       printFat();

    }
      else
    if(strcmp(buf, "clear")== 0)
    {

      clearScreen();

    }
}// end while

return 0;
}// end main

/******************************************************************************/
//                      FUNCTIONS                                            ///
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
	superBlock.blockNum = 0;
	root_dir.blockList.push_back(superBlock);    // add super block to root dir meta data
	fat.UnusedBlocks = DISK_BLOCKS - 1;
	fat.FAT.push_back(root_dir); // add root dir to fat table

	// we start in the root directory
	cwd = "/";
	// write to disk // testing for debugging
    block_write(superBlock.blockNum, "boot sector");
	printf("creating super block at block %i \n file descriptor count %i \n", superBlock.blockNum, fileDesCount);

	//debugging
	//printFat();



return 0;
} // end initBootSector
/******************************************************************************/
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
/******************************************************************************/
/* This function will create a new file with the name as input
    and with the parent set as the current working directoty. we do not give it a block yet.*/
  int fs_create(char *name)
  {
    // make a temp file to name and add to FAT
    File tmpFile  ;

    //check if we have any file descriptors left to use
    if (fileDesCount < MAX_OPEN_FILE)
        {
            // assign file initial values
            tmpFile.filedes = -1 ; // it has not been opened yet
            tmpFile.FileSize = 0;
            strncpy(tmpFile.filename, name, sizeof(tmpFile.filename) - 1);
            tmpFile.filePointer = 0;
            tmpFile.isDir = 1 ; // this is not a directory
            strncpy(tmpFile.parent, "/", sizeof(tmpFile.parent) - 1);   // parent is whatever the cwd is when made
            tmpFile.startingAddr = 0;

            // add new file to FAT
            fat.FAT.push_back(tmpFile);
            printFat();

        }
        else
         {  // we have no open file descriptors return -1
             printf("no open file descriptors please close a file \n");
             return -1;
         }

         printf("created new file %s \n", name);
    return 0;
  } // end fs_create
/******************************************************************************/
/******************************************************************************/
/*
In fs_delete we only are required for this project to delete an empty file
this is easy because while a file is empty it only exists in the FAT
so all we need to do is remove its entry from the FAT
*/
  int fs_delete(char *name)
  {
      // look for file
      // first we check if file exists
     for (int i =0 ; i < fat.FAT.size(); i ++)
        {

           if (strcmp(fat.FAT.at(i).filename, name)== 0  )
            {
                                                              // if we find a match then create an iterator to point to the index to delete
                fat.FAT.erase(fat.FAT.begin() + i);           // we need to do it this way so erase works
                printf("deleted %s successfully\n", name);    // pass the iterator to the erase function to remove the file from FAT
                return 0;
            }

      }


            // return 1 if no match found
            printf("could not find file : %s \n", name);
            return 1;
  } // end fs_delete
/******************************************************************************/
/******************************************************************************/
  int fs_mkdir(char *name);
/******************************************************************************/
/******************************************************************************/

  int fs_read(int fildes, void *buf, size_t nbyte)
              {
   char * tmp;
   tmp = (char *)malloc(1 * sizeof(char));

   //check if file exists
   for (int i =0 ; i < fat.FAT.size(); i ++){

   if (fat.FAT.at(i).filedes == fildes )
        {

            printf("found file with descriptor %i \n", fildes);

//            //return try and read
          if (block_read(0 , (char*)tmp) > 0)
                {
                     printf("\n\n date from read: %p \n",tmp);
                    return 0;
                }
                else
                {
                  printf("\n\n problem with reading file %i \n",fildes);
                }

        } // end if

  } // end for
  printf("file descriptor count %i \n", fileDesCount);
  printf("failed read \n");
  return -1;
              }
/******************************************************************************/
/******************************************************************************/
  int fs_write(int fildes, void *buf, size_t nbyte);
/******************************************************************************/
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
/******************************************************************************/
  int fs_lseek(int fildes, off_t offset);
/******************************************************************************/
/******************************************************************************/
  int fs_truncate(int fildes, off_t length);
/******************************************************************************/
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
/******************************************************************************/

//testing function
int printFat()
{

  printf("\n\nsize of FAT %i \n\n ", fat.FAT.size());
  printf("used blocks : %i \n" , fat.UnusedBlocks);
for (int i =0 ; i < fat.FAT.size() ; i ++)
 {


     printf("element in FAT: %i \n  ", i );
     printf("***********************\n  " );
     printf("FILE NAME: %s \n  ",fat.FAT.at(i).filename );
     printf("FILE SIZE: %d \n  ",fat.FAT.at(i).FileSize );
     printf("FILE DES: %i \n  ",fat.FAT.at(i).filedes );
     printf("FILE START ADDR: %i \n  ",fat.FAT.at(i).startingAddr );
     printf("FILE PARENT: %i \n\n\n"  ,fat.FAT.at(i).parent );

    // if we have any blocks
    if (fat.FAT.at(i).blockList.size()> 0)
     printf("FILE BlOCK NUMBERS: %i \n\n\n",fat.FAT.at(i).blockList.at(0).blockNum );

  } // end for]
  return 0;
}

/******************************************************************************/
/******************************************************************************/
/* This function will iterate through the FAT and find all the files,
  then for each file it will append all the meta data to one long string.
  this string will then be written to the first block in the disk
*/
int writeMetaData(){

string tmp;

// iterate through FAT to find all files in system
for (int i =0 ; i < fat.FAT.size(); i ++)
  {
//	// then for each file in the FAT we want to append the meta data to a string
//	 make sure to convert each int to a string when appropriate

tmp += to_string(fat.FAT.at(i).FileSize);
tmp += " ";
tmp += (string)fat.FAT.at(i).filename;
tmp += " ";
tmp += to_string(fat.FAT.at(i).filePointer);
tmp += " ";
tmp += to_string(fat.FAT.at(i).filedes);
tmp += " ";
tmp += to_string(fat.FAT.at(i).startingAddr);
tmp += " ";
tmp += to_string(fat.FAT.at(i).isDir);
tmp += " ";
tmp += (string)fat.FAT.at(i).parent;
tmp += " ";
 if (fat.FAT.at(i).blockList.size()> 0)
tmp += to_string(fat.FAT.at(i).blockList.at(0).blockNum);
tmp += " ";
  // blocks where file is store for now just one we will put a loop here which add them all later
tmp += "\n";

  }
cout << "writing FAT to disk : "<< tmp << endl;
// once we run through FAT write to super-block
//but lets check first if we actually wrote anything
if (!tmp.empty())
 {

 block_write(0,(char *)tmp.c_str());
 printf("successfully wrote FAT to super block \n");
 return 0;
 }
// return 1 if nothing was written
else
 {
   printf("nothing in FAT to write to superblock \n");
   return 1;
 }

}// end writeMetaData


/******************************************************************************/
/******************************************************************************/
/*
This function will read in the data in the 0th block where we keep the meta data
for the fat and its associated files.
I will use strok to tokenize the string in to lines, then from that break it down
in to words to put back in to the FAT
this will be called in the mount function
*/
int parseMetaData(char * data)
{

// temp variable for holding data
//array of string to hold lines of data, where each line is a file


// read in data string from block 0 using block read

 // use strok and write each element out as a new element in the array of file data

// then for each element in the array of strings we will use strok to break them down and
// assign each value to the associated FAT value



return 0;
} // end parseMetaData
