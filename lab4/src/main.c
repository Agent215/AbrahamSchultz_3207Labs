#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#include "disk.h"

int main (int argc , char** argv){

	char * DiskName = "testDisk";

	//int testDisk = make_disk(DiskName);
	int error = open_disk(DiskName);

return 0;
}// end main
