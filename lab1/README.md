# Abraham Schultz
## Discrete_Event_Simulation

Assignment 1 for fall 2019 Temple 3207

This is my implementation of a Discrete Event Simulation.

We have a virtual CPU which processes event structs.
There is a QUIT_PROB which is read in from the Config.txt file
which determines if the event quits after the CPU is done.

The CPU can then pass the event struct on to do I/O at 
one of two virtual DISKs. Once done at the disks the 
event gets passed back to the CPU.

The process repeats until the value FIN_TIME (loaded in config file) is reached.

This is all handled by the event queue.


### There are 5 major data structures in this project
- EventQueue (this is a priority queue)
- CpuQueue   (FIFO queue)
- Disk1Queue   (FIFO queue)
- Disk2Queue   (FIFO queue)
- Event(Struct with three variables){ 
 PID;
 Time;
 EventType;
}

###  Files Included
 - Des.h                 //header file
 - InitilizeConfig.cpp   // file that contains function to read in config file
 - RandomGen.cpp         // containes function for generating random number, reads using SEED from initConfig
 - DesDriver.cpp         // this is the main driver program which uses the other files in its while loop
 - CreateProcess.cpp     // this is file that contains the function to create new processes
 - EventHandlers.cpp     // this contains all the event handlers
 - Config.txt            //Text file to read in initial values from
 - log.txt              //Text file to output logs too
 - Stat.txt             // Text file to output statistics too
 - Runs.txt             // documentation on runtime testing data
 
 See UML diagram for more information on functions contained in these files.

![UML Diagram](https://github.com/Agent215/AbrahamSchultz_3207Labs/raw/master/lab1/DES_diagram_AbrahamSchultz.png)


- To compile type : g++ .cpp* -o YourExeNameHere
- to run type: ./YourExeNameHere

