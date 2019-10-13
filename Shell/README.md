# Abraham Schultz
## CreamShell
(Cache Reduces Extra Allotment of Memory)

- 2nd lab assignment for Temples 3207 Intro to OS class.
- This is my implementation of a Command line interpreter.



## Commands supported 

[] indicate arguments should be provided after typing in command
for example if i want to print out hello world i would type:
echo hello world

      - cd  [directory name to move to]
      - dir   // prints out files in current directory
      - echo  [args] // prints to screen whatever is given as input
      - envr   //prints environment variables
      - pause  // pauses until user hits enter
       - clr   // clear terminal
      - help   // displays help

## Features 
      - pipe handling                      // use the "|" symbol for piping
      - redirection                        // > for append to file>> for truncate file,  same for < and << symbols
      - batch file support                 // type ./creamShell followed by the batch file name
      
      - all other general commands available in UNIX shell

###  Files Included
CreamShell.cpp;
internalFunnction.cpp;
CreamShell.h;
batch.txt;
makefile;

### Instructions to run 
- To compile type : make
- to run type: ./creamShell
