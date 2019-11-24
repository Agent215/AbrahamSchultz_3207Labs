/*
disk.h

Abraham Schultz
11/24/2019

much of this code was provided by professor Eugene Kwatney for use
with the virtual file system project.

*/

#include <vector>
#include <stdlib.h>

#ifndef _DISK_H_
#define _DISK_H_

/******************************************************************************/
#define DISK_BLOCKS  16384      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

using namespace std;

/******************************************************************************/
/******************************************************************************/
/*                           FUNCTION DECLERATIONS                            */

int make_disk(char *name);     /* create an empty, virtual disk file          */
int open_disk(char *name);     /* open a virtual disk (file)                  */
int close_disk();              /* close a previously opened disk (file)       */

int block_write(int block, char *buf);
                               /* write a block of size BLOCK_SIZE to disk    */
int block_read(int block, char *buf);
                               /* read a block of size BLOCK_SIZE from disk   */
/******************************************************************************/


 struct Block{
	char DATA[BLOCK_SIZE] ; // these file will only hold text for now. 4096 bytes = 4KB
	int addrNextBlock;      // point to next block used -1 if not used }

}; // end Block


//// /******************************************************************************/
struct File{
	vector <Block> blockList;   // this will contain the block holding the data
    int FileSize ;              //  = blockList.size * BLOCK_SIZE;
    int filePointer;            // pointer to current data byte
    int startingAddr;           // address of first block used by this file
}; // end File



// a Struct definition for FAT
// the fat contains a vector with each file
struct FAT{
	int TotalBlocks;
	int UnusedBlocks;  // start at total blocks and subtract from as we go
    vector <File > FAT; // a linked list containing linked lists of blocks. }

 } ; // end FAT

/////******************************************************************************/
////

///******************************************************************************/
//Struct BootSector{
//	Int totalBlocks   // 16383
//	Int bytesPerBlock // 4096
//	String DiskName
//	StartingBlocks    // blocks saved for initialization }
// end struct BootSector // struct to keep track of directory hierarchies



#endif
