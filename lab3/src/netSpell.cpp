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

const int PORT = 8888;
extern int errno;      // errno for use with std error
queue<string> logQueue;        // queue  to hold buffer for access to which workers threads can write to log file
queue<int> clientQueue;     // queue to keep track of clients.

// thread pool 
pthread_t worker1;
pthread_t worker2;
pthread_t worker3;
pthread_t logger;



// main function for program
int main (){
string* diction = new string [99171];

//load dictionary
diction = loadDiction();


int connectPort = PORT;




    int socket_desc, new_socket, c;
    struct sockaddr_in sin;
    struct sockaddr_in server, client;
    char *message;

    //create socket(create active socket descriptor)
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if(socket_desc == -1) //if an error occurred when creating the socket
    {
        puts("Error creating socket!");
    }
    //prepare the sockaddr_instructure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);
    //Bind (connect the server's socket addressto the socket descriptor);print a message and exit the program if an error occurred
    if(bind(socket_desc, (struct sockaddr*)&server, sizeof(server)) <0)
    {
        puts("Error: Bind Failed");
        return 1;
    }
    puts("Bind done.");
    //listen (converts the active socket to a LISTENING socket; can accept connections)
    listen(socket_desc, 3);
    //Print a message saying that the server is waiting for incoming connections
    cout << "Waiting for incoming connections at port number " << PORT << "..." << endl;
    //while loop that continues to wait for incoming connections
    while(1)
    {
        //Accept an incoming connection; create a new CONNECTED descriptor
        c = sizeof(struct sockaddr_in);
        new_socket = accept(socket_desc, (struct sockaddr*)&client, (socklen_t*)&c);
        if(new_socket < 0)
        {
            perror("Error: Accept Failed");
            return 1;
        }
        puts("Connection accepted.");


        char buf[1024];
        for (int i = 0; i < sizeof(buf); i++)
        buf[i] = '\0';  
	
	// add cleint to queue 
       clientQueue.push(new_socket);
		while (1) {

                    
  		 serviceClient(new_socket, diction);
   			 } // end while


} // end while


} // end main

