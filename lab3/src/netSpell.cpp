/*
netSpell.cpp
Abraham Schultz

last edited 11/3/2019

This is the main file for the networked spell checker program
which is the third lab as part of temple university 3207 OS class



this file contains the main function which has the main while loop.
within the loop we continually check for incoming connection requests and then add them
to the clientQueue.  we also create a pool of worker threads which continually look to see if the
clientQueue is empty.
There are two critical sections in this code where race condition can occur.
1. the clientQueue 2. the logQueue
The workers also check a lock to see if any other workers are editing the queues.
then if all is clear, and  if it is not empty then the worker executes the service routine
which in this case checks the word against a dictionary file.

*/


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <errno.h>
#include <iostream>
#include <pthread.h>
#include <sys/types.h>
#include <time.h>
#include <queue>
#include <algorithm>
#include "netSpell.h"


using namespace std;

int PORT = 8000;
char *DICTIONARY;
extern int errno;              // errno for use with std error
queue<string> logQueue;        // queue  to hold buffer for access to which workers threads can write to log file
queue<int> clientQueue;        // queue to keep track of clients.

int MAXCLIENTS = 3;

// mutual exclusion variables for client queue
pthread_mutex_t socketEdit =  PTHREAD_MUTEX_INITIALIZER;             // lock for editing clientQueue
pthread_cond_t clientNotEmpty =  PTHREAD_COND_INITIALIZER;          // condition if we have clients in queue
pthread_cond_t clientNotFull =  PTHREAD_COND_INITIALIZER;          // if we are not full

//mututal exclusion condition variables for log queue
pthread_mutex_t logEdit =  PTHREAD_MUTEX_INITIALIZER;			  // lock for editing logQueue
pthread_cond_t logNotEmpty =  PTHREAD_COND_INITIALIZER;          // condition if we have log in queue
pthread_cond_t logNotFull =  PTHREAD_COND_INITIALIZER;

// thread pool
pthread_t worker1;
pthread_t worker2;
pthread_t worker3;
pthread_t logger;

string* gdiction = new string [99171];

/*
************************************************************************************************************
*/

// main function for program
int main (int argc , char** argv){


// create worker thread pool
pthread_create(&worker1, NULL, work, NULL);
pthread_create(&worker2, NULL, work, NULL);
pthread_create(&worker3, NULL, work, NULL);
pthread_create(&logger, NULL, log, NULL);

//check if user provided arguments for dictionary and port
if (argc == 2) { DICTIONARY = argv[1];
cout << "loading dictionary from " << DICTIONARY << endl;
}// end if
if (argc == 3) { DICTIONARY = argv[1]; PORT = atoi(argv[2]);
//cout << "loading dictionary from " << DICTIONARY << endl;
}
else {
	DICTIONARY = "dictionary.txt";
	cout << "loading dictionary from " << DICTIONARY << endl;
     }// end else



//load dictionary
gdiction = loadDiction(DICTIONARY);


    int socket_desc, new_socket, c;     //socket file descriptors
    struct sockaddr_in  client;

    // here we call the function to open and assign incoming port
     socket_desc = open_listenfd(PORT);

    // log to console
    cout << "Waiting for incoming connections at port number " << PORT << "..." << endl;



    //while loop that continues to wait for incoming connections
    while(1)
    {

        c = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c); //Accept an incoming connection and assign new file descriptor for incoming client
        if(new_socket < 0)
        {
            printf("Error: %s\n", strerror(errno));
            return 1;
        }
        puts("Connection accepted.");
		pthread_cond_signal(&clientNotEmpty);
//              if(clientQueue.size() >MAXCLIENTS){      // check and see if we have reached maximum amount of clients
//
//              pthread_cond_wait(&clientNotFull, &socketEdit);
//               }
//else{
//        //otherwise we can signal that we are not full
//     pthread_cond_signal(&clientNotFull);
//   }
	       // add client to queue
          clientQueue.push(new_socket);
		  // print queue for debugging
		 //  printQueue(clientQueue);




    } // end while

	return 0;
} // end main

/*
************************************************************************************************************
*/

//this the worker threads function
// there is a critical section in this code pertaining to workers accessing the
// client queue we will use mutex and lock to prevent race conditions here.
void *work(void *varg){
    int new_socket;
    while(1){
        //lock mutex
        pthread_mutex_lock(&socketEdit);
        //check to see if buffer is empty
        while(clientQueue.empty())
                {
                    // if we are empty then set condition we have space and we are not editing queue
                    pthread_cond_wait(&clientNotEmpty, &socketEdit);
                } // end while
                        if(!clientQueue.empty()){

                                        new_socket = clientQueue.front();
                                        clientQueue.pop();

                                        }
        // unlock
        pthread_mutex_unlock(&socketEdit);


        //Read from client and check words
        int work = serviceClient(new_socket,gdiction,logQueue);   // work = 1 if we failed that means user selected to exit
     //   pthread_cond_signal(&logNotEmpty);

            if (work ==1)
            {
                // we should not reach here
                break;
            }
         } // end while

} // end work


/*
************************************************************************************************************
*/

//worker thread to log work to logfile
void *log  (void *varg){

     string w ;
     fstream logFile;
     logFile.open("log.txt", ofstream::out | ofstream::trunc);

  while(1){
        //lock mutex
        pthread_mutex_lock(&logEdit);

        while(logQueue.empty())
                {
                //   pthread_cond_wait(&logNotEmpty, &logEdit);
                }if(!logQueue.empty()){

                        //write to log file
                         w = logQueue.front();
                        logQueue.pop();

               }
               // unlock
               pthread_mutex_unlock(&logEdit);

             //write to log file
             logFile << w << endl;


         }

} // end log

/*
************************************************************************************************************
*/
//testing function for debugging
int printQueue(queue<int> clientQueue){

	int len = clientQueue.size();
	for (int i = 0; i < len; i++) {

		cout << "queue item " << i << "=" << clientQueue.front() << endl;
		clientQueue.pop();
	}
	return 0;
} // end printQueue
