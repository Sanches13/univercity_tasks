#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
typedef struct{
	//cannon coordinates
	int cannon_x, cannon_y;
	//ship coordinates
	int ship_x[5], ship_y[5];
	//bullet coordinates, counter - number of bullets per field,
	//bullet_num - number of shots
	int bullet_x[3], bullet_y[3], bullet_num, counter;
	//if the variable win is 1, then the player won
	int win;
}game;

//procedure displaying playing field
int print_arena()
{
	int height, width;
	pthread_mutex_lock(&mutex);
	for(height = LINES/3; height <= 2*LINES/3; height++){
		if(height == LINES/3 || height  == 2*LINES/3){
			for(width = COLS/3; width <= 2*COLS/3; width++)
				mvaddch(height, width, '+');
		}
		else{
			mvaddch(height, COLS/3, '+');
			mvaddch(height, 2*COLS/3, '+');
		}
	}
	pthread_mutex_unlock(&mutex);
	return 0;
}

//procedure calculating and changing the coordinates of the ship	
static void* ship_calc(void *data)
{
	game *p = (game*)data;
	int i;
	
	//the thread is running, the player has not won
	while(p->win == 0){
		//we suspend the work of the thread
		usleep(100000);

		pthread_mutex_lock(&mutex);
		mvaddch(p->ship_x[0], p->ship_y[0], ' ');
		for(i = 0; i < 5; i++)
		{
			//change the coordinates of the ship
			p->ship_x[i]--;
			if(p->ship_x[i] == LINES/3)
				p->ship_x[i] = p->ship_x[i] * 2 - 1;

			mvaddch(p->ship_x[i], p->ship_y[i], '#');

			refresh();
		}

		pthread_mutex_unlock(&mutex);
	}
}

//procedure calculating and changing the coordinates of the bullets	
static void* bullet_calc(void *data)
{
	game *p = (game*)data;
	int i;
	//the thread is running, the player has not won
	while(p->win==0){
		//we suspend the work of the thread
		usleep(100000);

		pthread_mutex_lock(&mutex);
		for(i = 0; i < 3; i++)
		{
			//change the coordinates of the ship
			mvaddch(p->bullet_x[i], p->bullet_y[i], ' ');

			p->bullet_y[i]++;	
			if(p->bullet_y[i] == 2*COLS/3 - 1)
				p->counter--;
			
			if(p->bullet_y[i] < 2*COLS/3+1)
				mvaddch(p->bullet_x[i], p->bullet_y[i], '*');

			refresh();
		}
		pthread_mutex_unlock(&mutex);
	}
}

//procedure calculating and changing the coordinates of the bullets and of the ship
//if the player hits the ship, then the variable is assigned 1	
static void* calculation(void *data)
{
	game *p = (game*)data;
	int i, j;
	while(p->win==0){
		//we suspend the work of the thread
		usleep(100000);
		pthread_mutex_lock(&mutex);
		for(i = 0; i < 3; i++)
			for(j = 0; j < 5; j++)
			{
				if((p->bullet_x[i] == p->ship_x[j]) && (p->bullet_y[i] == p->ship_y[j]))
					p->win++;
			}
		pthread_mutex_unlock(&mutex);
	}
}

//procedure that spawns bullets
int bullet_spawn(void *data)
{
	game *p = (game*)data;
	//check how many bullets are on the field now
	pthread_mutex_lock(&mutex);
	if(p->counter != 3)
	{
		//assign values to the coordinates of the bullet
		p->bullet_x[p->bullet_num % 3] = p->cannon_x;
		p->bullet_y[p->bullet_num % 3] = COLS/3+2;
		//increasing the number of shots
		p->bullet_num++;
		//increasing the number of bullets per field
		p->counter++;
	}
	pthread_mutex_unlock(&mutex);
	return 0;
}

int main()
{
	int i, ch;
	game a;

	pthread_t ship_pos, bullet_pos, calc;

	initscr();
	noecho();
	//assign initial values to the coordinates of the ship
	for(i = 0; i < 5; i++){
		a.ship_x[i] = 2*LINES/3 - i - 1;
		a.ship_y[i] = 2*COLS/3 - 1;
	}

	//assign initial values to the coordinates of the cannon
	a.cannon_x = LINES/2;
	a.cannon_y = COLS/3+1;

	a.bullet_num = 0;
	a.counter = 0;
	a.win = 0;

	keypad(stdscr, TRUE);
	curs_set(0);
	clear();

	//the function halfdelay turns on the mode when the getch() function	
	//can wait no more than N tens of a second for pressing the key
	halfdelay(1);
	
	pthread_create(&(bullet_pos), NULL, bullet_calc, &a);
	pthread_create(&(calc), NULL, calculation, &a);
	pthread_create(&(ship_pos), NULL, ship_calc, &a);
	
	while((ch = getch()) && a.win == 0){
		switch(ch){
		//if the key KEY_BACKSPACE is pressed then exit the game
		case KEY_BACKSPACE:
			a.win = 2;
			break;
		//if the key SPACE is pressed then shoot
		case 0x20:
			bullet_spawn(&a);
			break;
		}

		
		//print a playing field
		print_arena();
		//print a cannon
		mvaddch(a.cannon_x, a.cannon_y, '<');

		refresh();

	}

	pthread_join(bullet_pos, NULL);
	pthread_join(calc, NULL);
	pthread_join(ship_pos, NULL);

	//Victory message
	if(a.win == 1){
		mvprintw(LINES/5, (COLS-strlen("YOU WIN!"))/2, "YOU WIN!");
		mvprintw(LINES/5+1, (COLS-strlen("Please, press the any key to exit"))/2, "Please, press the any key to exit");
		refresh();
		while((ch = getch()) == ERR);
	}
	endwin();
	return 0;
}
