/* InitilizeConfig.cpp
..
Abraham Schultz

Last Edited 8/26/2019

function to read in config.txt file

*/
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include "Des.h"


using namespace std;

// array to store data from config file
string* InitData = new string [1000];

  int numberOfConfig = 12;
  int SEED ;
  int INIT_TIME;
  int FIN_TIME;
  int ARRIVE_MIN;
  int ARRIVE_MAX;
  float QUIT_PROB;
  int CPU_MIN;
  int CPU_MAX;
  int DISK1_MIN;
  int DISK1_MAX;
  int DISK2_MIN;
  int DISK2_MAX;


 string *InitConfig(FILE * OutTXT){




  int counter = 0;                             //to count the data elements, start at 1 because we ignore the first element
  string line;                                 // read file one line at a time
  ifstream myfile ("config.txt");              //grab file
  if (myfile.is_open())
  {
      // split each line
    while ( getline (myfile,line) )
    {
             InitData[counter] = line;
             counter++;
    } // end while


    for (int j = 0; j < numberOfConfig; j++)
    {
            // split first words
            string firstWord = InitData[j].substr(0, InitData[j].find(" "));
           //position of blank space
            size_t whitespace = InitData[j].find(" ");
            // second word
            string secondWord =  InitData[j].substr(whitespace);
            //make array of  second  values
            InitData[j]=  secondWord;
            printf(" Loading Data from ConfigFile %s %s \n",firstWord.c_str(),secondWord.c_str());
            fprintf(OutTXT,"Loading Data from ConfigFile %s %s \n", firstWord.c_str(),secondWord.c_str());

    } // end for
 cout << endl<< endl;
 fprintf(OutTXT,"\n\n");

    // convert strings to int
    SEED =  atoi (InitData[0].c_str());
    INIT_TIME =  atoi (InitData[1].c_str());
    FIN_TIME = atoi(InitData[2].c_str());
    ARRIVE_MIN = atoi(InitData[3].c_str());
    ARRIVE_MAX = atoi(InitData[4].c_str());
    QUIT_PROB = atof (InitData[5].c_str());    // except this one! this is a float
    CPU_MIN = atoi (InitData[6].c_str());
    CPU_MAX = atoi (InitData[7].c_str());
    DISK1_MIN = atoi (InitData[8].c_str());
    DISK1_MAX = atoi (InitData[9].c_str());
    DISK2_MIN = atoi (InitData[10].c_str());
    DISK2_MAX = atoi (InitData[11].c_str());




    myfile.close();
  } // end if

  else cout << "Unable to open file";





  return InitData;
}

// *****************GETTERS ******************************************************************

int getSEED(){return SEED;};
int getINIT_TIME(){return INIT_TIME;};
int getFIN_TIME(){return FIN_TIME;};
int getARRIVE_MIN(){return ARRIVE_MIN;};
int getARRIVE_MAX(){return ARRIVE_MAX;};
float getQUIT_PROB(){return QUIT_PROB;};
int getCPU_MIN(){return CPU_MIN;};
int getCPU_MAX(){return CPU_MAX;};
int getDISK1_MIN(){return DISK1_MIN;};
int getDISK1_MAX(){return DISK1_MAX;};
int getDISK2_MIN(){return DISK2_MIN;};
int getDISK2_MAX(){return DISK2_MAX;};
