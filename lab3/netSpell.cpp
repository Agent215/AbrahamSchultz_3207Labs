/*
netSpell.cpp
Abraham Schultz

last edited 10/16/2019

This is the main file for the networked spell checker program
which is the third lab as part of temple university 3207 OS class

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include "netSpell.h"


// main function for program
int main (){
string* diction = new string [99171];

diction = loadDiction();

                 // loop for testing string matching
                 while (1){

                       string buf;                                                //create a string buf to hold user input
                       cout << "hello please enter a word to check" << endl;      // get user input from console
                       getline( cin, buf);

                        if  (strcmp(buf.c_str(),"exit")==0)                       // check to exit
                              {
                                return 0;
                                break;
                              } // end if

                              checkSpell(buf,diction );                          // call checkSpell function

                  } // end while




} // end main
