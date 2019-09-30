#include "Des.h"
#include <stdlib.h>

/*
CreateProcess.cpp

Abraham Schultz

Last Edited 9/11/2019

a function to be called when creating a new process

*/

// function to create a process
Event createProcess (int PID, int time, int event){
// new process to return
Event newProc;
// set time to desired time
newProc.Time = time;
// set to desired PID
newProc.procID =PID;
// set desired starting state
newProc.EventType = event;
//return to be used pr main
return newProc;
} // end createProcess
