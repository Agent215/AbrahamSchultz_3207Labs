/*
Des.h

Abraham Schultz

Last Edited 8/26/2019

This is the header file for assignment number one from Temple universities
3207 course on operating systems.

The program should Implement a Discrete event simulation.


*/

#include <stdlib.h>
#include <string>
#include <queue>

#ifndef DES_H_
#define DES_H_



using namespace std;

/*Declare EVENT enumeration*/
int const PROCESS_ARRIVAL = 0;
int const CPU_ENTER = 1;
int const CPU_FINISHED = 2;
int const PROCESS_EXIT = 3;
int const DISK1_START = 4;
int const DISK2_START = 5;
int const DISK1_FINISHED = 6;
int const DISK2_FINISHED = 7;
int const SIMULATION_FINISHED = 8;


/*
Declare struct to represent some process that the CPU will handle
*/
struct Event {

    int procID;   /* id of associated process*/

	int Time;   /* this is time that the event occurs */

	int EventType;       /* an int representing event type. */


    /*  here we create a function to make sure the priority queue knows to sort by time
        and to sort from lowest to highest
    */
	 bool operator<(const Event& rhs) const
    {
        return Time > rhs.Time;
    }

};

//**********************************************************************************************************
//***************function declarations**********************************************************************
//**********************************************************************************************************


// getters to get init values
int getSEED();
int getINIT_TIME();
int getFIN_TIME();
int getCPU_MIN();
int getCPU_MAX();
int getARRIVE_MIN();
int getARRIVE_MAX();
int getDISK1_MIN();
int getDISK1_MAX();
int getDISK2_MIN();
int getDISK2_MAX();
float getQUIT_PROB();
// event handlers
int Update_CPU(priority_queue<Event> &eq,queue <Event> &CPUQueue, int &CPU_IDLE, int currentTime,FILE * OutTXT);
int handleCPUfinish(int counter, priority_queue<Event> &eq,queue <Event> &Disk_1_Queue,queue <Event> &Disk_2_Queue, int &CPU_IDLE, int currentTime,FILE * OutTXT);
int HandleArrival(int counter, priority_queue<Event> &eq , queue <Event> &CPUQueue,int currentTime, FILE * OutTXT,FILE * StatTXT);
int UPDATE_DISK(priority_queue<Event> &eq,queue <Event> &Disk_1_Queue,queue <Event> &Disk_2_Queue, int &DISK2_IDLE,  int &DISK1_IDLE, int currentTime,FILE * OutTXT);
int handleDISKfinish(priority_queue<Event> &eq, int currentTime,FILE * OutTXT);
//UTILITIES
Event createProcess (int PID,int time, int event);      // general function to create a new process;
int PrintEQ (priority_queue<Event> q);                  // for testing and debugging
int randomBool(float prob);
string *InitConfig(FILE * OutTXT);
int randomRange(int min, int max);
float randomFloat();
int PrintStats(int finTime,int jobNum);                                       // for printing out stat file
int initRandom(int seed);
int printUtil(FILE * StatTXT, int finTime,int jobNum);
//getters for stats
int getCpuBusy();
int getDisk1Busy();
int getDisk2Busy();
#endif
