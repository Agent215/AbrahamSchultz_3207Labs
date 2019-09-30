/*
DesDriver.cpp

Abraham Schultz

Last Edited 9/4/2019

This is the driver program for the
 Discrete event simulation.

 This contains the main method and main while loop;
 We will call upon outside utility functions and event handlers here.

before the while loop begins we will load the config file and set the
parameters.

The while loop should have at least 5 stages per iteration
1.set time
2.pop of next event of event queue
3.handle event
4.update cpu
5.update disks

*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <bits/stdc++.h>
#include "Des.h"

using namespace std;


     /////QUEUES///////////////
     priority_queue <Event> eq;            // Creates a priority queue
     queue <Event> CPUQueue;               //create CPU FIFO queue
     queue <Event> Disk_1_Queue;           // create DISK 1 FIFO queue
     queue <Event> Disk_2_Queue;           // create DISK 2 FIFO queue

    // current time in simulation
     int currentTime ;

     //  IDLE FLAGS
     int CPU_IDLE;
     int DISK1_IDLE;
     int DISK2_IDLE;
     int RUNNING;


     int eqMax;             // highest number of elements in event queue
     int CpuMax;            // highest number of elements in CPU queue
     int Disk1Max;          // highest number of elements in Disk1 queue
     int Disk2Max;          // highest number of elements in Disk2 queue
     int eqTotal;           // to calculate avg # of elements in queue
     int CpuTotal;          // to calculate avg # of elements in queue
     int Disk1Total;        // to calculate avg # of elements in queue
     int Disk2Total;         // to calculate avg # of elements in queue


     FILE * OutTXT;                //log file
     FILE * StatTXT;               //stat file

//**********************************************************************************************************************
//**********************************************************************************************************************

 int main(){

// set flags for modules to all be idle on start
CPU_IDLE = 0;
DISK1_IDLE = 0;
DISK2_IDLE = 0;
RUNNING = 0;
// initialize counters
eqMax = 0;
CpuMax = 0;
Disk1Max= 0;
Disk2Max = 0;
eqTotal = 0;
CpuTotal = 0;
Disk1Total = 0;
Disk2Total = 0;

// naming out put files
char logFile[] = "log.txt";
char statFile[] = "Stat.txt";


//create array for parsing config file
string* InitData = new string [1000];

    // delete old log.txt file every time we run
	//Deletes out.txt file if exists
	if (remove(logFile) != 0)
		perror("log File deletion failed");
	else
		cout << "log File deleted successfully" << endl;

    // delete old Stat.txt file every time we run
    //Deletes out.txt file if exists
	if (remove(statFile) != 0)
		perror("Stat File deletion failed");
	else
		cout << "Stat File deleted successfully" << endl;

// make new log file
OutTXT = fopen ("log.txt","a");
// make new stat file
StatTXT = fopen ("Stat.txt","a");
// initialize array from config file
InitData = InitConfig(OutTXT);
// get starting time from config file
int time =  getINIT_TIME();
// initialize random seed
initRandom(getSEED());
       // pushing to priority queue first and last events
       Event job0 = createProcess(0,1,PROCESS_ARRIVAL);
       Event EndSimEvent = createProcess(2100,getFIN_TIME(),SIMULATION_FINISHED);
       Event thirdProc = createProcess(2,3,PROCESS_ARRIVAL);
       eq.push(job0);                                // add first job
       eq.push(thirdProc);                           // add third job for good measure
       eq.push(EndSimEvent);			             // add last job

      // start counter at 3 because we already have a job 2 ^^^
      int counter = 3;

//**********************************************************************************************************************
//**************** MAIN WHILE LOOP*****************************************************************
//**********************************************************************************************************************

// while queue is empty and last event has not been hit
while(RUNNING == 0){

 // set time each iteration through the main loop
 currentTime = eq.top().Time;

 // find max size of each queue
 if (eq.size() >= eqMax) {  eqMax = eq.size(); cout << eq.size()<< endl;}
 if (CPUQueue.size() >= CpuMax) {  CpuMax = CPUQueue.size();}
 if (Disk_1_Queue.size() >= Disk1Max) { Disk1Max = Disk_1_Queue.size();}
 if (Disk_2_Queue.size() >= Disk2Max){    Disk2Max = Disk_2_Queue.size();}

 // get totals for avg calculation
 eqTotal += eq.size();
 CpuTotal += CPUQueue.size();
 Disk1Total += Disk_1_Queue.size();
 Disk2Total += Disk_2_Queue.size();
 //PrintEQ (eq);


switch(eq.top().EventType){

            case PROCESS_ARRIVAL:{
                         //CASE PROCESS ARIVAL
                         HandleArrival(counter, eq, CPUQueue, currentTime,OutTXT,StatTXT);
                         counter++;
            } // end case 0
             break;
            // CPU ENTER
            case CPU_ENTER: {

                         eq.pop();
                         counter++;
            } // end case 1
             break;
                    //CPU FINISH
            case CPU_FINISHED: {

                        CPU_IDLE = 0;
                        handleCPUfinish(counter,eq,Disk_1_Queue,Disk_2_Queue,CPU_IDLE, currentTime,OutTXT);
                        counter++;

            }
             break;
					//PROCESS EXIT
            case PROCESS_EXIT:{

                       printf("TIME:%i JOB:%i EXITED\n", currentTime, eq.top().procID);
                       fprintf(OutTXT,"TIME:%i JOB:%i EXITED\n", currentTime, eq.top().procID);
                     //  myfile << "TIME:" << currentTime<< " JOB:"<< eq.top().procID << " EXITED\n";
                       eq.pop();
                       counter++;
            }

            break;
            // DISK 1 ARRIVAL
            case DISK1_START:{

                      eq.pop();
                      counter++;
            }
            break;
            //DISK 2 ARRIVAL
            case DISK2_START: {

                     eq.pop();
                     counter++;
            }
            break;
            //DISK 1 FINISH
            case DISK1_FINISHED:
             {

                     DISK1_IDLE = 0;
                     handleDISKfinish(eq,currentTime,OutTXT);
                     counter++;

             }
            break    ;
            //DISK 2 FINISH
            case DISK2_FINISHED:
               {

                    DISK2_IDLE = 0;
                    handleDISKfinish(eq,currentTime,OutTXT);
                    counter++;
                    //handle_disk_finish();
               }
            break    ;
            // SIMULATION END

            case SIMULATION_FINISHED:
            {
                    RUNNING = 1;
					printf("TIME:%i JOB:%i SIMULATION END\n", currentTime, eq.top().procID);
                    fprintf(OutTXT,"TIME:%i JOB:%i SIMULATION END\n", currentTime, eq.top().procID);

                    //print stats before finishing
                    PrintStats(getFIN_TIME(),getINIT_TIME());
                    fclose(OutTXT);
                    fclose(StatTXT);
                    return 0;
                    counter++;
            }
            break    ;
            default:
            break;


} // end switch


//    UPDATE CPU AND DISK
   Update_CPU(eq,CPUQueue,CPU_IDLE,currentTime,OutTXT);
   UPDATE_DISK (eq,Disk_1_Queue,Disk_2_Queue,DISK2_IDLE,DISK1_IDLE,currentTime,OutTXT);

}//  end while


 fclose(OutTXT);
 fclose(StatTXT);
 return 0;
 } // end main

//**********************************************************************************************************************
//**********************************************************************************************************************
// function to use in testing to print event queue
// this function copy's event queue and prints out the copy
int PrintEQ (priority_queue<Event> q){

 Event Tmp;

while (!q.empty())
{
        // check top
     Tmp = q.top();
     printf ("EQ  ID:%i TIME:%i EVENT: %i \n",Tmp.procID ,Tmp.Time ,Tmp.EventType);

     q.pop();


} // end while

return 0;
} // end Print EQ
//**********************************************************************************************************************
//**********************************************************************************************************************
// function to call to print out stat file
int PrintStats(int finTime,int init)
{

  int  num = init;
  fprintf(StatTXT,"STATISTICS FILE\n\n************************************\n************************************\n");
  fprintf(StatTXT,"QUEUE SIZES \n************************************\n");
  fprintf(StatTXT,"Event Queue maximum size: %i \n",eqMax);
  fprintf(StatTXT,"Event Queue Average size: %i \n\n",eqTotal/getFIN_TIME());
  fprintf(StatTXT,"CPU Queue maximum size: %i \n",CpuMax);
  fprintf(StatTXT,"CPU Queue Average size: %i \n\n",CpuTotal/getFIN_TIME());
  fprintf(StatTXT,"DISK1 Queue maximum size: %i \n",Disk1Max);
  fprintf(StatTXT,"DISK1 Queue Average size: %i \n\n",Disk1Total/getFIN_TIME());
  fprintf(StatTXT,"DISK2 Queue maximum size: %i \n",Disk2Max);
  fprintf(StatTXT,"DISK2 Queue Average size: %i \n\n",Disk2Total/getFIN_TIME());
  fprintf(StatTXT,"\n************************************\n************************************\n");
  fprintf(StatTXT,"UTILIZATION\n************************************\nTOTAL SIMULATION RUN TIME: %i\n\n", finTime - getINIT_TIME());

printUtil(StatTXT,12000, num);

return 0;

};
