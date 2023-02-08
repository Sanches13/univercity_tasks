#include <ncurses.h>

//function that displays the tank on the screen

int print_tank(char tank[3][3], int x, int y)
{
//display each part of the tank separately

	mvaddch(x-1, y-1, tank[0][0]);
	mvaddch(x-1, y, tank[0][1]);
	mvaddch(x-1, y+1, tank[0][2]);
	mvaddch(x, y-1, tank[1][0]);
	mvaddch(x, y, tank[1][1]);
	mvaddch(x, y+1, tank[1][2]);
	mvaddch(x+1, y-1, tank[2][0]);
	mvaddch(x+1, y, tank[2][1]);
	mvaddch(x+1, y+1, tank[2][2]);
	return 0;
}
	
int turn90right(int *x_tower, int *y_tower, char *tower, int x, int y)
{
	switch(*tower){
		case '|':
			if(*x_tower == x-1){
				*x_tower = x;
				*y_tower = y+1;
				*tower = '-';
			}
			
			else{
				*x_tower = x;
				*y_tower = y-1;
				*tower = '-';
			}
				break;
		case '-':
			if(*y_tower == y-1){
				*x_tower = x-1;
				*y_tower = y;
				*tower = '|';
			}
			else{
				*x_tower = x+1;
				*y_tower = y;
				*tower = '|';
			}
			break;
		case 0x2F:
			if(*x_tower == x-1){
				*x_tower = x+1;
				*y_tower = y+1;
				*tower = 0x5C;
			}
			else{
				*x_tower = x-1;
				*y_tower = y-1;
				*tower = 0x5C;
			}
			break;
		case 0x5C:
			if(*x_tower == x-1){
				*x_tower = x-1;
				*y_tower = y+1;
				*tower = 0x2F;
			}
			else{
				*x_tower = x+1;
				*y_tower = y-1;
				*tower = 0x2F;
			}
			break;
	}
	return 0;
}
	
int turn90left(int *x_tower, int *y_tower, char *tower, int x, int y)
{
	switch(*tower){
		case '|':
			if(*x_tower == x-1){
				*x_tower = x;
				*y_tower = y-1;
				*tower = '-';
			}
			
			else{
				*x_tower = x;
				*y_tower = y+1;
				*tower = '-';
			}
			break;
		case '-':
			if(*y_tower == y-1){
				*x_tower = x+1;
				*y_tower = y;
				*tower = '|';
			}
			else{
				*x_tower = x-1;
				*y_tower = y;
				*tower = '|';
			}
			break;
		case 0x2F:
			if(*x_tower == x-1){
				*x_tower = x-1;
				*y_tower = y-1;
				*tower = 0x5C;
			}
			else{
				*x_tower = x+1;
				*y_tower = y+1;
				*tower = 0x5C;
			}
			break;
		case 0x5C:
			if(*x_tower == x-1){
				*x_tower = x+1;
				*y_tower = y-1;
				*tower = 0x2F;
			}
			else{
				*x_tower = x-1;
				*y_tower = y+1;
				*tower = 0x2F;
			}
			break;
	}
	return 0;
}

int main(void)
{
//position at which the tank moves up and down

	char vert_tank[3][3] = {'#', ' ', '#',
	                        '#', '*', '#',	
		                '#', ' ', '#'};

//position at which the tank moves right and left

	char hor_tank[3][3] = {'#', '#', '#',
			       ' ', '*', ' ',
			       '#', '#', '#'};

//tower stores the position of the tank turret

	char new_tank[3][3], tower;

/*coordinates x and y store the position of the central of the tank
  coordinates x_tower and y_tower store the position of the turret
*/

	int ch, x, y, i, j, x_tower, y_tower, last_pos;
	initscr();
	keypad(stdscr, TRUE);

//place the tank in the center of the screen, the turret pointing up
	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			new_tank[i][j] = vert_tank[i][j];

	x = LINES/2;
	y = COLS/2;
	x_tower = x-1;
	y_tower = y;
	tower = '|';
	last_pos = 1;

//display the tank on the screen, then display the turret

	print_tank(new_tank, x, y);
	mvaddch(x_tower, y_tower, tower);
	refresh();
	curs_set(0);

//until the 'u' key is pressed, read the pressed keys
	while((ch=getch())!= 'u')
	{
		clear();
		switch(ch){

//by pressing the arrows, change the coordinates 
//of the center and the turret of the tank 

		case KEY_UP:
			x--;
			x_tower--;

//if the center coordinate now lies on one of the screen borders
//move the tank to the opposite border
			
			if(x == 0){
				x += LINES-2;
				x_tower += LINES-2;
			}

			if(last_pos == 4)
				turn90right(&x_tower, &y_tower, &tower, x, y);

			if(last_pos == 3)
				turn90left(&x_tower, &y_tower, &tower, x, y);

			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					new_tank[i][j] = vert_tank[i][j];
			last_pos = 1;
			break;

		case KEY_DOWN:
			x++;
			x_tower++;
			if(x == LINES-1){
				x -= LINES-2;
				x_tower -= LINES-2;
			}
			
			if(last_pos == 3)
				turn90right(&x_tower, &y_tower, &tower, x, y);

			if(last_pos == 4)
				turn90left(&x_tower, &y_tower, &tower, x, y);

			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					new_tank[i][j] = vert_tank[i][j];
			last_pos = 2;
			break;

		case KEY_RIGHT:
			y++;
			y_tower++;
			if(y == COLS-1){
				y -= COLS-2;
				y_tower -= COLS-2;
			}
		
			if(last_pos == 1)
				turn90right(&x_tower, &y_tower, &tower, x, y);

			if(last_pos == 2)
				turn90left(&x_tower, &y_tower, &tower, x, y);

			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					new_tank[i][j] = hor_tank[i][j];
			last_pos = 3;
			break;

		case KEY_LEFT:
			y--;
			y_tower--;
			if(y == 0){
				y += COLS-2;
				y_tower += COLS-2;
			}

			if(last_pos == 2)
				turn90right(&x_tower, &y_tower, &tower, x, y);

			if(last_pos == 1)
				turn90left(&x_tower, &y_tower, &tower, x, y);


			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					new_tank[i][j] = hor_tank[i][j];
			last_pos = 4;
			break;

//by pressing the 'q' key, turn the turret up and to the right
//change the coordinates of the turret

		case 'q':
			tower = 0x5C;    // '\'
			x_tower = x-1;
			y_tower = y-1;
			break;

//by pressing the 'w' key, turn the turret up

		case 'w':
			tower = '|';
			x_tower = x-1;
			y_tower = y;
			break;		

//by pressing the 'e' key, turn the turret up and to the left

		case 'e':
			tower = 0x2F;	// '/'
			x_tower = x-1;
			y_tower = y+1;
			break;		

//by pressing the 'a' key, turn the turret left

		case 'a':
			tower = '-';
			x_tower = x;
			y_tower = y-1;
			break;

//by pressing the 'd' key, turn the turret right

		case 'd':
			tower = '-';
			x_tower = x;
			y_tower = y+1;
			break;

//by pressing the 'q' key, turn the turret down and to the left

		case 'z':
			tower = 0x2F;	// '/'
			x_tower = x+1;
			y_tower = y-1;
			break;

//by pressing the 'q' key, turn the turret down

		case 'x':
			tower = '|';
			x_tower = x+1;
			y_tower = y;
			break;

//by pressing the 'q' key, turn the turret down and to the right

		case 'c':
			tower = 0x5C;	// '\'
			x_tower = x+1;
			y_tower = y+1;
			break;
		}

//display the tank and the turret on the screen

		print_tank(new_tank, x, y);
		mvaddch(x_tower, y_tower, tower);
		refresh();
	}

	endwin();
	return 0;

}

