/*
Init.cpp
Abraham Schultz
10/16/2019

This should be file that contains the function to initialize our general data structures for
our networked spell check program

for now it just a test script.
/*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "netSpell.h"


using namespace std;

const char *DICTIONARY  = "dictionary.txt";
string* diction = new string [99171];
/*
  here i am testing a design for reading in a file to an array data structure
  for general use by the rest of the server program
  i am reusing some code from my first lab. this is from the config file.
*/
//***************************************************************************************************************
string* loadDiction(){


  int counter = 0;
  string line;                                 // read file one line at a time
  ifstream myfile (DICTIONARY);                //grab file
  if (myfile.is_open())
   {
              // split each line
            while ( getline (myfile,line) )
            {
                     diction[counter] = line;
                     counter++;
            } // end while

              for (int j = 0; j < FILESIZE; j++)
            {
                     // split words
                     string firstWord = diction[j].substr(0, diction[j].find(" "));
                     diction[j]=  firstWord;
            } // end for


   } // end if

return diction;
} // end main

//***************************************************************************************************************




