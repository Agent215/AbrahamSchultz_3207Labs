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
#include <errno.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <errno.h>
#include <iostream>
#include "netSpell.h"

const int PORT = 8888;
extern int errno;      // errno for use with std error
// main function for program
int main (){
string* diction = new string [99171];

int connectPort = PORT;
struct sockaddr_in client;
socklen_t clientLen = sizeof(client);
int connectSocket, clientSocket;

//load dictionary
diction = loadDiction();

connectSocket = open_listenfd(connectPort);
 if(connectSocket == -1){
      cout << strerror(errno) << endl;
   }// end if




    while(1) {
        if((clientSocket = accept(connectSocket, (struct sockaddr*)&client, &clientLen)) == -1){
            printf("Error connecting to client.\n");
            return -1;
        }


        char *success_msg = "connection to server successful!\n";
        send(clientSocket, success_msg, strlen(success_msg), 0);
        char *wait_msg = "please wait...\n";
        send(clientSocket, wait_msg, strlen(wait_msg), 0);



    } // end while
//
//                 // loop for testing string matching
//                 while (1){
//
//                       string buf;                                                //create a string buf to hold user input
//                       cout << "hello please enter a word to check" << endl;      // get user input from console
//                       getline( cin, buf);
//
//                        if  (strcmp(buf.c_str(),"exit")==0)                       // check to exit
//                              {
//                                return 0;
//                                break;
//                              } // end if
//
//                              checkSpell(buf,diction );                          // call checkSpell function
//
//                  } // end while





} // end main
