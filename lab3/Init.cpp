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



using namespace std;

const char *DICTIONARY  = "dictionary.txt";
const int FILESIZE = 99171;
string diction[99171];   // used word count program to count lines of dictionary file

/*
  here i am testing a design for reading in a file to an array data structure
  for general use by the rest of the server program
  i am reusing some code from my first lab. this is from the config file.
*/
int main (){


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
                     // printf(" Loading Data from ConfigFile  %s \n",firstWord.c_str());

            } // end for


                 // loop for testing string matching
                 while (1){

                       int correct = 0;                                         // is 1 if correct
                       string buf;                                              //create a string buf to hold user input
                       cout << "hello please enter a word to check" << endl;    // get user input from console
                       getline( cin, buf);

                        if  (strcmp(buf.c_str(),"exit")==0)                     // check to exit
                              {
                                return 0;
                                break;
                              } // end if

                           for (int k = 0; k < FILESIZE; k++)                      // check against dictionary array
                            {

                                 if (strcmp(buf.c_str(),diction[k].c_str())==0)
                                     {                                            //compare against each word in array
                                        cout << "OK " <<  buf.c_str() <<endl;
                                        correct =1;
                                        break;
                                     } // end if

                            } // end for
                                if (correct == 0)
                               cout << " MISSPELLED " << buf.c_str() <<endl;
                  } // end while

   } // end if

return 0;
} // end main
