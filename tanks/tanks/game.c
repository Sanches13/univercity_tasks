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

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int win = 0;
int work_flag = 1;

int sock, sock2;
unsigned int addr_len;
struct sockaddr_in addr;

Game game;
Window wind;

void init_window()
{
	initscr();
	keypad(stdscr, TRUE);
	curs_set(0);
	timeout(100);

	if(LINES < 40 || COLS < 100)
	{
		endwin();
		fprintf(stderr, "You need: LINES >= %d, COLS >= %d. Your size: LINES >= %d, COLS >= %d\n", 40, 100, LINES, COLS);
		exit(1);
	}

	if(has_colors() == false)
	{
		endwin();
		fprintf(stderr, "Colors not supported");
		exit(1);
	}
	start_color();

    	init_pair(1, COLOR_RED, COLOR_BLACK);
    
    	init_pair(2, COLOR_BLUE, COLOR_BLACK);

	init_pair(3, COLOR_CYAN, COLOR_CYAN);

	wind.lines = 40;
	wind.cols = 100;
	wind.zero_x = COLS/2 - wind.cols/2;
	wind.zero_y = LINES/2 - wind.lines/2;
}

void game_initialization(char *ip1, char *ip2)
{
	int i, j, l;

	memset(game.player[0].ip, 0, 16);
	memset(game.player[1].ip, 0, 16);

	game.player[0].ip_len = strlen(ip1);
	game.player[1].ip_len = strlen(ip2);

	memcpy(game.player[0].ip, ip1, game.player[0].ip_len);
	memcpy(game.player[1].ip, ip2, game.player[1].ip_len);

	for (i = 0; i < 2; i++)
	{
		// Init wheels
		game.player[i].hor_tank[0][0] = game.player[i].hor_tank[0][1] = game.player[i].hor_tank[0][2] =
		game.player[i].hor_tank[2][0] = game.player[i].hor_tank[2][1] = game.player[i].hor_tank[2][2] =
		game.player[i].vert_tank[0][0] = game.player[i].vert_tank[1][0] = game.player[i].vert_tank[2][0] =
		game.player[i].vert_tank[0][2] = game.player[i].vert_tank[1][2] = game.player[i].vert_tank[2][2] = '#';

		game.player[i].hor_tank[1][0] = game.player[i].hor_tank[1][2] =
		game.player[i].vert_tank[0][1] = game.player[i].vert_tank[2][1] = ' ';

		game.player[i].hor_tank[1][1] = game.player[i].vert_tank[1][1] = '*';

		game.player[i].bul_count = 0;
		game.player[i].id = i;
	}

	
	for (l = 0; l < 2; l++)
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[l].new_tank[i][j] = game.player[l].vert_tank[i][j];

	game.player[0].y = wind.zero_y + 20;
	game.player[0].x = wind.zero_x + (wind.cols)/4;
	game.player[0].y_tower = game.player[0].y - 1;
	game.player[0].x_tower = game.player[0].x;
	game.player[0].tower = '|';

	game.player[1].y = wind.zero_y + 20;
	game.player[1].x = wind.zero_x + 3*(wind.cols)/4;
	game.player[1].y_tower = game.player[1].y - 1;
	game.player[1].x_tower = game.player[1].x;
	game.player[1].tower = '|';

}

void print_borders()
{
	int heigth, width;
	pthread_mutex_lock(&mutex);

	attron(COLOR_PAIR(3));
	for (heigth = wind.zero_y; heigth <= wind.zero_y + wind.lines - 1; heigth++)
	{
		if (heigth == wind.zero_y || heigth == wind.zero_y + wind.lines - 1)
		{
			for (width = wind.zero_x; width <= wind.zero_x + wind.cols - 1; width++)
				mvaddch(heigth, width, '+');
		}
		else
		{
			mvaddch(heigth, wind.zero_x, '+');
			mvaddch(heigth, wind.zero_x + wind.cols - 1, '+');
		}
	}
	attroff(COLOR_PAIR(3));

	/*mvprintw(1, 1, "Player #1 Center of Tank: %d %d", game.player[0].y - wind.zero_y, game.player[0].x - wind.zero_x);
	mvprintw(2, 1, "Player #2 Center of Tank: %d %d", game.player[1].y - wind.zero_y, game.player[1].x - wind.zero_x);*/

	pthread_mutex_unlock(&mutex);
}

void print_tank(int id)
{
	//display each part of the tank separately

	pthread_mutex_lock(&mutex);

	attron(COLOR_PAIR(id+1));
	mvaddch(game.player[id].y - 1, game.player[id].x - 1, game.player[id].new_tank[0][0]);
	mvaddch(game.player[id].y - 1, game.player[id].x, game.player[id].new_tank[0][1]);
	mvaddch(game.player[id].y - 1, game.player[id].x + 1, game.player[id].new_tank[0][2]);
	mvaddch(game.player[id].y, game.player[id].x - 1, game.player[id].new_tank[1][0]);
	mvaddch(game.player[id].y, game.player[id].x, game.player[id].new_tank[1][1]);
	mvaddch(game.player[id].y, game.player[id].x + 1, game.player[id].new_tank[1][2]);
	mvaddch(game.player[id].y + 1, game.player[id].x - 1, game.player[id].new_tank[2][0]);
	mvaddch(game.player[id].y + 1, game.player[id].x, game.player[id].new_tank[2][1]);
	mvaddch(game.player[id].y + 1, game.player[id].x + 1, game.player[id].new_tank[2][2]);

	mvaddch(game.player[id].y_tower, game.player[id].x_tower, game.player[id].tower);

	attroff(COLOR_PAIR(id+1));
	pthread_mutex_unlock(&mutex);
}

void print_bullet(int id)
{
	int i;

	attron(COLOR_PAIR(id+1));
	for (i = 0; i < 4; i++)
	{
		if(game.player[id].bul_alive[i] == true)
		{
			pthread_mutex_lock(&mutex);
			mvaddch(game.player[id].y_bul[i], game.player[id].x_bul[i], 'o');
			pthread_mutex_unlock(&mutex);
		}
	}
	attroff(COLOR_PAIR(id+1));
}

int check_hit(int bullet_id, int player_id)
{
	int i, j;
	for(i = game.player[(player_id + 1) % 2].x - 1; i <= game.player[(player_id + 1) % 2].x + 1; i++)
		for(j = game.player[(player_id + 1) % 2].y - 1; j <= game.player[(player_id + 1) % 2].y + 1; j++)
		{
			if(game.player[player_id].y_bul[bullet_id] == j &&
			   game.player[player_id].x_bul[bullet_id] == i)
				return 1;
		}
	return 0;
}

void* bullet_spawn(void *data)
{
	Player *player = (Player*)data;
	//check how many bullets are on the field now

	player->bul_count += 1;

	int i = 0, j, res, first_flag = 0;
	for(j = 0; j < 4; j++)
	{
		if(!player->bul_alive[j])
		{
			i = j;
			break;
		}
		
	}

	player->bul_alive[i] = true;

	switch (player->tower)
	{
	case '|':
		if (player->y_tower < player->y)
		{
			player->y_bul[i] = player->y_tower - 1;
			player->bul_tower[i] = 1;
		}

		else
		{
			player->y_bul[i] = player->y_tower + 1;
			player->bul_tower[i] = 2;
		}

		player->x_bul[i] = player->x_tower;

		break;

	case '-':
		if (player->x_tower < player->x)
		{
			player->x_bul[i] = player->x_tower - 1;
			player->bul_tower[i] = 3;
		}
		else
		{
			player->x_bul[i] = player->x_tower + 1;
			player->bul_tower[i] = 4;
		}
		player->y_bul[i] = player->y_tower;

		break;
	case '/':
		if (player->y_tower < player->y)
		{
			player->y_bul[i] = player->y_tower - 1;
			player->x_bul[i] = player->x_tower + 1;
			player->bul_tower[i] = 5;
		}
		else
		{
			player->y_bul[i] = player->y_tower + 1;
			player->x_bul[i] = player->x_tower - 1;
			player->bul_tower[i] = 6;
		}
			
		break;

	case 0x5C: // '\'
		if (player->y_tower < player->y)
		{
			player->y_bul[i] = player->y_tower - 1;
			player->x_bul[i] = player->x_tower - 1;
			player->bul_tower[i] = 7;
		}
		else
		{
			player->y_bul[i] = player->y_tower + 1;
			player->x_bul[i] = player->x_tower + 1;
			player->bul_tower[i] = 8;
		}
			
		break;
	}

	while((player->y_bul[i] > wind.zero_y) && 
	      (player->y_bul[i] < wind.zero_y + wind.lines - 1) && 
	      (player->x_bul[i] > wind.zero_x) && 
	      (player->x_bul[i] < wind.zero_x + wind.cols - 1))
	{
		//pthread_mutex_lock(&mutex);

		res = check_hit(i, player->id);
		if(res == 1)
		{
			win = player->id + 1;
			//pthread_mutex_unlock(&mutex);
			break;
		}

		if(first_flag == 0)
		{
			first_flag = 1;
			//pthread_mutex_unlock(&mutex);
			usleep(50000);
			continue;
		}

		switch (player->bul_tower[i])
			{
			case 1:
				player->y_bul[i]--;
				break;
			case 2:
				player->y_bul[i]++;
				break;
			case 3:
				player->x_bul[i]--;
				break;
			case 4:
				player->x_bul[i]++;
				break;
			case 5:
				player->y_bul[i]--;
				player->x_bul[i]++;
				break;
			case 6:
				player->y_bul[i]++;
				player->x_bul[i]--;
				break;
			case 7:
				player->y_bul[i]--;
				player->x_bul[i]--;
				break;
			case 8:
				player->y_bul[i]++;
				player->x_bul[i]++;
				break;
			}
		//pthread_mutex_unlock(&mutex);
		usleep(50000);

	}

	pthread_mutex_lock(&mutex);
	player->bul_count--;
	player->bul_alive[i] = false;
	pthread_mutex_unlock(&mutex);
	return (void *)NULL;

}

int check_location()
{
	int i, j, k, l;

	for (i = game.player[0].x - 1; i <= game.player[0].x + 1; i++)
	{
		for (j = game.player[0].y - 1; j <= game.player[0].y + 1; j++)
		{
			for (l = game.player[1].x - 1; l <= game.player[1].x + 1; l++)
			{
				for (k = game.player[1].y - 1; k <= game.player[1].y + 1; k++)
				{
					if (i == l && j == k)
						return 1;
				}
			}
		}
	}
	return 0;
}

void key_check(int ch, int id)
{
	pthread_mutex_lock(&mutex);
	int i, j;
	pthread_t bullet;

	switch (ch)
	{
		//by pressing the arrows, change the coordinates
		//of the center and the turret of the tank

	case KEY_UP:

		game.player[id].y--;
		game.player[id].y_tower--;

		//if the center coordinate now lies on one of the screen borders
		//move the tank to the opposite border

		if (game.player[id].y == wind.zero_y + 1 || ((check_location()) == 1))
		{
			game.player[id].y++;
			game.player[id].y_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[id].new_tank[i][j] = game.player[id].vert_tank[i][j];

		break;

	case KEY_DOWN:

		game.player[id].y++;
		game.player[id].y_tower++;
		
		if (game.player[id].y == wind.zero_y + wind.lines - 1 - 1 || ((check_location()) == 1))
		{
			game.player[id].y--;
			game.player[id].y_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[id].new_tank[i][j] = game.player[id].vert_tank[i][j];

		break;

	case KEY_RIGHT:
		game.player[id].x++;
		game.player[id].x_tower++;
	
		if (game.player[id].x == wind.zero_x + wind.cols - 1 - 1 || ((check_location()) == 1))
		{
			game.player[id].x--;
			game.player[id].x_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[id].new_tank[i][j] = game.player[id].hor_tank[i][j];

		break;

	case KEY_LEFT:
		game.player[id].x--;
		game.player[id].x_tower--;
		
		if (game.player[id].x == wind.zero_x + 1 || ((check_location()) == 1))
		{
			game.player[id].x++;
			game.player[id].x_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[id].new_tank[i][j] = game.player[id].hor_tank[i][j];

		break;
		//by pressing the 'q' key, turn the turret up and to the right
		//change the coordinates of the turret
	case 'q':
		game.player[id].tower = 0x5C; // '\'
		game.player[id].y_tower = game.player[id].y - 1;
		game.player[id].x_tower = game.player[id].x - 1;
		break;
		//by pressing the 'w' key, turn the turret up
	case 'w':
		game.player[id].tower = '|';
		game.player[id].y_tower = game.player[id].y - 1;
		game.player[id].x_tower = game.player[id].x;
		break;
		//by pressing the 'e' key, turn the turret up and to the left
	case 'e':
		game.player[id].tower = 0x2F; // '/'
		game.player[id].y_tower = game.player[id].y - 1;
		game.player[id].x_tower = game.player[id].x + 1;
		break;
		//by pressing the 'a' key, turn the turret left

	case 'a':
		game.player[id].tower = '-';
		game.player[id].y_tower = game.player[id].y;
		game.player[id].x_tower = game.player[id].x - 1;
		break;
		//by pressing the 'd' key, turn the turret right
	case 'd':
		game.player[id].tower = '-';
		game.player[id].y_tower = game.player[id].y;
		game.player[id].x_tower = game.player[id].x + 1;
		break;
		//by pressing the 'q' key, turn the turret down and to the left

	case 'z':
		game.player[id].tower = 0x2F; // '/'
		game.player[id].y_tower = game.player[id].y + 1;
		game.player[id].x_tower = game.player[id].x - 1;
		break;
		//by pressing the 'q' key, turn the turret down

	case 'x':
		game.player[id].tower = '|';
		game.player[id].y_tower = game.player[id].y + 1;
		game.player[id].x_tower = game.player[id].x;
		break;
		//by pressing the 'q' key, turn the turret down and to the right

	case 'c':
		game.player[id].tower = 0x5C; // '\'
		game.player[id].y_tower = game.player[id].y + 1;
		game.player[id].x_tower = game.player[id].x + 1;
		break;

	case ' ':
		if(game.player[id].bul_count >= 4)
			break;
		else
			pthread_create(&bullet, NULL, bullet_spawn, (void *)&game.player[id]);
		break;

	case 'k':
		win = 3;
		break;
	}
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
	if (argc != 3)
	{
		printf("Please, use format <.exe> <ip_1> <ip_2>\n");
		return -1;
	}

	if((strcmp(argv[1], argv[2])) == 0)
	{
		printf("You entered equal ip\n");
		return -1;
	}

	int ch;
	int yes = 1, ret, i;

	pthread_t pid;

	init_window();

	// Init game
	game_initialization(argv[1], argv[2]);

	// Create udp server thread
	pthread_create(&pid, NULL, udp_server, NULL);

	sock2 = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock2 < 0)
	{
		perror("sock");
		return -1;
	}

	ret = setsockopt(sock2, SOL_SOCKET, SO_BROADCAST, (char *)&yes, sizeof(yes));
	if (ret == -1)
	{
		perror("setsockopt");
		return 0;
	}

#ifdef TEST_FLAG
	char *devname = "tap0";
	ret = setsockopt(sock2, SOL_SOCKET, SO_BINDTODEVICE, devname, strlen(devname));
	if (ret == -1)
	{
		perror("setsockopt");
		return 0;
	}
#endif

	memset((void *)&addr, 0, addr_len);
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
	addr.sin_port = htons(PORT);
	addr_len = sizeof(struct sockaddr_in);

	while (work_flag && win == 0)
	{
		ch = getch();

		pthread_mutex_lock(&mutex);
		erase();
		pthread_mutex_unlock(&mutex);

		for(i = 0; i < 2; i++)
			print_tank(i);
		
		for(i = 0; i < 2; i++)
			print_bullet(i);

		print_borders();

		pthread_mutex_lock(&mutex);
		refresh();
		pthread_mutex_unlock(&mutex);

		if(ch != ERR)		
			sendto(sock2, &ch, sizeof(int), 0, (struct sockaddr *)&addr, addr_len);

		usleep(1000000/120);

	}

	char buf[256];
	char ip[16];
	memset(ip, 0, 16);
	if(win == 1)
	{
		memcpy(ip, game.player[0].ip, game.player[0].ip_len);
		sprintf(buf, "Player #1 with ip %s win in game!", ip);
		erase();
		mvprintw(1, (COLS - strlen(buf)) / 2, buf);
		sprintf(buf, "Press ENTER to exit");
		mvprintw(2, (COLS - strlen(buf)) / 2, buf);
		refresh();
		while((ch = getch()) != 0x0A);
	}

	else if(win == 2)
	{
		memcpy(ip, game.player[1].ip, game.player[1].ip_len);
		sprintf(buf, "Player #2 with ip %s win in game!", ip);
		erase();
		mvprintw(1, (COLS - strlen(buf)) / 2, buf);
		sprintf(buf, "Press ENTER to exit");
		mvprintw(2, (COLS - strlen(buf)) / 2, buf);
		refresh();
		while((ch = getch()) != 0x0A);
	}

	else
	{
		erase();
		sprintf(buf, "One of players has finished the game");
		mvprintw(1, (COLS - strlen(buf)) / 2, buf);
		sprintf(buf, "Press ENTER to exit");
		mvprintw(2, (COLS - strlen(buf)) / 2, buf);
		refresh();
		while((ch = getch()) != 0x0A);
	}

	pthread_cancel(pid);
	pthread_join(pid, NULL);
	close(sock2);
	endwin();

	return 0;
}
