#include <ncurses.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//the structure into which the coordinates of each part of the snake
//will be written

struct snake{
	int x[1024], y[1024], last, snake_len, tail_x, tail_y;
	int apple_x, apple_y;
};

//the function that displays the snake on the screen

int print_snake(struct snake *p)
{
	int i;
	for(i = 0; i < p->snake_len; i++)
		mvaddch(p->y[i], p->x[i], 'O');
	return 0;
}

int apple_spawn(struct snake *p)
{
	int i;
	srand(time(NULL));
	p->apple_x = rand() % COLS;
	p->apple_y = rand() % LINES;
	for(i = 0; i < p->snake_len; i++)
		if(p->apple_x == p->x[i] && p->apple_y == p->y[i])
		{
			p->apple_x = rand() % COLS;
			p->apple_y = rand() % LINES;
			i = 0;
		}
	return 0;
}

int main(void)
{
	int ch, i, index, j;
	index = 0;

//the snake will have a lenght of 4 symbols

	struct snake p;

	initscr();
	keypad(stdscr, TRUE);
	noecho();
//set the initial position of the snake in the upper left corner
//of the screen

	p.snake_len = 4;
	for(i = 0; i < p.snake_len; i++){
		p.x[i] = i;
		p.y[i] = 0;
	}

	p.tail_x = p.x[0];
	p.tail_y = p.y[0];

	p.last = 3;
	
	apple_spawn(&p);
	mvaddch(p.apple_y, p.apple_x, 'o');
	print_snake(&p);
	refresh();
	curs_set(0);
	halfdelay(2);
//if the 'q' key is pressed, finish the program

	while((ch=getch())!= 'q' && index == 0)
	{
//to determine the position of the last part, we analyze the value
//of the pressed key

		p.tail_x = p.x[0];
		p.tail_y = p.y[0];

		for(i = 0; i < p.snake_len-1; i++){
			p.x[i] = p.x[i+1];
			p.y[i] = p.y[i+1];
		}

		switch(ch){
		case ERR:
			switch(p.last){
			case 1:
				p.y[(p.snake_len)-1]--;
				if(p.y[(p.snake_len)-1] < 0)
					index = 2;
				break;
			case 2:
				p.y[(p.snake_len)-1]++;
				if(p.y[(p.snake_len)-1] > LINES-1)
					index = 2;
				break;
			case 3:
				p.x[(p.snake_len)-1]++;
				if(p.x[(p.snake_len)-1] > COLS-1)
					index = 2;
				break;
			case 4:
				p.x[(p.snake_len)-1]--;
				if(p.x[(p.snake_len)-1] < 0)
					index = 2;
				break;
			}
			break;

		case KEY_UP:
			p.y[(p.snake_len)-1]--;
			if(p.y[(p.snake_len)-1] < 0)
				index = 2;
			p.last = 1;
			break;

		case KEY_DOWN:
			p.y[(p.snake_len)-1]++;
			if(p.y[(p.snake_len)-1] > LINES-1)
				index = 2;
			p.last = 2;
			break;

		case KEY_RIGHT:
			p.x[(p.snake_len)-1]++;
			if(p.x[(p.snake_len)-1] > COLS-1)
				index = 2;
			p.last = 3;
			break;

		case KEY_LEFT:
			p.x[(p.snake_len)-1]--;
			if(p.x[(p.snake_len)-1] < 0)
				index = 2;
			p.last = 4;
			break;	
		}

		for(i = 0; i < p.snake_len-1; i++)
		{
			if(p.x[(p.snake_len)-1] == p.x[i] && p.y[(p.snake_len)-1] == p.y[i])
				index = 1;
		}

		if(p.x[(p.snake_len)-1] == p.apple_x && p.y[(p.snake_len)-1] == p.apple_y)
		{
			p.snake_len++;
			
			for(i = p.snake_len-1; i > 0; i--){
				p.x[i] = p.x[i-1];
				p.y[i] = p.y[i-1];
			}

			p.x[0] = p.tail_x;
			p.y[0] = p.tail_y;
			apple_spawn(&p);
		}
		//assign the coordinates of the snake part to the coordinates of the next
		//snake part

		clear();

		//display the snake on the screen
		mvaddch(p.apple_y, p.apple_x, '+');
		print_snake(&p);
		refresh();
	}

	if(index == 1)
		mvprintw(LINES/2, (COLS-strlen("You lose.SNAKE. Press any key to exit."))/2, "You lose.SNAKE. Press any key to exit.");

	if(index == 2)
		mvprintw(LINES/2, (COLS-strlen("You lose.BORT. Press any key to exit."))/2, "You lose.BORT. Press any key to exit.");

	refresh();
	while((ch = getch()) == ERR);
	endwin();
	return 0;

}

