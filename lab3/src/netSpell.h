/*
netSpell.h

Abraham Schultz

Last Edited 10/16/2019

This is the header file for assignment number three from Temple university
3207 course on operating systems.

The program should Implement a networked spell checker server.


*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <cstring>
#include <fstream>
#include <iostream>
#include <queue>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#ifndef NETSPELL_H
#define NETSPELL_H


using namespace std;

const int FILESIZE = 99171;   // used word count program to count lines of dictionary file


/*
Declare functions
*/
int checkSpell(string input, string dict[]);
string * loadDiction(char *DICT);
int open_listenfd(int port);
int serviceClient(int &client, string dict[],queue<string> &logqueue);
void* work(void *varg);
void *log  (void *varg);
int printQueue(queue<int> clientQueue);
#endif
