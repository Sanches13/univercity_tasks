#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "game.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int win = 0;

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

void game_initialization()
{
	int i, j, l;

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
				return (player_id + 1);
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
		if(res != 0)
		{
			win = res;
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

void key_check(int ch)
{
	pthread_mutex_lock(&mutex);
	int i, j;
	pthread_t bullet;

	switch (ch)
	{
	case KEY_UP:

		game.player[1].y--;
		game.player[1].y_tower--;

		if (game.player[1].y == wind.zero_y + 1 || ((check_location()) == 1))
		{
			game.player[1].y++;
			game.player[1].y_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[1].new_tank[i][j] = game.player[1].vert_tank[i][j];

		break;

	case KEY_DOWN:

		game.player[1].y++;
		game.player[1].y_tower++;
		
		if (game.player[1].y == wind.zero_y + wind.lines - 1 - 1 || ((check_location()) == 1))
		{
			game.player[1].y--;
			game.player[1].y_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[1].new_tank[i][j] = game.player[1].vert_tank[i][j];

		break;

	case KEY_RIGHT:
		game.player[1].x++;
		game.player[1].x_tower++;
	
		if (game.player[1].x == wind.zero_x + wind.cols - 1 - 1 || ((check_location()) == 1))
		{
			game.player[1].x--;
			game.player[1].x_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[1].new_tank[i][j] = game.player[1].hor_tank[i][j];

		break;

	case KEY_LEFT:
		game.player[1].x--;
		game.player[1].x_tower--;
		
		if (game.player[1].x == wind.zero_x + 1 || ((check_location()) == 1))
		{
			game.player[1].x++;
			game.player[1].x_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[1].new_tank[i][j] = game.player[1].hor_tank[i][j];

		break;

	case '7':
		game.player[1].tower = 0x5C; // '\'
		game.player[1].y_tower = game.player[1].y - 1;
		game.player[1].x_tower = game.player[1].x - 1;
		break;
		
	case '8':
		game.player[1].tower = '|';
		game.player[1].y_tower = game.player[1].y - 1;
		game.player[1].x_tower = game.player[1].x;
		break;
		
	case '9':
		game.player[1].tower = 0x2F; // '/'
		game.player[1].y_tower = game.player[1].y - 1;
		game.player[1].x_tower = game.player[1].x + 1;
		break;

	case '4':
		game.player[1].tower = '-';
		game.player[1].y_tower = game.player[1].y;
		game.player[1].x_tower = game.player[1].x - 1;
		break;
		
	case '6':
		game.player[1].tower = '-';
		game.player[1].y_tower = game.player[1].y;
		game.player[1].x_tower = game.player[1].x + 1;
		break;

	case '1':
		game.player[1].tower = 0x2F; // '/'
		game.player[1].y_tower = game.player[1].y + 1;
		game.player[1].x_tower = game.player[1].x - 1;
		break;

	case '2':
		game.player[1].tower = '|';
		game.player[1].y_tower = game.player[1].y + 1;
		game.player[1].x_tower = game.player[1].x;
		break;

	case '3':
		game.player[1].tower = 0x5C; // '\'
		game.player[1].y_tower = game.player[1].y + 1;
		game.player[1].x_tower = game.player[1].x + 1;
		break;

	case 0x0A:
		if(game.player[1].bul_count >= 4)
			break;
		else
			pthread_create(&bullet, NULL, bullet_spawn, (void *)&game.player[1]);
		break;

	case 'w':

		game.player[0].y--;
		game.player[0].y_tower--;

		if (game.player[0].y == wind.zero_y + 1 || ((check_location()) == 1))
		{
			game.player[0].y++;
			game.player[0].y_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[0].new_tank[i][j] = game.player[0].vert_tank[i][j];

		break;

	case 's':

		game.player[0].y++;
		game.player[0].y_tower++;
		
		if (game.player[0].y == wind.zero_y + wind.lines - 1 - 1 || ((check_location()) == 1))
		{
			game.player[0].y--;
			game.player[0].y_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[0].new_tank[i][j] = game.player[0].vert_tank[i][j];

		break;

	case 'd':
		game.player[0].x++;
		game.player[0].x_tower++;
	
		if (game.player[0].x == wind.zero_x + wind.cols - 1 - 1 || ((check_location()) == 1))
		{
			game.player[0].x--;
			game.player[0].x_tower--;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[0].new_tank[i][j] = game.player[0].hor_tank[i][j];

		break;

	case 'a':
		game.player[0].x--;
		game.player[0].x_tower--;
		
		if (game.player[0].x == wind.zero_x + 1 || ((check_location()) == 1))
		{
			game.player[0].x++;
			game.player[0].x_tower++;
		}
		for (i = 0; i < 3; i++)
			for (j = 0; j < 3; j++)
				game.player[0].new_tank[i][j] = game.player[0].hor_tank[i][j];

		break;

	case 'r':
		game.player[0].tower = 0x5C; // '\'
		game.player[0].y_tower = game.player[0].y - 1;
		game.player[0].x_tower = game.player[0].x - 1;
		break;
		
	case 't':
		game.player[0].tower = '|';
		game.player[0].y_tower = game.player[0].y - 1;
		game.player[0].x_tower = game.player[0].x;
		break;
		
	case 'y':
		game.player[0].tower = 0x2F; // '/'
		game.player[0].y_tower = game.player[0].y - 1;
		game.player[0].x_tower = game.player[0].x + 1;
		break;

	case 'f':
		game.player[0].tower = '-';
		game.player[0].y_tower = game.player[0].y;
		game.player[0].x_tower = game.player[0].x - 1;
		break;
		
	case 'h':
		game.player[0].tower = '-';
		game.player[0].y_tower = game.player[0].y;
		game.player[0].x_tower = game.player[0].x + 1;
		break;

	case 'v':
		game.player[0].tower = 0x2F; // '/'
		game.player[0].y_tower = game.player[0].y + 1;
		game.player[0].x_tower = game.player[0].x - 1;
		break;

	case 'b':
		game.player[0].tower = '|';
		game.player[0].y_tower = game.player[0].y + 1;
		game.player[0].x_tower = game.player[0].x;
		break;

	case 'n':
		game.player[0].tower = 0x5C; // '\'
		game.player[0].y_tower = game.player[0].y + 1;
		game.player[0].x_tower = game.player[0].x + 1;
		break;

	case ' ':
		if(game.player[0].bul_count >= 4)
			break;
		else
			pthread_create(&bullet, NULL, bullet_spawn, (void *)&game.player[0]);
		break;
	}
	pthread_mutex_unlock(&mutex);
}

int main(int argc, char *argv[])
{
	if (argc != 1)
	{
		printf("Too many arguments\n");
		return -1;
	}

	int ch, i;
	init_window();

	// Init game
	game_initialization();

	while (win == 0)
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
			key_check(ch);

		usleep(1000000/120);

	}

	char buf[256];
	
	if(win == 1)
	{
		sprintf(buf, "Player #1 win in game!");
		erase();
		mvprintw(1, (COLS - strlen(buf)) / 2, buf);
		sprintf(buf, "Press ENTER to exit");
		mvprintw(2, (COLS - strlen(buf)) / 2, buf);
		refresh();
		while((ch = getch()) != 0x0A);
	}

	else if(win == 2)
	{
		sprintf(buf, "Player #2 win in game!");
		erase();
		mvprintw(1, (COLS - strlen(buf)) / 2, buf);
		sprintf(buf, "Press ENTER to exit");
		mvprintw(2, (COLS - strlen(buf)) / 2, buf);
		refresh();
		while((ch = getch()) != 0x0A);
	}

	endwin();

	return 0;
}
