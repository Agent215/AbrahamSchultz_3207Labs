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

void serviceClient(int &client, string dict[]){


    while (1) {
                    char buf[1024];                             // holds user input
                    for (int i = 0; i < sizeof(buf); i++)
                    buf[i] = '\0';                              // make sure buf is empty by clearing it out


                    string tmpMsg;
                    tmpMsg += "please enter a word to check\n";                   //prompt message
                    write(client, tmpMsg.c_str(), tmpMsg.size());
                   // cout << "waiting for word" << endl;                           // server is watiting for input
                    read(client, buf, 1024);
                    buf[strlen(buf)+1] = '\0';
                    string s = buf;
                    s.erase(s.length()-2);
                    s.erase(remove(s.begin(), s.end(), '\n'), s.end());

                    //debugging
                    cout <<"Recieved " << buf << endl;

                    int correct = checkSpell(s,dict );     // call checkSpell function, 1 if correct


                      if (correct == 1) {   // if correct then tell client

                          tmpMsg = "OK ";                   //OK message
                          tmpMsg += s;
                          tmpMsg += "\n";
                          write(client, tmpMsg.c_str(), tmpMsg.size());

                                       } // end if
                      else{

                          tmpMsg = "MISSPELLED ";                  // if misspelled send to client
                          tmpMsg += s;
                          tmpMsg += "\n";
                          write(client, tmpMsg.c_str(), tmpMsg.size());
                           } // end else

                        for (int i = 0; i < sizeof(buf); i++)    // clear buf
                          buf[i] = '\0';

    }

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
