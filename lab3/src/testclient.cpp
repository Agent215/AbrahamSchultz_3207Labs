/*
testServer.cpp
Abraham Schultz

taken from class slides

*/
#include<stdio.h>
#include<string.h>  //inet_addrint
#include<sys/socket.h>
#include <unistd.h>
#include<iostream>
#include <pthread.h>
#include<arpa/inet.h>  //inet_addrint

using namespace std;

void *connectThread(void * argv);


// thread pool
pthread_t client1;
pthread_t client2;
pthread_t client3;
pthread_mutex_t lock;
pthread_cond_t usingserver;

pthread_t  threadPool[100];


     int socket_desc;
	 struct sockaddr_in server;
	 char *message , server_reply[2000];

string temp;
 main(int argc, char *argv[])
 {


// set mutual exclusion conditions on start to open
pthread_mutex_unlock(&lock);
pthread_cond_signal(&usingserver);


     //Create socket
	 socket_desc= socket(AF_INET , SOCK_STREAM , 0);
	 if (socket_desc== -1)
     {printf("Could not create socket");}
	 server.sin_addr.s_addr= inet_addr("129.32.95.111");
	 server.sin_family= AF_INET;server.sin_port= htons( 8888 );
	 //Connect to remote server
	 if (connect(socket_desc, (struct sockaddr*)&server , sizeof(server)) < 0)
            {
               cout <<"connect error" << endl;
            }

         puts("Connected");







pthread_create(&client1, NULL, connectThread, NULL);
pthread_create(&client2, NULL, connectThread, NULL);
pthread_create(&client3, NULL, connectThread, NULL);


while (1) {
           if( recv(socket_desc, server_reply, 2000 , 0) < 0)            //Receive a reply from the server

           {

             puts("recvfailed");


           }
}

}




 /*
 thread for multithreading connection

 here we will send a few words to test then one missspelled word.
 then the exit command.
 */
void *connectThread(void * argv){


while(1){

//
  pthread_mutex_lock(&lock);
     	   message = (char *)"hello  ";

	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
             message = (char *)"word  ";

	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
             message = (char *)"notaword  ";

	       if( send(socket_desc, message , strlen(message) , 0) < 0){
           puts("Send failed");
         break;
            }
            usleep(1000);
              message = (char *)"exit  ";
	       if( send(socket_desc, message , strlen(message) , 0) < 0){
         //  puts("Send failed");
         break;
            }
          // cout <<"word sent" << endl;                                 //Send some data

    pthread_mutex_unlock(&lock);


}

	// return 0;



} // end connectThread
