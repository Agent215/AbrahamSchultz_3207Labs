#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "disk.h"

int main (int argc , char** argv){

	char * DiskName = "testDisk";

	//int testDisk = make_disk(DiskName);
	int error = open_disk(DiskName);



//
//	intitlize();
//	While(running == true) {
//		4
//			Char * buff // hold user input 
//			Getline();
//		Big switch case
//			Switch()
//			Case  open()
//			….
//			Rest of the functions listed below will each be there own case
//			Default
//			Case unrecognized command try again
//	}
//} // end while 


return 0;
}// end main


/*
**************************************************************************************************************************
**************************************************************************************************************************
												FUNCTIONS 
*/

//?   int make_fs(char *disk_name); { MakeDisk(disk_name)
////create new FAT instance  Struct FAT = new FAT(); FAT.totalBlocks = TOTALBLOCKS Struct FILE rootDir;   // create root directory on  lowest memory blocks } / ? / end make_fs       int mount_fs(char *disk_name); { // check if disk exists
//If it exists
//Open disk (disk_name)
//Else
//Error message
//// we need to make the selected virtual disk available to edit and read and write from
//5
//} / ? / end mount      int umount_fs(char *disk_name); { Closedisk (disk_name) } / ? / end unmount    ?   int fs_open(char *name);{ For (each item in FAT) { If FAT(i).name = name{ // open file Return 1; } } Return -1 // only here if no file found }    ?   int fs_close(int fildes);       exist or is not open, the function returns -1.     ?  int fs_create(char *name){ File tmpFile = File; File.name = name; //first check if we have space If (FAT unusedBlocks > 0) { FAT.insert(name)
//
//} // else we have no space printf(“not enough space on disk \n”) };       int fs_delete(char *name);{ //check if file exists
//6
//For (each item in FAT) { If FAT(i).name = name{ FAT(i). Return 1; } } Return -1 // only here if no file found
//
//}       int fs_mkdir(char *name);{ }/ ? / end fs_mkdir       int fs_read(int fildes, void *buf, size_t nbyte);{ // wrapper function for one provided by professor }       int fs_write(int fildes, void *buf, size_t nbyte);{ // wrapper function for one provided by professor }
//
//int fs_get_filesize(int fildes); { For (each element int FAT) { // check if file exists If (FAT(i)  = fildes ){ Return FAT(i).fileSize; } // end if } // end for } ? // end fs_get_filesize       int fs_lseek(int fildes, off_t offset);{ } ? //end fs_lseek
//7
//int fs_truncate(int fildes, off_t length);{ // ? check if file exists // if file exists then check how big it its // if it is bigger than the length we must free the extra blocks // if file does not exist then return error message and -1 } ? //end fs_truncate
//