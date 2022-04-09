#include<stdio.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include "structs.h"
#include "client_functions.h"

int main()
{
	int sd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser;
	if(sd==-1)
	{
		perror("failed at socket system call\n");
		exit(EXIT_FAILURE);
	}
	ser.sin_family = AF_INET;
	ser.sin_port = htons(8800);
	
	if(inet_pton(AF_INET,"127.0.0.1",&ser.sin_addr)==-1)
	{
		perror("address is invalid\n");
		exit(EXIT_FAILURE);
	}
	
	if(connect(sd,(struct sockaddr *)&ser,sizeof(ser))==-1)
	{
		perror("connection failure\n");
		return -1;
	}
	printf("connected to railway ticket booking server\n");
	
	
	mainMenu(sd);	
}
