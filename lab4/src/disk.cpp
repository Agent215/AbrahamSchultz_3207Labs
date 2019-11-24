/*
disk.cpp

Abraham Schultz
11/24/2019

most of this code was provided by professor Eugene Kwatney for use
with the virtual file system project.

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#include "disk.h"
#include "header.h"

/******************************************************************************/
static int active = 0;  /* is the virtual disk open (active) */
static int handle;      /* file handle to virtual disk       */

struct FAT fat;
struct File root_dir;
/******************************************************************************/
int make_disk(char *name)
{
  int f, cnt;
  char buf[BLOCK_SIZE];

  if (!name) {
    fprintf(stderr, "make_disk: invalid file name\n");
    return -1;
  }

  if ((f = open(name, O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0) {
    perror("make_disk: cannot open file");
    return -1;
  }

  memset(buf, 0, BLOCK_SIZE);
  for (cnt = 0; cnt < DISK_BLOCKS; ++cnt)
    write(f, buf, BLOCK_SIZE);

  close(f);

  return 0;
}
/******************************************************************************/
int open_disk(char *name)
{
  int f;

  if (!name) {
    fprintf(stderr, "open_disk: invalid file name\n");
    return -1;
  }

  if (active) {
    fprintf(stderr, "open_disk: disk is already open\n");
    return -1;
  }

  if ((f = open(name, O_RDWR, 0644)) < 0) {
    perror("open_disk: cannot open file");
    return -1;
  }

  handle = f;
  active = 1;

  return 0;
}
/******************************************************************************/
int close_disk()
{
  if (!active) {
    fprintf(stderr, "close_disk: no open disk\n");
    return -1;
  }

  close(handle);

  active = handle = 0;

  return 0;
}
/******************************************************************************/
int block_write(int block, char *buf)
{
  if (!active) {
    fprintf(stderr, "block_write: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_write: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_write: failed to lseek");
    return -1;
  }

  if (write(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_write: failed to write");
    return -1;
  }

  return 0;
}
/******************************************************************************/
int block_read(int block, char *buf)
{
  if (!active) {
    fprintf(stderr, "block_read: disk not active\n");
    return -1;
  }

  if ((block < 0) || (block >= DISK_BLOCKS)) {
    fprintf(stderr, "block_read: block index out of bounds\n");
    return -1;
  }

  if (lseek(handle, block * BLOCK_SIZE, SEEK_SET) < 0) {
    perror("block_read: failed to lseek");
    return -1;
  }

  if (read(handle, buf, BLOCK_SIZE) < 0) {
    perror("block_read: failed to read");
    return -1;
  }

  return 0;
}
/******************************************************************************/

/*
function to initialize the first block of memory on disk with FAT
and also to add the root directory
*/
int initBootSector (){


   lseek(handle,  0, SEEK_SET);
   int ptr = 0;
   fat = {};
   root_dir = {};
   struct Block superBlock;

  fat.TotalBlocks =BLOCK_SIZE ;
  fat.UnusedBlocks = BLOCK_SIZE;
  root_dir.FileSize = 32;           // allocate 32 bytes to root directory
  root_dir.isDir = 0; // is dir
  root_dir.startingAddr =0;
  root_dir.filePointer = 0;
  superBlock.isUsed =1;
  superBlock.blockNum = 1;
  root_dir.blockList.push_back (superBlock);    // add super block to root dir meta data
  fat.UnusedBlocks = BLOCK_SIZE -1;
  fat.FAT.push_back(root_dir); // add root dir to fat table


  // write to disk
   block_write(superBlock.blockNum,superBlock.DATA);
   lseek(handle, 2 * BLOCK_SIZE, SEEK_SET);

return 0;
} // end initBootSector
/******************************************************************************/
