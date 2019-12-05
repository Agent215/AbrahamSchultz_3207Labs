# Abraham Schultz

## Virtual Disk
- A virtual disk built on top of the linux file system using a single file
- this is to be implmented using a FAT data structure

The professor provided a disk.c file and a disk.h file. These files use system calls to do disk I/O, and form the basis for the rest of my functions.

Although I did not successfully complete every requirement for this project, I do believe I learned a lot and came close to finishing.
Below I have outlined where I met, and did not meet the specifications for this lab.

## Files included
- makefile  // type make to build program
- disk.cpp // functions to read and write to virtual disk
- header.h  // my header file
- disk.h   // header file for program supplied by proffesor
- main.cpp
- functions.cpp // various utility functions. and disk mount and unmount.
- testDisk // a virtual disk for testing during development
## Design
The main function in this program will run a shell allowing the user to
create a virtual disk, with a file system inside.This disk will be actually a
single file created on the Linux file system. The shell will accept commands to write to memory locations
within the file so that we can add files then read from them later. I will keep track of what file is located 
at each location in memory by using the following data structures:

- FAT     // this is a struct containing a list of File structs and block count int
- FILE     // a struct that contains meta data about file instance
- BLOCK   // this will describe meta data of the blocks

Additionally, the user should be able to use the shell to display the file hierarchy of the file system to the terminal. Also, we should
be able to navigate to at least one other directory and view its contents. The files for this project are only required to be text files. So, we should be able to print the contents of the file to the screen also. This is acomplished by having a current working directory variable that starts at the root. When we hit cd, we just check to make sure there is a file, that has the directory flag flipped with the matching name. then we change the cwd to that name. displaying the files works in the same way, except we search the fat for any files whos parent variable matches the cwd, then print to the screen.

when we mount a disk initially, we will create new FAT with no files. If we mount an existing disk, then we
must look to the starting super block where the meta data for the FAT is. We will then load the metadata in to our structs listed above.
On the flip side when we unmount the disk, we will need to write all the meta data for the FAT to the super block. This 
is accomplished by using a loop where we append the data for each file to one large string, then use block write to write this 
to the first block. There is then a function ParseMetaData, which will take the string and parse it in to its associated structs. In this way we can have the file structure and hierarchy of our file system be persistent.

I Still need to implement the following functionality:
The write file function will first check the size of the file and find as many empty blocks as needed. We then associtate these blocks with the coresponding file and write to them one at a time.
ls function will just go through FAT and find all files that parent matches current working directory and print them to screen. 
cd will update current working directory variable if path exists.

- refer to design document for entire list of available functions. 

## Testing
my main testing tools were the gdb debugger hex dump in the Linux shell. 
I also used printf to strategically print error codes 

I attempted to test my code in a modular fashion. 
almost every function has its own shell command to execute and test it.

I could not get my parseMetaData function to work correctly. so, this means that my mount function does not load the FAT meta data
back into memory. Reading the string into my parsing function was givign me some trouble. If I had been able to get that function to work then my disk would have achieved persistence. 

Furthermore, I did not get the write function to work. I intended to have a function find empty blocks and add them to the File block list. But this ended up taking longer than I anticipated. I did update my pseudo code for this and other functions in my design document.

within the design and testing document there are screenshots of various modules being tested.

## instructions to run
- type: make to build
- type:./fileSys to run



