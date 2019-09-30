/*
RandomGen.cpp

Abraham Schultz
Last Edited 9/5/2019

This is a file for a random number generator set of functions.
These will be used in a discrete event simulation for simulating
random times that the CPU and disks are in use.

At least one of these functions will take a Seed to start off the randomization

*/
#include "Des.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <time.h>
using namespace std;


//returns a number between min and max
int randomRange(int min, int max){
// number to return;
int randNum;

randNum = (rand() % (max + min));
//return random number from function;
return randNum;

} // end randomRange

//***********************************************************************************
//***********************************************************************************

// wrapper function for srand class, will be used to determine process quit prob, and disk prob
float randomFloat(){

float returnFloat;

returnFloat = rand()/(double(RAND_MAX));

return returnFloat;
} // end randomFloat

//***********************************************************************************
//***********************************************************************************
// function to return random bool takes a float which should represent proability
int randomBool(float prob) {

// we return this
int returnValue;
float tmp;

tmp = randomFloat();
 //printf ("random float :%f \n", tmp );

        // if we are less than prob then true
    if (tmp <  prob )
    {
          returnValue = 0;
          return  returnValue;
    }
    else {
            // else return false
          returnValue = 1;
          return  returnValue;

    }

};

int initRandom(int seed){
int tmp = seed;
srand(seed);
return 0;
}
