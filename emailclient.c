#include <netdb.h> 
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h>
#include <arpa/inet.h>
#define MAX 80 
#define MAX_USER_LENGTH 20
#define PORT 8080 
#define SA struct sockaddr 

void usercommand(int sockfd){
	char user_id[MAX_USER_LENGTH];
	// read user_id
	scanf("%s",user_id);
	write(sockfd,user_id,sizeof(user_id));
	
	char buff[MAX]; 
	bzero(buff,sizeof(buff));
	read(sockfd,buff,sizeof(buff)); // initial buffer.
	
	if(strcmp(buff,"username does not exists") == 0){
			// user doeas not exists.
			printf("%s\n",buff);
	}
	
	else{
		printf("%s",buff); // user and his mails. sucess
		
		char command[MAX];
		char messagebuffer[200];
		while(1 < 2){
			printf("Sub-Prompt-%s> ",user_id);
			scanf("%s",command);
			if(strcmp(command,"Done") == 0){
				bzero(command,sizeof(command));
				strcpy(command,"DONEU");
				write(sockfd,command,sizeof(command));

				break;
			}
			
			else if(strcmp(command,"Read") == 0){
				bzero(command,sizeof(command));
				strcpy(command,"READM");
				write(sockfd,command,sizeof(command));
	
				bzero(messagebuffer,sizeof(messagebuffer));			
				read(sockfd,messagebuffer,sizeof(messagebuffer));
				printf("%s\n",messagebuffer);
			}
			
			else if(strcmp(command,"Delete") == 0){
				bzero(command,sizeof(command));
				strcpy(command,"DELM");
				write(sockfd,command,sizeof(command));
				bzero(messagebuffer,sizeof(messagebuffer));
				
				char tempm[MAX];
				read(sockfd,tempm,sizeof(tempm));
				printf("%s\n",tempm);
			}
			
			else if(strcmp(command,"Send") == 0){
				char rec_id[MAX_USER_LENGTH];
				printf("SEND in client\n");
				bzero(command,sizeof(command));
				strcpy(command,"SEND");
				write(sockfd,command,sizeof(command));
				printf("Wrote %s\n",command);
				scanf("%s",rec_id);
				write(sockfd,rec_id,sizeof(rec_id)); // send username.
				
				printf("receiver id is %s\n",rec_id);
				
				bzero(buff,sizeof(buff));
				read(sockfd,buff,sizeof(buff));
				printf("%s",buff);
				if(strcmp(buff,"sucess") == 0){
					printf("Entered Sucess\n");
					char messagebuffer[200];
					char c;
					int n=0,flg=0;
					c = getchar();
					//c = getchar();
					while(flg ==0){
						messagebuffer[n] = c;
						n++;
						if(!(n >=3 && messagebuffer[n-1] == '#' && messagebuffer[n-2] == '#' && messagebuffer[n-3] == '#'))
						{
							c = getchar();
						}
						else{
							flg = 1;
						}
					}
					messagebuffer[n] = '\0'; // read input to messagebuffer.
					
					write(sockfd,messagebuffer,sizeof(messagebuffer));
					
				}
				
				else{
					// userid not present.
					printf("%s\n",buff);
				}
				
			}
			
			else{
				char messagebuffer[200];
					char c;
					int n=0;
					c = getchar();
					messagebuffer[n]=c;
					while(messagebuffer[n]!='\n')
					{ c= getchar();
						messagebuffer[n]=c;
					}
				printf("invalid command\n");
					continue;
			}
		}
	}
}

void command_processor(int sockfd) 
{ 
	char buff[MAX]; 
	int n; 
	for (;;) { 
		bzero(buff, sizeof(buff)); 
		printf("Main-Prompt> "); 
		scanf("%s",buff);
		if(strcmp("Adduser",buff) == 0){
				// printf("Entered ADDU");
				bzero(buff,sizeof(buff));
				strcpy(buff,"ADDU");
				write(sockfd,buff,sizeof(buff)); // gives addu to server
				bzero(buff, sizeof(buff)); 
				
				scanf("%s",buff); // gives id to server
				write(sockfd,buff,sizeof(buff));
				bzero(buff, sizeof(buff)); 
				
				read(sockfd,buff,sizeof(buff)); // get reply from execution of following command wheather user created or exists.
				printf("%s\n",buff);
				bzero(buff, sizeof(buff));
				
		}
		
		else if(strcmp(buff,"Listusers") == 0){
				bzero(buff,sizeof(buff));
				strcpy(buff,"LSTU");
				write(sockfd,buff,sizeof(buff));
				bzero(buff, sizeof(buff)); 
				
				read(sockfd,buff,sizeof(buff)); // get reply from execution of following command.
				printf("%s\n",buff);
				bzero(buff, sizeof(buff));
		}
		
		else if(strcmp(buff,"SetUser") == 0){
				bzero(buff,sizeof(buff));
				strcpy(buff,"USER");
				write(sockfd,buff,sizeof(buff));
				bzero(buff,sizeof(buff));
				
				usercommand(sockfd);
		}
		
		else if(strcmp("Quit",buff) == 0){
			bzero(buff,sizeof(buff));
			strcpy(buff,"QUIT");
			write(sockfd,buff,sizeof(buff)); // gives addu
			bzero(buff, sizeof(buff)); 
			break;
		}
		
		else{
			printf("Invalid command read again\n");
		}
	} 
} 

int main(int argc,char* argv[]) 
{ 
	int sockfd, connfd; 
	struct sockaddr_in servaddr, cli; 

	// socket create and varification 
	sockfd = socket(AF_INET, SOCK_STREAM, 0); 
	if (sockfd == -1) { 
		printf("socket creation failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully created..\n"); 
	bzero(&servaddr, sizeof(servaddr)); 

	// assign IP, PORT 

	int V_Port;
	sscanf(argv[2],"%d",&V_Port);	

	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = inet_addr(argv[1]); 
	servaddr.sin_port = htons(V_Port); 

	// connect the client socket to server socket 
	if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) { 
		printf("connection with the server failed...\n"); 
		exit(0); 
	} 
	else
		printf("connected to the server..\n"); 

	// function for chat 
	command_processor(sockfd); 

	// close the socket 
	close(sockfd); 
}
