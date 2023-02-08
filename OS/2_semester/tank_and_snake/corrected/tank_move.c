#include <ncurses.h>

int print_tank(char tank[3][3], int x, int y)
{
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
int main(void)
{
	char vert_tank[3][3] = {'#', ' ', '#',
	                        '#', '*', '#',	
		                '#', ' ', '#'};

	char gor_tank[3][3] = {'#', '#', '#',
			       ' ', '*', ' ',
			       '#', '#', '#'};
	char new_tank[3][3], tower;

	int ch, x, y, i, j, x_tower, y_tower, last_pos;
	last_pos = 1;
	initscr();
	keypad(stdscr, TRUE);
	x = LINES/2;
	y = COLS/2;
	x_tower = x-1;
	y_tower = y;
	tower = '|';
	print_tank(vert_tank, x, y);
	mvaddch(x_tower, y_tower, tower);
	refresh();
	curs_set(0);
	halfdelay(1);
	for(;;)
	{
		//if((ch = getch())==ERR){
		if(((ch = getch())==ERR) || ((ch!='q')&&(ch!='w')&&(ch!='e')&&(ch!='a')&&(ch!='d')&&(ch!='z')&&(ch!='x')&&(ch!='c')&&(ch!='u')&&(ch!=KEY_UP)&&(ch!=KEY_RIGHT)&&(ch!=KEY_DOWN)&&(ch!=KEY_LEFT))){
		clear();
			if(last_pos == 1){
				x--;
				x_tower--;
				if(x == 0){
					x += LINES-2;
					x_tower += LINES-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = vert_tank[i][j];
			}
			
			if(last_pos == 2){
				x++;
				x_tower++;
				if(x == LINES-1){
					x -= LINES-2;
					x_tower -= LINES-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = vert_tank[i][j];
			}
			
			if(last_pos == 3){
				y++;
				y_tower++;
				if(y == COLS-1){
					y -= COLS-2;
					y_tower -= COLS-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = gor_tank[i][j];
			}
			
			if(last_pos == 4){
				y--;
				y_tower--;
				if(y == 0){
					y += COLS-2;
					y_tower += COLS-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = gor_tank[i][j];
			}
		}
		else{
			clear();
			if(ch == KEY_UP){
				x--;
				x_tower--;
				if(x == 0){
					x += LINES-2;
					x_tower += LINES-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = vert_tank[i][j];
				last_pos = 1;

			}
			if(ch == KEY_DOWN){
				x++;
				x_tower++;
				if(x == LINES-1){
					x -= LINES-2;
					x_tower -= LINES-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = vert_tank[i][j];
				last_pos = 2;			
			}
			if(ch == KEY_RIGHT){
				y++;
				y_tower++;
				if(y == COLS-1){
					y -= COLS-2;
					y_tower -= COLS-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = gor_tank[i][j];
				last_pos = 3;
			}
			if(ch == KEY_LEFT){
				y--;
				y_tower--;
				if(y == 0){
					y += COLS-2;
					y_tower += COLS-2;
				}
				for(i = 0; i < 3; i++)
					for(j = 0; j < 3; j++)
						new_tank[i][j] = gor_tank[i][j];
				last_pos = 4;
			}
			if(ch == 'q'){
				tower = 0x5C;
				x_tower = x-1;
				y_tower = y-1;
			}
			if(ch == 'w'){
				tower = '|';
				x_tower = x-1;
				y_tower = y;
			}		
			if(ch == 'e'){
				tower = 0x2F;
				x_tower = x-1;
				y_tower = y+1;
			}			
			if(ch == 'a'){
				tower = '-';
				x_tower = x;
				y_tower = y-1;
			}
			if(ch == 'd'){
				tower = '-';
				x_tower = x;
				y_tower = y+1;
			}
			if(ch == 'z'){
				tower = 0x2F;
				x_tower = x+1;
				y_tower = y-1;
			}
			if(ch == 'x'){
				tower = '|';
				x_tower = x+1;
				y_tower = y;
			}
			if(ch == 'c'){
				tower = 0x5C;
				x_tower = x+1;
				y_tower = y+1;
			}
	
			if(ch == 'u'){
				endwin();
				return 0;
			}
		}
		print_tank(new_tank, x, y);
		mvaddch(x_tower, y_tower, tower);
		refresh();
	}
	endwin();
	return 0;

}

