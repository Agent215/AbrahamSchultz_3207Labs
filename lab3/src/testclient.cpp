/*
testServer.cpp
Abraham Schultz

taken from class slides and modified for multi-threading
network connection testing and server client testing of network spell checking.

*/
#include<stdio.h>
#include<string.h>  //inet_addrint
#include<sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include<iostream>
#include <pthread.h>
#include<arpa/inet.h>  //inet_addrint

using namespace std;

void *connectThread(void * argv);

//queue<int> clientQueue;        // queue to keep track of clients.

// thread pool
pthread_t client1;
pthread_t client2;
pthread_t client3;
pthread_mutex_t lock;
pthread_cond_t usingserver;

// default values for host and port info
int PORT = 8000;
char * hostname = (char *)"129.32.95.111";

//was gonna use this for thread pool
pthread_t  threadPool[100];

// network connection variables
int socket_desc;
struct sockaddr_in server;
char *message ,server_reply[2000];

/*
*****************************************************************************************************
main method here we create a pool of worker threads to each try and connect to a server that check spelling of input
words. each threads will attempt to send a series of messages
before exiting to allow for more clients to connect.
*/
 main(int argc, char *argv[])
 {


// set mutual exclusion conditions on start to open
pthread_mutex_unlock(&lock);
pthread_cond_signal(&usingserver);



// check for input args otherwise use deault values for networking
if (argc == 2) { PORT = atoi(argv[2]);
cout << "trying port " << PORT << endl;
}// end if
if (argc == 3) { hostname = argv[1]; PORT = atoi(argv[2]);
cout << "trying port " << PORT << endl;
}

     //Create socket
	 socket_desc= socket(AF_INET , SOCK_STREAM , 0);
	 if (socket_desc== -1)
     {printf("Could not create socket");}
	 server.sin_addr.s_addr= inet_addr(hostname);
	 server.sin_family= AF_INET;server.sin_port= htons( PORT );
	 //Connect to remote server
	 if (connect(socket_desc, (struct sockaddr*)&server , sizeof(server)) < 0)
            {
               cout <<"connect error" << endl;
            }
     puts("Connected");


// create worker pool
pthread_create(&client1, NULL, connectThread, NULL);
pthread_create(&client2, NULL, connectThread, NULL);
pthread_create(&client3, NULL, connectThread, NULL);


// go forever!!
while (1) {
        pthread_create(&client3, NULL, connectThread, NULL);
           if( recv(socket_desc, server_reply, 2000 , 0) < 0)            //Receive a reply from the server
           {

             puts("recvfailed");

           } // end if
         } // end while

} // end main

/*
*****************************************************************************************************
*/

 /*
 thread for multithreading connection

 here we will send a few words to test then one missspelled word.
 then the exit command.
 */
void *connectThread(void * argv){

while(1){

           pthread_mutex_lock(&lock);       // lock

     	   message = (char *)"hello  ";
            //send first word
	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
             message = (char *)"word  ";
            //send second word
	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
             message = (char *)"notaword  ";
            //send third word
	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
              message = (char *)"exit  ";
            //send exit command word
	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }

    pthread_mutex_unlock(&lock);        // unlock

    } // end while


} // end connectThread
