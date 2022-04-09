#include<stdio.h>
#include<sys/types.h>          
#include<sys/socket.h>
#include<stdlib.h>
#include<fcntl.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>
#include<fcntl.h>
#include "structs.h"
#include "server_functions.h"


int main()
{
	int sd = socket(AF_INET,SOCK_STREAM,0);
	struct sockaddr_in ser,cli;
	int clisz = sizeof(cli);
	int nsd;
	if(sd==-1)
	{
		perror("failed at socket system call\n");
		exit(EXIT_FAILURE);
	}
	ser.sin_family = AF_INET;
	ser.sin_addr.s_addr = INADDR_ANY;
	ser.sin_port = htons(8800);
	
	if(bind(sd,(struct sockaddr*)&ser,sizeof(ser))==-1)
	{
		perror("failed at bind system call\n");
		exit(EXIT_FAILURE);
	}
	if(listen(sd,5)==-1)
	{
		perror("failed at listen system call\n");
		exit(EXIT_FAILURE);
	}
	
	printf("railway ticket system server is running.\n");
	
	while(1)
	{
		nsd = accept(sd,(struct sockaddr*)&cli,&clisz);
		if(nsd==-1)
		{
			perror("failed at accept system call\n");
			exit(EXIT_FAILURE);
		}
		printf("connection successful\n");
		printf("client ip address = %s and port number = %d \n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
				
		if(!fork())
		{
			close(sd);
			operations(nsd);
			printf("disconnected ip address = %s and port number = %d \n",inet_ntoa(cli.sin_addr),ntohs(cli.sin_port));
			exit(1);
		}
		else
		{
			close(nsd);
		}
	
	}
	return 0;	
}

