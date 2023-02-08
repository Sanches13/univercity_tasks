#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/epoll.h>
#include <netdb.h>
#include <malloc.h>

int main(int argc, char *argv[])
{
	if(argc != 3)
	{
		printf("Please, use format .exe <ip> <port> ");
		return 0;
	}
	int sd;
	struct sockaddr_in addr;
	sd = socket(AF_INET, SOCK_STREAM, 0);
	if(sd < 0)
	{
		perror("SD");
		return 0;
	}

	addr.sin_family = AF_INET;
	addr.sin_port = htons(atoi(argv[2]));
	addr.sin_addr.s_addr = inet_addr(argv[1]);

	if(bind(sd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("Bind");
		return 0;
	}

	if(listen(sd, 5) == -1)
	{
		perror("Listen");
		return 0;
	}

	int epoll_fd = epoll_create(5);
	if(epoll_fd < 0)
	{
		perror("epoll_fd\n");
		return 0;
	}

    int cs;
	struct sockaddr_storage client_addr;
	socklen_t addr_size = sizeof client_addr;

	struct epoll_event evlist[64];
	while(1)
	{
		if((cs = accept(sd, (struct sockaddr *)&client_addr, &addr_size)) == -1)
			{
			    perror("accept\n");
			    close(sd);
			    return -1;
			}	
	
	struct epoll_event ev;
	ev.data.fd = cs;
	ev.events = EPOLLIN;

	if(epoll_ctl(epoll_fd, EPOLL_CTL_ADD, cs, &ev) == -1)
	{
		perror("epoll_stl\n");
	}

	int ready = epoll_wait(epoll_fd, evlist, 1, -1);
	if(ready == -1)
	{
		perror("epoll_wait\n");
	}
	int j;
	for(j = 0; j < ready; j++)
	{
                int n1, m1, i;
		char *fname, *data, *p, n_count[4], m_count[4];
                int n = read(evlist[j].data.fd, n_count, 4);
                int m = read(evlist[j].data.fd, m_count, 4);
                n1 = atoi(n_count);
                m1 = atoi(m_count);
                fname = (char*)malloc(n1*sizeof(char));
                data = (char*)malloc(m1*sizeof(char));
		int fname_count = read(evlist[j].data.fd, fname, n1+1);
		int data_count = read(evlist[j].data.fd, data, m1+1);      
                if(n == -1)
		{
			perror("n1_count");
		}
                if(m == -1)
		{
			perror("m1_count");
		}
		if(fname_count == -1)
		{
			perror("fname_count");
		}
		if(data_count == -1)
		{
			perror("data_count");
		}
                        p = (char*)malloc((n1)*sizeof(char));
                        for(i = 0; i < n1-1; i++)
                        {
                               p[i] = fname[i];
                        }
                        printf("Number of name bytes: %d\n", n1);
                        printf("Number of name bytes: %d\n", m1);
                        printf("File name: %s\n", fname);
                        printf("Data: %s\n", data);
			if(n1<=0 || m1 < 0)
				continue;
			else
			{
                                FILE *fp = fopen(p, "w");
			        if(fp == NULL)
			        {
				        perror("file\n");
				        return -1;
			        }
                                fputs(data, fp);
			        fclose(fp);
			}
                        free(fname);
                        free(data);
                        free(p);
	}
	close(cs);
	}
	close(sd);
	return 0;
}
