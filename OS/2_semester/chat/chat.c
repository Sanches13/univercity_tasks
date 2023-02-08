#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

#ifndef PORT
#  define PORT 9999
#endif

int work_flag = 1;
int firstRun = 1;
int correct = 1;

struct NamePacket {
 	char packet_type;
 	char name[64];
};

struct MessagePacket {
 	char packet_type;
 	char data[1024];
};

struct Node  {
	char name[64];
	char ip[16];
	struct Node* next;
	struct Node* prev;
};

struct Node* head;

struct Node* create_node(char* name, char* ip) {
	struct Node* tmp = (struct Node*)malloc(sizeof(struct Node));
	memcpy(tmp->name, name, 64);
	memcpy(tmp->ip, ip, 16);
	tmp->prev = tmp->next = NULL;
	return tmp;
}

void add_node(char* name, char* ip) {
	struct Node* tmp = create_node(name, ip);
	if(head == NULL) {
		head = tmp;
		return;
	}
	head->prev = tmp;
	tmp->next = head; 
	head = tmp;
}

void get_name(char* ip, char* name)
{
	struct Node* tmp = head;
	if(NULL == tmp) 
		return;

	while(NULL != tmp)
	{
		if(!strcmp(tmp->ip, ip))
		{
      			memcpy(name, tmp->name, 64);
      			return;
    		}
    		tmp = tmp->next;
  	}
  	return;
}

void* udp_server(void* none)
{
	int sock, count, ret;
	unsigned int addr_len;
	fd_set readfd;
	struct sockaddr_in addr;
	struct NamePacket* nick = (struct NamePacket*)malloc(sizeof(struct NamePacket));
	struct MessagePacket* msg = (struct MessagePacket*)malloc(sizeof(struct MessagePacket));
  
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if ( 0 > sock ) 
	{
		perror("socket");
		return NULL;
	}

	memset((void*)&addr, 0, addr_len);
	addr.sin_family = AF_INET;	
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons(PORT);
	addr_len = sizeof(addr);

	if ( bind(sock,(struct sockaddr*)&addr, addr_len) < 0) 
	{
		perror("bind");
		close(sock);
		return NULL;
	}

	while (work_flag) 
	{
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock+1, &readfd, NULL, NULL, 0);
		if (ret > 0) 
		{
			if (!firstRun && FD_ISSET(sock, &readfd)) 
			{
				count = recvfrom(sock, msg, sizeof(*msg), 0, (struct sockaddr*)&addr, &addr_len);   
        
				if(msg->packet_type == 'N')
				{
					char* tmp = (char*)malloc(64*sizeof(char));
					get_name(inet_ntoa(addr.sin_addr), tmp);
					if(tmp[0] != '\0')
					{
						free(tmp);
						tmp = NULL;
					} 
					else
					{
						nick = (struct NamePacket*)msg;
						add_node(nick->name, inet_ntoa(addr.sin_addr));
						fprintf(stderr, "\r%s joined the chat!\n", nick->name);
					}
				}
				else if (msg->packet_type == 'M')
				{
					char* tmp = (char*)malloc(64*sizeof(char));

					get_name(inet_ntoa(addr.sin_addr), tmp);
					fprintf(stderr, "\r%s: %s\n", tmp, msg->data);

					free(tmp);
					tmp = NULL;
				}
			}
		}
	}	
	close(sock);
	return (void*)NULL;
}

int main(int argc, char *argv[])
{
	int sock, yes = 1, i;
	pthread_t pid;
	struct sockaddr_in addr;
	int addr_len;
	int ret;
	char buffer[1024];
	struct NamePacket* nick = (struct NamePacket*)malloc(sizeof(struct NamePacket));
	struct MessagePacket* msg = (struct MessagePacket*)malloc(sizeof(struct MessagePacket));

	pthread_create(&pid, NULL,udp_server,NULL);
  
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		perror("sock");
		return -1;
	}

	ret = setsockopt(sock, SOL_SOCKET, SO_BROADCAST | SO_REUSEADDR, (char*)&yes, sizeof(yes));
	if (ret == -1) {
		perror("setsockopt");
		return 0;
	}

	addr_len = sizeof(struct sockaddr_in); 
	memset((void*)&addr, 0, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(PORT);

	while(work_flag)
	{
    		if(firstRun == 1){
	      		while(correct == 1)
	      		{
				fprintf(stderr, "Please, enter your name like this: <NICK> <your nick>\n");

				fgets(buffer, 64, stdin);
				ret = strlen(buffer);
				if (buffer[ret-1] == '\n') 
					buffer[ret-1] = 0;

	       			if((memcmp(buffer, "NICK ", 5) == 0))
		  				correct = 0;
	      		}

	      		firstRun = 0;
	      		char packet_type = 'N';
			nick->packet_type = packet_type;
			for(i = 5; i < 64; i++)
				nick->name[i-5] = buffer[i];

	      		sendto(sock, nick, sizeof(*nick), 0, (struct sockaddr*)&addr, addr_len);
			fprintf(stderr, "Hello, %s! Now you can write messages!\n", nick->name);
    		}

		else
		{
		        fgets(buffer, 1024, stdin);

		        ret = strlen(buffer);
		        if (buffer[ret-1] == '\n') 
				buffer[ret-1] = 0;

		        char packet_type = 'M';
			msg->packet_type = packet_type;
		        memcpy(msg->data, buffer, 1024);

		        sendto(sock, nick, sizeof(*nick), 0, (struct sockaddr*)&addr, addr_len);
		        sendto(sock, msg, sizeof(*msg), 0, (struct sockaddr*)&addr, addr_len);
    		}
  	}

  	free(nick);
	free(msg);

	pthread_join(pid, NULL);
	close(sock);
	return 0;
}
