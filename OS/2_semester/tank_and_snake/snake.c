#include <ncurses.h>

//the structure into which the coordinates of each part of the snake
//will be written

typedef struct{
	int x, y;
}snake;

//the function that displays the snake on the screen

int print_snake(snake *p)
{
	int i;
	for(i = 0; i < 4; i++)
		mvaddch(p[i].x, p[i].y, '*');
	return 0;
}

int main(void)
{
	int ch, i;

//the snake will have a lenght of 4 symbols

	snake p[4];

	initscr();
	keypad(stdscr, TRUE);

//set the initial position of the snake in the upper left corner
//of the screen

	for(i = 0; i < 4; i++){
		p[i].x = 0;
		p[i].y = i;
	}

	print_snake(p);
	refresh();
	curs_set(0);

//if the 'q' key is pressed, finish the program

	while((ch=getch())!= 'q')
	{
		if((ch!=KEY_UP)&&(ch!=KEY_DOWN)&&(ch!=KEY_RIGHT)&&(ch!=KEY_LEFT)){
			clear();
			print_snake(p);
			refresh();
			continue;	
		}

//assign the coordinates of the snake part to the coordinates of the next
//snake part

		for(i = 0; i < 3; i++){
			p[i].x = p[i+1].x;
			p[i].y = p[i+1].y;
		}
		clear();

//to determine the position of the last part, we analyze the value
//of the pressed key

		switch(ch){
		case KEY_UP:
			p[3].x--;
			if(p[3].x < 0)
				p[3].x = LINES-1;
			break;

		case KEY_DOWN:
			p[3].x++;
			if(p[3].x > LINES-1)
				p[3].x = 0;
			break;

		case KEY_RIGHT:
			p[3].y++;
			if(p[3].y > COLS-1)
				p[3].y = 0;
			break;

		case KEY_LEFT:
			p[3].y--;
			if(p[3].y < 0)
				p[3].y = COLS-1;
			break;	
		}

//display the snake on the screen

		print_snake(p);
		refresh();
	}
	endwin();
	return 0;

}

