#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	if(argc != 4)
	{
		printf("Use format .exe <ip> <port> <file_name>");
		return -1;
	}
	FILE *fp;
	fp = fopen(argv[3], "r");
        if(fp == NULL)
        {
             printf("File\n");
             return -1;
        }
	char buf[1024], n_count[4], m_count[4];
	fgets(buf, 1024, fp);
	int sd, cd, n, m;
	struct sockaddr_in addr;
        sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0)
	{
		perror("sd\n");
		return 0;
	}

        addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

        cd = connect(sd, (struct sockaddr *)&addr, sizeof(addr));
	if(cd < 0)
	{
		perror("Connect");
		return 0;
        } 
  
        n = strlen(argv[3])+1;
        m = strlen(buf);
        sprintf(n_count, "%d", n);
        sprintf(m_count, "%d", m); 
        send(sd, n_count, strlen(n_count)+1, 0);
        sleep(1);
        send(sd, m_count, strlen(m_count)+1, 0);
        sleep(1);
	send(sd, argv[3], n+1, 0);
	sleep(1);
        send(sd, buf, m+1, 0);
	return 0;
}
