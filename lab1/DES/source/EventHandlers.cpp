/*
EventHandlers.cpp


Abraham Schultz

Last Edited 9/11/2019

This is the file containing the event handlers for the
discreet event simulation

This contains :
HandleArrival event
Update_CPU
handleCPUfinish
handleDISKfinish
UPDATE_DISK


*/

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <queue>
#include <string>
#include <bits/stdc++.h>
#include "Des.h"


using namespace std;

     int timeCPUbusy;       //counter to count time CPU is busy
     int timeDisk1busy ;     //counter to count time CPU is busy
     int timeDisk2busy ;     //counter to count time CPU is busy

     //keep track of jobs handled
     int CpuJobs;
     int Disk1Jobs;
     int Disk2Jobs;

     // keep track of how long jobs take
     int maxCpuTime=0;
     int maxDisk1Time=0;
     int maxdisk2Time=0;
     int jobCounter;

//*************************************************************************************************************************************************************************************
 //***************************** EVENT HANDLERS****************************************************************************************************************************************

 // HandleArrival pushes the next item of the event queue and in to the cpu queue
 // this event only is called if the top of the event queue is of type 0 = job arrive
int HandleArrival(int counter, priority_queue<Event> &eq , queue <Event> &CPUQueue ,int currentTime,FILE * OutTXT,FILE * StatTXT ){
   //OutTXT = fopen ("Out.txt","a");
    // create a new event for adding to cpu queue , with same PID as top of the eq queue, but with status now CPU ENTER
    Event tempEvent = createProcess(eq.top().procID,currentTime,CPU_ENTER);
    // pop off event queue
    eq.pop();
    // log to console
    printf("TIME:%i JOB:%i ENTER CPU QUEUE\n", currentTime, tempEvent.procID);
    fprintf(OutTXT,"TIME:%i JOB:%i  ENTER CPU QUEUE\n", currentTime, tempEvent.procID);
    // push top of the event queue to cpu queue
    CPUQueue.push(tempEvent);
    jobCounter = counter;
    // initialize stuff at its the beginning
    if (currentTime <= 1){timeCPUbusy = 0; timeDisk1busy= 0; timeDisk2busy = 0; maxCpuTime= 0;maxDisk1Time= 0; maxdisk2Time= 0;CpuJobs =0 ;Disk1Jobs =0 ; Disk2Jobs =0;}
    return 0;
};

//*************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************

//UPDATE CPU()
int Update_CPU (priority_queue<Event> &eq,queue <Event> &CPUQueue, int &CPU_IDLE, int currentTime,FILE * OutTXT){



                        // if CPU IS NOT BEING USED AND WE HAVE SOMTHING TO PROCESS
                        if ((CPU_IDLE == 0) && !CPUQueue.empty()){


                         int  range =  randomRange(getCPU_MIN(),getCPU_MAX());
                         // find longest job
                         if (range >= maxCpuTime)
                            maxCpuTime = range;
                         // then take the front element off the CPU and push it to the event Queue to process when finished with CPU
                         Event tempEvent = createProcess(CPUQueue.front().procID, currentTime +range,CPU_FINISHED);

                         printf("TIME:%i JOB:%i BEING PROCESSED BY CPU\n", currentTime, tempEvent.procID);
                         fprintf(OutTXT,"TIME:%i JOB:%i BEING PROCESSED BY CPU\n", currentTime, tempEvent.procID);
                         // remove element from the front of the CPU Queue
                         CPUQueue.pop();

                         //push event when CPU will be finished to the event Queue
                         eq.push(tempEvent);
                         // count time the cpu will be busy
                         timeCPUbusy += range;
                         CPU_IDLE =1;
                       //  cout << timeCPUbusy << endl;

                         return 1;
                        } else if
                        // if CPU IS BEING USED
                        (CPU_IDLE == 1)
                        // if idle just log that cpu is idle
                        {
						// fprintf(OutTXT,"CPU IN USE\n");

                        }

return 0;
}// end UPDATE_CPU

//*************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************


//HANDLE CPU FINISH()
int handleCPUfinish(int counter, priority_queue<Event> &eq,queue <Event> &Disk_1_Queue,queue <Event> &Disk_2_Queue, int &CPU_IDLE, int currentTime,FILE * OutTXT){

 int disk1_size = Disk_1_Queue.size();
 int disk2_size = Disk_2_Queue.size();

 //to decide what to do next
 int tempBool;
 Event tempEvent ;
 //get probaility to quit
 tempBool = randomBool(.5f);
 // when the new time will be for the next event
 int newArrivalTime;
 // int to represent what the process will do next, determined by random number belows
 int nextEvent;

if (randomFloat() < getQUIT_PROB())
        {
            //set next event to Process exit
            nextEvent = PROCESS_EXIT;
            // create new time for next arrival event
            newArrivalTime = currentTime+ randomRange(getARRIVE_MIN(),getARRIVE_MAX());
           // create a new event for exiting system with same PID ,random time,and event at Next Event
            tempEvent = createProcess(eq.top().procID,newArrivalTime,nextEvent);

         //   timeCPUbusy += eq.top().Time - currentTime;
            // pop of top of event queue
            eq.pop();
           //set CPU  IDLE

            //push to  NEW event queue
            eq.push(tempEvent);

			printf("TIME:%i JOB:%i FINISHED AT CPU, SENT TO EXIT SYSTEM \n", currentTime, tempEvent.procID);
            fprintf(OutTXT,"TIME:%i JOB:%i BEING PROCESSED BY CPU\n", currentTime, tempEvent.procID);
            CPU_IDLE = 0;
            CpuJobs ++;
        }   else {


                            // go to disk 1 if it has less stuff in it
                             if (randomFloat() <= .5f )
                                {

                                        nextEvent = DISK1_START;
                                        // create new time for next arrival event
                                        newArrivalTime = currentTime+ randomRange(getDISK1_MIN(),getDISK1_MAX());
                                        // create a new event for exiting system with same PID ,random time,and event at Next Event
                                        tempEvent = createProcess(eq.top().procID,newArrivalTime,nextEvent);
                                      //  timeCPUbusy += eq.top().Time - currentTime;
                                        // pop of top of event queue
                                        eq.pop();
										// push to disk
										Disk_1_Queue.push(tempEvent);
                                        printf ("TIME:%i JOB:%i FINISHED AT CPU, SENT TO DISK 1 QUEUE\n",currentTime, tempEvent.procID);
                                        fprintf(OutTXT,"TIME:%i JOB:%i FINISHED AT CPU, SENT TO DISK 1 QUEUE\n", currentTime, tempEvent.procID);
                                        //keep track of #cpu jobs

                                        CpuJobs ++;
                                    } else
                                 // other wise go to disk 2
                                 {


                                nextEvent =DISK2_START;

                                // create new time for next arrival event
                                newArrivalTime = currentTime+ randomRange(getDISK2_MIN(),getDISK2_MAX());
                                // create a new event for adding to cpu queue , with same PID ,random time,and event at CPU_ENTER
                                tempEvent = createProcess(eq.top().procID,newArrivalTime,nextEvent);
                             //   timeCPUbusy += eq.top().Time - currentTime;
                                // pop of top of event queue
                                eq.pop();
                                printf ("TIME:%i JOB:%i FINISHED AT CPU, SENT TO DISK 2 QUEUE\n",currentTime, tempEvent.procID);
                                fprintf(OutTXT,"TIME:%i JOB:%i FINISHED AT CPU, SENT TO DISK 2 QUEUE\n", currentTime, tempEvent.procID);
                                Disk_2_Queue.push(tempEvent);
                                //keep track of #cpu jobs
                                CpuJobs++;

                                 } // end else


                } //end else



            // create new time for next arrival event
            newArrivalTime = currentTime + randomRange(getARRIVE_MIN(),getARRIVE_MAX());
            //create new job arrival at random time
            Event newEvent= createProcess(counter,newArrivalTime,PROCESS_ARRIVAL);
            //push new job to event queue
            eq.push(newEvent);
            //debugging
            printf("TIME:%i JOB:%i HAS ARRIVED \n", currentTime, newEvent.procID);

            fprintf(OutTXT,"TIME:%i JOB:%i HAS ARRIVED \n", currentTime, tempEvent.procID);



    // set cpu not idle
    CPU_IDLE = 0;
    return 0;

}// end handle cpu finish



//
//*************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************
//HANDLE DISK FINISH()
// Handle disk finish checks to see which disk finished then logs that to console/ file
// It saves the top event of the event queue then changes its state to process Arrival, because the disk send events
// back to event queue to be pushed to cpu queue
int handleDISKfinish(priority_queue<Event> &eq, int currentTime ,FILE * OutTXT){

//to decide what to do next
 Event tempEvent;
 // when the new time will be for the next event
 int newArrivalTime;
// int to represent what the process will do next, determined by random number belows
 int nextEvent;

                            nextEvent =PROCESS_ARRIVAL;
                            // create new time for next arrival event
                            newArrivalTime = currentTime+ randomRange(getARRIVE_MIN(),getARRIVE_MAX());
                             // create a new event for adding to cpu queue , with same PID ,random time,and event at CPU_ENTER
                            tempEvent = createProcess(eq.top().procID,newArrivalTime,nextEvent);

                // check if the next event is a disk 1 finished event
			    if (eq.top().EventType == DISK1_FINISHED){
              //  timeDisk1busy += eq.top().Time  - currentTime;;
			    printf ("TIME:%i JOB:%i FINISHED ON DISK1 SENT BACK TO CPU\n",currentTime, tempEvent.procID);
                fprintf(OutTXT,"TIME:%i JOB:%i FINISHED ON DISK1 SENT BACK TO CPU\n", currentTime, tempEvent.procID);
                Disk1Jobs++;
				} else if (eq.top().EventType == DISK2_FINISHED)// check if the next event is a disk 2 finished event
				{
                        // add the time to the total time but minus the current time
				     // timeDisk2busy += eq.top().Time - currentTime;
			    printf ("TIME:%i JOB:%i FINISHED ON DISK2 SENT BACK TO CPU\n",currentTime, tempEvent.procID);
                fprintf(OutTXT,"TIME:%i JOB:%i FINISHED ON DISK2 SENT BACK TO CPU\n", currentTime, tempEvent.procID);
                Disk2Jobs++;
				}
                            // pop of top of event queue
                            eq.pop();
                            //push to event queue
                            eq.push(tempEvent);



        //    }
    return 0;

}// end handle DISK finish

//
//*************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************

//UPDATE DISK()
int UPDATE_DISK ( priority_queue<Event> &eq,queue <Event> &Disk_1_Queue,queue <Event> &Disk_2_Queue, int &DISK2_IDLE,  int &DISK1_IDLE, int currentTime ,FILE * OutTXT){

                        // if DISK IS NOT BEING USED AND HAS SOMTHING TO PROCESS
                        if ((DISK1_IDLE == 0) && !Disk_1_Queue.empty() ){

                            int range =  randomRange(getDISK1_MIN(),getDISK1_MAX());
                             // find longest job
                            if (range >= maxDisk1Time)
                            maxDisk1Time = range;
							// make new event with same id as front of disk 1 but with disk finished status and new time
							Event tempEvent = createProcess(Disk_1_Queue.front().procID, currentTime + range, DISK1_FINISHED );
							printf("TIME:%i JOB:%i BEING PROCESSED  BY DISK1 \n", currentTime, tempEvent.procID);
                            fprintf(OutTXT,"TIME:%i JOB:%i BEING PROCESSED  BY DISK1 \n", currentTime, tempEvent.procID);

							// remove from disk 1 queue
							Disk_1_Queue.pop();

							// add Disk1_finished event to queue
							eq.push(tempEvent);

							// add the time the dis will be busy to the counter for stats
							timeDisk1busy += range;

						printf("TIME:%i JOB:%i BEING PUSHED TO EVENT Q from DISK 1 WITH TYPE %i \n", currentTime, tempEvent.procID, tempEvent.EventType);
                        //DISK BEING USED
                        DISK1_IDLE =1;

                        } else if (DISK1_IDLE == 1)
                        // if idle log to console
                        { cout <<"DISK 1 IN USE " <<endl;
                            // fprintf(OutTXT,"DISK 1 IN USE \n");

                        }

                        // if DISK IS NOT BEING USED AND HAS SOMTHING TO PROCESS
                        if ((DISK2_IDLE == 0 ) && !(Disk_2_Queue.empty()) ){

                        int range =  randomRange(getDISK2_MIN(),getDISK2_MAX());
                        // find longest job
                        if (range >= maxdisk2Time)
                        maxdisk2Time = range;
                        Event tempEvent = createProcess(Disk_2_Queue.front().procID,currentTime + range,DISK2_FINISHED);
                        printf("TIME:%i JOB:%i BEING PROCESSED  BY DISK2 \n", currentTime, tempEvent.procID);
                        fprintf(OutTXT,"TIME:%i JOB:%i BEING PROCESSED  BY DISK2 \n", currentTime, tempEvent.procID);

						cout<<Disk_2_Queue.front().procID<< " POPPING OFF OF DISK 2 QUEUE"<< endl;
                        Disk_2_Queue.pop();

                        // add the time the dis will be busy to the counter for stats
                        timeDisk2busy += range;

                        eq.push(tempEvent);
						printf("TIME:%i JOB:%i BEING PUSHED TO EVENT Q from DISK 2 WITH TYPE %i \n", currentTime, tempEvent.procID, tempEvent.EventType);

                        //DISK BEING USED
                        DISK2_IDLE =1;

                        } else if (DISK2_IDLE == 1)
                        // // if idle log
                        {
							cout <<"DISK 2 IN USE " <<endl;
                            //fprintf(OutTXT,"DISK 2 IN USE \n");

                        }
return 0;
}// end UPDATE_DISK

//*************************************************************************************************************************************************************************************
//*************************************************************************************************************************************************************************************

// for statistics file for testing

int printUtil(FILE * StatTXT, int finTime, int init){


   float fin = (float)finTime;
   // total time is time finished - when we started
   float totalTime = ( finTime+0.0) - (float)init;
   // time utilization is time busy/ over total time
   double CPUutil = ((float)timeCPUbusy) /totalTime;
   double Disk1 = ((float)timeDisk1busy )/totalTime;
   double Disk2 = ((float)timeDisk2busy)/totalTime;
   double EQjobs = ((double)CpuJobs) +   ((double)Disk1Jobs) + ((double)Disk2Jobs) ;
   // through put is  calculated by time active / number of jobs
   double CPUthrough =   ((double)CpuJobs) / (float)timeCPUbusy+0.0   ;
   double Disk1through =  ((double)Disk1Jobs) /(float)timeDisk1busy+0.0 ;
   double Disk2through =  ((double)Disk2Jobs) / (float)timeDisk2busy+0.0 ;
  // double EQthrough =  EQjobs / totalTime + 0.0f  ;

   // utilization is time busy/ total time being used
  fprintf(StatTXT,"CPU Utilization: %f.2%c \n",CPUutil,'%');
  fprintf(StatTXT,"Disk1 Utilization: %f.2%c \n",Disk1,'%');
  fprintf(StatTXT,"Disk2 Utilization: %f.2%c \n",Disk2,'%');
  fprintf(StatTXT,"\n");
  fprintf(StatTXT,"CPU total jobs processed: %i \n",CpuJobs);
  fprintf(StatTXT,"Disk1 total jobs processed: %i \n",Disk1Jobs);
  fprintf(StatTXT,"Disk2 total jobs processed: %i \n",Disk2Jobs);
  fprintf(StatTXT,"\n");
  fprintf(StatTXT,"************************************\n");
  fprintf(StatTXT,"************************************\n");
  fprintf(StatTXT, "THROUGHPUT \n\n");
  //through put is jobs services/ time spent servicing jobs

  //fprintf(StatTXT,"EQ Throughput: %.3f \n",EQthrough);
  fprintf(StatTXT,"CPU Throughput: %.3f \n",CPUthrough);
  fprintf(StatTXT,"Disk1 Throughput: %.6f \n",Disk1through);
  fprintf(StatTXT,"Disk2 Throughput: %.6f \n",Disk2through);
  fprintf(StatTXT,"\n");
  fprintf(StatTXT,"************************************\n");
  fprintf(StatTXT,"************************************\n");
  fprintf(StatTXT, "RESPONSE TIME \n\n");
  // longest job
  fprintf(StatTXT,"CPU longest job: %i \n",maxCpuTime);
  fprintf(StatTXT,"DISK1 longest job: %i \n",maxDisk1Time);
  fprintf(StatTXT,"DISK2 longest job: %i \n\n",maxdisk2Time);

  float tmp = ((float)timeCPUbusy+0.0 )/((double)CpuJobs);
  // average length of job is all the time the cpu is busy/ number of jobs
  fprintf(StatTXT,"CPU average job: %f.4 \n", tmp);
  tmp = ((float)timeDisk1busy+0.0 )/((double)Disk1Jobs);
  fprintf(StatTXT,"DISK1 average job: %f.4 \n",tmp);
  tmp = ((float)timeDisk2busy+0.0 )/((double)Disk2Jobs);
  fprintf(StatTXT,"DISK2 average job: %f4.4 \n",tmp);

return 0;

}

//getters
int getCpuBusy(){
    int num = timeCPUbusy;
    return num;}
int getDisk1Busy(){
    int num = timeDisk1busy;
    return num;}
int getDisk2Busy(){
    int num = timeDisk2busy;
    return num;}

