/*
disk.h

Abraham Schultz
11/24/2019

this code was provided by professor Eugene Kwatney for use
with the virtual file system project.

*/

#include <vector>
#include <stdlib.h>
#include <string.h>
#include <string>

#ifndef _DISK_H_
#define _DISK_H_

/******************************************************************************/
#define DISK_BLOCKS  16384      /* number of blocks on the disk                */
#define BLOCK_SIZE   4096      /* block size on "disk"                        */

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
///******************************************************************************/

#endif
