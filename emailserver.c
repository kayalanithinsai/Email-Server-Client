#include <stdio.h> 
#include <netdb.h> 
#include <stdlib.h>
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <dirent.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<time.h>
#define MAX 80 
#define PORT 8080 
#define SA struct sockaddr 
#define MAX_USER_LENGTH 20
#define MAX_USER_LIST 100
struct User{
	char name[MAX_USER_LENGTH];
	int number_of_mails;
};

struct User user_list[MAX_USER_LIST];
// To point the user to which it should be added 
int user_pointer =0; 

int check(char user_id[],int flg,int sockfd){
	for(int i=0;i<user_pointer;i++){
		if(strcmp(user_list[i].name,user_id) == 0){
			if(flg == 1){
				char buff[MAX];
				bzero(buff,sizeof(buff));
				snprintf(buff,MAX,"User with the Userid %s exists and has %d mails in the spoolfie\n",user_id,user_list[i].number_of_mails);
				write(sockfd,buff,sizeof(buff));
			}
			return i+1;
		}
	}
	return 0;
}

void LSTU(int sockfd){
		char buff[MAX] = "";
		if(user_pointer == 0){
			snprintf(buff,MAX,"No users Yet\n");
		}
		else{
			for(int i=0;i<user_pointer-1;i++){
				// printf("%s ",user_list[i].name);
				strcat(buff,user_list[i].name);
				strcat(buff," ");
			}
			strcat(buff,user_list[user_pointer-1].name);
			// strcat(buff,"\n\0");
		}
		write(sockfd,buff,sizeof(buff));
		
}

void ADDU(int sockfd){
	char user_id[MAX_USER_LENGTH];
	// scanf("%s",user_id);
	
	bzero(user_id,sizeof(user_id));
	read(sockfd,user_id,sizeof(user_id));
	// printf("The id from client is %s\n",user_id);
	
	char buff[MAX];
	if(check(user_id,0,sockfd) != 0){
		
		strcpy(buff,"Userid Already present\n");
		
	}
	
	else{
		// add new user to user_list
		strcpy(user_list[user_pointer].name,user_id);
		user_list[user_pointer].number_of_mails =0;
		user_pointer++;
		
		// printf("Created ");
		// printf("%s\n",user_list[user_pointer-1].name);
		
		strcpy(buff,"Created ");
		strcat(buff,user_list[user_pointer-1].name);
		
		FILE *fptr;
		fptr = fopen(user_id,"w");
		fclose(fptr);
	}
	
	write(sockfd,buff,sizeof(buff));
	bzero(buff,sizeof(buff));
	
}


void usercommand(int sockfd){
	
	char current_user_id[MAX_USER_LENGTH],buff[MAX];
	bzero(current_user_id,sizeof(current_user_id));
	read(sockfd,current_user_id,sizeof(current_user_id));
	int check_num = check(current_user_id,1,sockfd);
	
	if(check_num == 0){
		strcpy(buff,"username does not exists");
		write(sockfd,buff,sizeof(buff));
	}
	
	else{
		int currentptr=0;
		
		while(1 < 2){
			char buff[MAX];
			
			bzero(buff,sizeof(buff));
			read(sockfd,buff,sizeof(buff));
			// scanf("read buffer is %s",buff);
			
			if(strcmp(buff,"READM") == 0){
				
				int num_mails = user_list[check_num-1].number_of_mails;
				
				if(num_mails == 0){
					char tempm[MAX];
					strcpy(tempm,"No More Mail");
					write(sockfd,tempm,sizeof(tempm));
					continue;
				}
				
				else if(currentptr == num_mails){
					currentptr =0;
				}
				
				// open file and precess till currentptr number of mails.
				FILE *curr_fptr;
				// FILE *temp_fptr;
				
				curr_fptr = fopen(current_user_id,"r");
				// temp_fptr = fopen("temp","w");
				
				char c;
				char messagebuffer[200];
				int cnt =0,n=0,flg=0;
				for(int i=0;i<currentptr;i++){
					n=0;
					if(i != 0)
					c = fgetc(curr_fptr);
					c = fgetc(curr_fptr);
					flg=0;
					while(flg ==0){
						messagebuffer[n] = c;
						n++;
						if(!(n >=3 && messagebuffer[n-1] == '#' && messagebuffer[n-2] == '#' && messagebuffer[n-3] == '#'))
						{
							c = fgetc(curr_fptr);
						}
						else{
							flg = 1;
						}
					}
					messagebuffer[n] = '\0';
				}
				n=0;
				flg=0;
				if(currentptr != 0)
				c = fgetc(curr_fptr);
				c = fgetc(curr_fptr);
				while(flg ==0){
					messagebuffer[n] = c;
					n++;
					if(!(n >=3 && messagebuffer[n-1] == '#' && messagebuffer[n-2] == '#' && messagebuffer[n-3] == '#'))
					{
						c = fgetc(curr_fptr);
					}
					else{
						flg = 1;
					}
				}
				messagebuffer[n] = '\0';
				
				// printf("%s",messagebuffer);
				write(sockfd,messagebuffer,sizeof(messagebuffer));
				
				// fputs(messagebuffer,temp_fptr);
				// fclose(temp_fptr);
				fclose(curr_fptr);
				currentptr++;
			}
			
			else if(strcmp(buff,"DELM") == 0){
				int num_mails = user_list[check_num-1].number_of_mails;
				
				if(num_mails == 0){
					char tempm[MAX];
					
					strcpy(tempm,"No More Mail");
					write(sockfd,tempm,sizeof(tempm));
					continue;
				}
				
				else if(currentptr == num_mails){
					currentptr =0;
				}
				
				FILE *curr_fptr;
				FILE *fptr2;
				
				curr_fptr = fopen(current_user_id,"r");
				fptr2 = fopen("temp.txt","w");
				
				int target = currentptr;
				for(int i=0;i<num_mails;i++){
					int n=0;
					char messagebuffer[200],c;
					int cnt=0,flg=0;
					if(i != 0)
					c = fgetc(curr_fptr);
					c = fgetc(curr_fptr);
					while(flg ==0){
						messagebuffer[n] = c;
						n++;
						if(!(n >=3 && messagebuffer[n-1] == '#' && messagebuffer[n-2] == '#' && messagebuffer[n-3] == '#'))
						{
							c = fgetc(curr_fptr);
						}
						else{
							flg = 1;
						}
					}
					messagebuffer[n] = '\0';
					if(i != target)
					fputs(messagebuffer,fptr2);
				}
				fclose(curr_fptr);
				fclose(fptr2);
				user_list[check_num-1].number_of_mails--;
				if(remove(current_user_id) == 0){
					char tempm[MAX];
					strcpy(tempm,"Sucessfully removed");
					write(sockfd,tempm,sizeof(tempm));
				}
				else{
					printf("unable to delete.\n");
				}
				if(rename("temp.txt",current_user_id) == 0){
					printf("sucessfully changed.\n");
				}
				else{
					printf("unable to change.\n");
				}
			}
			
			else if(strcmp(buff,"SEND") == 0){
				char receiver_user_id[MAX_USER_LENGTH];
				read(sockfd,receiver_user_id,sizeof(receiver_user_id));
				// check for receiver user-id
				// printf("receiver id is %s\n",receiver_user_id);
				int check_num = check(receiver_user_id,0,sockfd) ;
				if(check_num == 0){
					char tempm[MAX];
					strcpy(tempm,"Userid is invalid ..");
					write(sockfd,tempm,sizeof(tempm));
					// printf("Userid is invalid ..\n");
					continue;
				}
				else{
					char tempm[MAX];
					strcpy(tempm,"sucess");
					write(sockfd,tempm,sizeof(tempm));
					FILE *fptr;
					fptr = fopen(receiver_user_id,"a+");
					// open a receiver file and append the message at end.
					fputs("From: ",fptr);
					fputs(current_user_id,fptr);
					
					fputs("\nTo: ",fptr);
					fputs(receiver_user_id,fptr);

					time_t t;
					time(&t);
					fputs("\nDate: ",fptr);
					fputs(ctime(&t),fptr);					

					fputs("Contents: ",fptr);
					printf("over subject");
					char messagebuffer[200];
					char c;
					int n=0,cnt=0;
					
					read(sockfd,messagebuffer,sizeof(messagebuffer));
					// printf("read subject as \n%s\n",messagebuffer);
					fputs(messagebuffer,fptr);
					fputs("\n",fptr);
					fclose(fptr);
					user_list[check_num-1].number_of_mails++;
				}
			}
			
			else if(strcmp(buff,"DONEU") == 0){
				break;
			}
			
			else{
				continue;
			}
			
		}
	}
	
}


// Function designed for chat between client and server. 
void command_processor(int sockfd) 
{
	char buff[MAX]; 
	while(1 < 2){
		bzero(buff,sizeof(buff));
		read(sockfd,buff,sizeof(buff)); // reads command.
		printf("BUFFER IS %s \n",buff);
		if(strcmp("ADDU",buff) == 0){
			
			ADDU(sockfd);
			
		}
		else if(strcmp("LSTU",buff) == 0){
			LSTU(sockfd);
		}
		else if(strcmp("QUIT",buff) == 0){
			break;
		}
		else if(strcmp("\0",buff) == 0){
			// printf("handled\n");
			continue;
		}
		else if(strcmp("USER",buff) == 0){
			usercommand(sockfd);
		}
		else{
			bzero(buff,sizeof(buff));
			strcpy(buff,"Test failed");
			write(sockfd,buff,sizeof(buff));
			bzero(buff,MAX);
		}

	}
} 

// Driver function 
int main(int argc,char* argv[]) 
{ 
	int sockfd, connfd, len; 
	struct sockaddr_in servaddr, cli; 

	// socket create and verification 
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
	sscanf(argv[1],"%d",&V_Port);
	
	servaddr.sin_family = AF_INET; 
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY); 
	servaddr.sin_port = htons(V_Port); 

	// Binding newly created socket to given IP and verification 
	if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
		printf("socket bind failed...\n"); 
		exit(0); 
	} 
	else
		printf("Socket successfully binded..\n"); 

	// Now server is ready to listen and verification 
	if ((listen(sockfd, 5)) != 0) { 
		printf("Listen failed...\n"); 
		exit(0); 
	} 
	else
		printf("Server listening..\n"); 
	len = sizeof(cli); 

	// Accept the data packet from client and verification 
	connfd = accept(sockfd, (SA*)&cli, &len); 
	if (connfd < 0) { 
		printf("server acccept failed...\n"); 
		exit(0); 
	} 
	else
		printf("server acccept the client...\n"); 

	// Function for chatting between client and server 
	command_processor(connfd); 

	// After chatting close the socket 
	close(sockfd); 
} 
