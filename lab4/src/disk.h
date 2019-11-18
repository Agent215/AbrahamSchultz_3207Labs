#ifndef _DISK_H_
#define _DISK_H_

/******************************************************************************/
#define DISK_BLOCKS  16384      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

/******************************************************************************/
int make_disk(char *name);     /* create an empty, virtual disk file          */
int open_disk(char *name);     /* open a virtual disk (file)                  */
int close_disk();              /* close a previously opened disk (file)       */

int block_write(int block, char *buf);
                               /* write a block of size BLOCK_SIZE to disk    */
int block_read(int block, char *buf);
                               /* read a block of size BLOCK_SIZE from disk   */
/******************************************************************************/

//Struct FAT{
//	Int TotalBlocks = 16384
//	Int UnusedBlocks;  // start at total blocks and subtract from as we go 
//Vector <File > FAT // a linked list containing linked lists of blocks. }? // struct FAT The file struct will be a linked list of blocks 
//Struct File{
//	Vector <struct Block> blockList; // this will contain the block holding the data 
//Int FileSize = blocklist.length * blockSize;
//Int filePointer;   // pointer to current data byte 
//Int startingAddr; // address of first block used by this file 
//bool operator<(const startingAddr& rhs) const
//{ // i think we can use this to compare file locations prioritizing lower address 
//	return startingAddr> rhs.startingAddr;
//} } ?// end Struct File The Block struct will be an element in the  
//3
//Struct Block{
//	Char DATA[4096]  // these file will only hold text for now. 4096 bytes = 4KB 
//	Int addrNextBlock; // point to next block used -1 if not used } ?// end Struct Block // used to initialize super block 
//Struct BootSector{
//	Int totalBlocks   // 16383 
//	Int bytesPerBlock // 4096 
//	String DiskName
//	StartingBlocks    // blocks saved for initialization } ?// end struct BootSector // struct to keep track of directory hierarchies 
//	Struct DirectoryList() {
//	Int directCount;  // keep track of directories 
//	Struct File* directList  // actual list referencing files that are directories  
//}// end directoryList 
//
//



#endif
