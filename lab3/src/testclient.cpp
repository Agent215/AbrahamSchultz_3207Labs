/*
testServer.cpp
Abraham Schultz

taken from class slides

*/
#include<stdio.h>
#include<string.h>  //inet_addrint
#include<sys/socket.h>
#include<iostream>
#include <pthread.h>
#include<arpa/inet.h>  //inet_addrint

using namespace std;

void *connectThread(void * argv);


// thread pool
pthread_t client1;
pthread_t client2;
pthread_t client3;

pthread_t  threadPool[100];

string temp;
 main(int argc, char *argv[])
 {


 for (int i =0 ; i <3 ; i++){

pthread_t tmp;
 threadPool[i] = tmp ;
 }
 // create worker thread pool
for (int i =0 ; i <3 ; i++){


 // pthread_create( tmp, NULL, connectThread, NULL);
 pthread_create(&threadPool[i], NULL, connectThread, NULL);
//pthread_create(&client3, NULL, connectThread, NULL);




//int socket_desc;
//	 struct sockaddr_in server;
//	 char *message , server_reply[2000];
//	 //Create socket
//	 socket_desc= socket(AF_INET , SOCK_STREAM , 0);
//	 if (socket_desc== -1)
//           {printf("Could not create socket");}
//	 server.sin_addr.s_addr= inet_addr("129.32.95.110");
//	 server.sin_family= AF_INET;server.sin_port= htons( 8000 );
//	 //Connect to remote server
//	 if (connect(socket_desc, (struct sockaddr*)&server , sizeof(server)) < 0)
//            {cout <<"connect error" << endl;}
//
//    // puts("Connected");
//     	 message = "testing \n";
//	 if( send(socket_desc, message , strlen(message) , 0) < 0){
//            puts("Send failed");}
//           cout <<"word sent" << endl;                                 //Send some data
//          if( recv(socket_desc, server_reply, 2000 , 0) < 0)   //Receive a reply from the server
//           {puts("recvfailed");}
//     puts("Reply received");
//     puts(server_reply);
//
}

 }


 /*
 thread for multithreading connection
 */
void *connectThread(void * argv){


while(1){

    int socket_desc;
	 struct sockaddr_in server;
	 char *message , server_reply[2000];
	 //Create socket
	 socket_desc= socket(AF_INET , SOCK_STREAM , 0);
	 if (socket_desc== -1)
           {printf("Could not create socket");}
	 server.sin_addr.s_addr= inet_addr("129.32.95.110");
	 server.sin_family= AF_INET;server.sin_port= htons( 8000 );
	 //Connect to remote server
	 if (connect(socket_desc, (struct sockaddr*)&server , sizeof(server)) < 0)
            {cout <<"connect error" << endl;}

    // puts("Connected");
     	 message = "testing";
	 if( send(socket_desc, message , strlen(message) , 0) < 0){
            puts("Send failed");}
           cout <<"word sent" << endl;                                 //Send some data
          if( recv(socket_desc, server_reply, 2000 , 0) < 0)   //Receive a reply from the server
           {puts("recvfailed");}
     puts("Reply received");
     puts(server_reply);
}

	// return 0;



} // end connectThread
