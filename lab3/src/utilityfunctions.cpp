/*
utilityFunctions.cpp
Abraham Schultz

last edited 10/16/2019

this will contain various functions needed to create a
networked spell checker program

*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <queue>
#include <fstream>
#include <iostream>
#include <algorithm>
#include "netSpell.h"


using namespace std;



/*
****************************************************************************************************
 this will be the function to check a given string against the given library
 return 1 if correct 0 if incorrect
*/
int checkSpell(string input, string dict[]){

                 int correct = 0;                                               // is 1 if correct

                  for (int i = 0; i < FILESIZE; i++)                            // check against dictionary array
                            {

                                 if (strcmp(input.c_str(),dict[i].c_str())==0)
                                     {                                            //compare against each word in array
                                        cout << "OK " <<  input.c_str() <<endl;
                                        correct =1;
                                        break;
                                     } // end if

                            } // end for
                                if (correct == 0)
                               cout << "MISSPELLED " << input.c_str() <<endl;


return correct;
} // end checkSpell


/*
*****************************************************************************************************
 this function will perform the service of spell checking and receiving and sending data to client
*/

int serviceClient(int &client, string dict[],queue<string> &logqueue){


    while (1) {
                    char buf[1024];                             // to holds user input
                    for (int i = 0; i < sizeof(buf); i++)
                    buf[i] = '\0';                              // make sure buf is empty by clearing it out


                    string tmpMsg;                                                    //tmp string to hold message for user at any given time
                    tmpMsg += "enter a word to check\n";                              //prompt message
                    write(client, tmpMsg.c_str(), tmpMsg.size());
                    // cout << "waiting for word" << endl;                            // server is waiting for input
                     read(client, buf, 1024);                                         // receive input from client

                    //debugging
                    cout <<"Received " << buf << endl;


                    string a (buf);                                      //convert to c++ style string
                    a.erase(a.length()-2);
                    a.erase(remove(a.begin(), a.end(), '\n'), a.end());  // erase any nextline escape sequence chars in string
                    int correct = checkSpell(a,dict );                   // call checkSpell function, 1 if correct



			// if the user types exit then exit loop. this frees up the worker thread to accpet new clients
                     if (strcmp(a.c_str(), "exit")==0){ cout << "a client exited program" << endl; break;}
                      if (correct == 1) {   // if correct then tell client

                          tmpMsg = "OK ";                   //OK message
                          tmpMsg += a;
                          tmpMsg += "\n";
                       //   write(client, tmpMsg.c_str(), tmpMsg.size());

                                       } // end if
                      else{

                          tmpMsg = "MISSPELLED ";                  // if misspelled send to client
                          tmpMsg += a;
                          tmpMsg += "\n";
                       //   write(client, tmpMsg.c_str(), tmpMsg.size());
                           } // end else

                            // add log data to log queue
                            logqueue.push(tmpMsg);

                        for (int i = 0; i < sizeof(buf); i++)    // clear buf
                          buf[i] = '\0';

    }
return 0;
} // end serviceClient



/*
*****************************************************************************************************************************
this function is copied from page 906 of "Computer Systems a programers perspective"
this is a helper function which opens binds and returns and listening socket.
*/
int open_listenfd(int port) {
	int listenfd, optval=1;         // file descriptors
	struct sockaddr_in serveraddr;  // struct for server address



	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}


	 if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
	 (const void *)&optval , sizeof(int)) < 0){
	 	return -1;
	 }


	 bzero((char *) &serveraddr, sizeof(serveraddr));
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 serveraddr.sin_port = htons((unsigned short)port);
	 if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
	 	return -1;
	 }


	 if (listen(listenfd, 20) < 0){
	 	return -1;
	 }

	 return listenfd;
}
