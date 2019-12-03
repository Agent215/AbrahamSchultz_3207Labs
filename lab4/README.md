# Abraham Schultz

## Virtual Disk
- A virtual disk built on top of the linux file system using a single file
- this is to be implmented using a FAT data structure

## Files included
- makefile  // type make to build program
- disk.cpp // functions to read and write to virtual disk
- header.h  // my header file
- disk.h   // header file for program supplied by proffesor
- main.cpp  
- testDisk // a virtual disk for testing during development
## Design and testing
The main function in this program will run a shell allowing the user to
create a virtual disk, with a file system inside. This disk will be actually
single file created on the Linux file system. The shell will accept commands to write to memory locations
within the file so that we can add files then read from them later. I will keep track of what file is located 
at each location in memory by using the following data structures:

- FAT    // will keep track of all files, and number of used block in memory
- FILE   // will keep all meta data about a file object
- BLOCK  // this will describe meta data of the blocks

when we mount a disk initially, we will create new FAT with no files. If we mount an existing disk, then we
must look to the starting super block where the meta data for the FAT is. We will then load the metadata in to our structs listed above.
On the flip side when we unmount the disk, we will need to write all the meta data for the FAT to the super block. This 
is accomplished by using a loop where we append the data for each file to one large string, then use block write to write this 
to the first block. There is then a function ParseMetaData, which will take the string and parse it in to its associated structs. In this way we can have the file structure and hierarchy of our file system be persistent.

I Still need to implement the following functionality:
The create file function will first check the size of the file and find as many empty blocks as needed. We then create a new file with the correct meta data and add the addresses for needed empty blocks. 
The mkdir function should create a file but with the flag for is directory true.
ls function will just go through FAT and find all files that parent matches current working directory. 
cd will update current working directory variable if path exists.
Additionally, the user should be able to use the shell to display the file hierarchy of the file system to the terminal. Also, we should
be able to navigate to at least one other directory and view its contents. The files for this project are only required to be text files. So, we should be able to print the contents of the file to the screen also.

- refer to design document for entire list of available functions. 
## instructions to run
- type: make to build
- type:./fileSys to run



