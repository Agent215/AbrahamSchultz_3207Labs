/*
header.h

Abraham Schultz
11/24/2019

contains structs and function declarations  for file system implementation over a virtual disk.
*/

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>
#include <queue>

#ifndef _HEADER_H_
#define _HEADER_H_

using namespace std;

/********************* FUNCTION DECLARATIONS *************************************/
int make_fs(char *virtualDisk);
///* initialize the disk  */
int initBootSector();         /* initialize the boot sector, which is the first memory blocks on disk  */
int mount_fs(char *disk_name);
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
/************************* STRUCTS ***************************************************/

struct Block {
	char DATA[BLOCK_SIZE]; // these file will only hold text for now. 4096 bytes = 4KB
	int addrNextBlock;      // point to next block used -1 if not used }
	int isUsed;             // 0 if free 1 if used
	int blockNum;

}; // end Block
/******************************************************************************/
struct File {
	vector <Block> blockList;   // this will contain the blocks holding the data
	char * filename;            // name of file
	int FileSize;              //  = blockList.size * BLOCK_SIZE;
	int filePointer;            // pointer to current data byte
	int filedes;                // file descriptor , -1 if closed should always be closed after unmount
	int startingAddr;           // address of first block used by this file
	int isDir;                  // flag 0 if dir 1 if regular file
	char * parent;              // parent of file or directory none if root.
								// we will compare values using starting address
//	bool operator<(const File& rhs) const
//	{
//		return startingAddr > rhs.startingAddr;
//	}
}; // end File
/******************************************************************************/
   // a Struct definition for FAT
   // the fat contains a vector with each file
struct FAT {
	int TotalBlocks;
	int UnusedBlocks;  // start at total blocks and subtract from as we go
	vector <File > FAT; // a linked list containing linked lists of blocks.

}; // end FAT

#endif
