#include <pthread.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/select.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "game.h"
#include "server.h"

void *udp_server(void *data)
{
	int sock, ret, i, key;
	unsigned int addr_len;

	fd_set readfd;
	struct sockaddr_in addr;

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (0 > sock)
	{
		perror("socket");
		return NULL;
	}

#ifdef TEST_FLAG
	char *devname = "tap0";
	ret = setsockopt(sock, SOL_SOCKET, SO_BINDTODEVICE, devname, strlen(devname));
	if (ret == -1)
	{
		perror("setsockopt");
		return 0;
	}
#endif

	addr_len = sizeof(struct sockaddr_in);
	memset((void *)&addr, 0, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(PORT);

	if (bind(sock, (struct sockaddr *)&addr, addr_len) < 0)
	{
		perror("bind");
		close(sock);
		return NULL;
	}
    
	while (work_flag && win == 0)
	{
		FD_ZERO(&readfd);
		FD_SET(sock, &readfd);

		ret = select(sock + 1, &readfd, NULL, NULL, 0);
		if (ret > 0)
		{
			if (FD_ISSET(sock, &readfd))
			{
				recvfrom(sock, &key, sizeof(int), 0, (struct sockaddr *)&addr, &addr_len);
                    		
				for (i = 0; i < 2; i++)
				{
					if ((memcmp(game.player[i].ip, inet_ntoa(addr.sin_addr), game.player[i].ip_len)) == 0)
						key_check(key, i);
				}
	
			}
		}
	}
	close(sock);
	return (void *)NULL;
}
