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
#include <fstream>
#include <iostream>
#include "netSpell.h"


using namespace std;



//*****************************************************************************************************
// this will be the function to check a given string against the given library
//return 1 if correct 0 if incorrect
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
                               cout << " MISSPELLED " << input.c_str() <<endl;


return correct;
} // end checkSpell

//*****************************************************************************************************

/*
this function is copied from page 906 of "Computer Systems a programers perspective"
this is a helper function which opens a returns and listening socket.
*/
int open_listenfd(int port) {
	int listenfd, optval=1;
	struct sockaddr_in serveraddr;

	/* Create a socket descriptor */
	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		return -1;
	}

	 /* Eliminates "Address already in use" error from bind */
	 if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR,
	 (const void *)&optval , sizeof(int)) < 0){
	 	return -1;
	 }

	 //Reset the serveraddr struct, setting all of it's bytes to zero.
	 //Some properties are then set for the struct, you don't
	 //need to worry about these.
	 //bind() is then called, associating the port number with the
	 //socket descriptor.
	 bzero((char *) &serveraddr, sizeof(serveraddr));
	 serveraddr.sin_family = AF_INET;
	 serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	 serveraddr.sin_port = htons((unsigned short)port);
	 if (bind(listenfd, (struct sockaddr*)&serveraddr, sizeof(serveraddr)) < 0){
	 	return -1;
	 }

	 //Prepare the socket to allow accept() calls. The value 20 is
	 //the backlog, this is the maximum number of connections that will be placed
	 //on queue until accept() is called again.
	 if (listen(listenfd, 20) < 0){
	 	return -1;
	 }

	 return listenfd;
}
