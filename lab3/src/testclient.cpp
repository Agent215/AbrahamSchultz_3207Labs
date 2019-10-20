#include<stdio.h>
#include<string.h>  //inet_addrint
#include<sys/socket.h>
#include<iostream>
#include<arpa/inet.h>  //inet_addrint

using namespace std;

 main(int argc, char *argv[])
 {
	 int socket_desc;
	 struct sockaddr_in server;
	 char *message , server_reply[2000];
	 //Create socket
	 socket_desc= socket(AF_INET , SOCK_STREAM , 0);
	 if (socket_desc== -1)
           {printf("Could not create socket");}
	 server.sin_addr.s_addr= inet_addr("127.0.0.1");
	 server.sin_family= AF_INET;server.sin_port= htons( 8888 );
	 //Connect to remote server
	 if (connect(socket_desc, (struct sockaddr*)&server , sizeof(server)) < 0)
            {cout <<"connect error" << endl;return 1;}

     puts("Connected\n");
     	 message = "testing\n";
	 if( send(socket_desc, message , strlen(message) , 0) < 0){
            puts("Send failed");return 1;}
           cout <<"word sent" << endl;                                 //Send some data
          if( recv(socket_desc, server_reply, 2000 , 0) < 0)   //Receive a reply from the server
           {puts("recvfailed");}
     puts("Reply received\n");
     puts(server_reply);
	 return 0;

 }
