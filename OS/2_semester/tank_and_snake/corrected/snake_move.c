#include <ncurses.h>

int main(void)
{
	int ch, x1, y1, x2, y2, x3, y3, x4, y4, last_pos;
	initscr();
	keypad(stdscr, TRUE);
	x1 = 0;
	x2 = 0;
	x3 = 0;
	x4 = 0;
	y1 = 0;
	y2 = 1;
	y3 = 2;
	y4 = 3;
	last_pos = 3;
	mvaddch(x1, y1, '*');
	mvaddch(x2, y2, '*');
	mvaddch(x3, y3, '*');
	mvaddch(x4, y4, '*');
	refresh();
	curs_set(0);
	halfdelay(1);
	for(;;)
	{
		x1 = x2;
		y1 = y2;
		x2 = x3;
		y2 = y3;
		x3 = x4;
		y3 = y4;
		if((ch=getch())==ERR){
			clear();
			if(last_pos == 1){
				x4--;
				if(x4 < 0)
					x4 = LINES-1;
			}

			if(last_pos == 2){
				x4++;
				if(x4 > LINES-1)
					x4 = 0;
			}

			if(last_pos == 3){
				y4++;
				if(y4 > COLS-1)
					y4 = 0;
			}

			if(last_pos == 4){
				y4--;
				if(y4 < 0)
					y4 = COLS-1;
			}
		}
			
		else{
			clear();
			if(ch == KEY_UP){
				x4--;
				if(x4 < 0)
					x4 = LINES-1;
				last_pos = 1;
			}
			if(ch == KEY_DOWN){
				x4++;
				if(x4 > LINES-1)
					x4 = 0;
				last_pos = 2;
			}
			if(ch == KEY_RIGHT){
				y4++;
				if(y4 > COLS-1)
					y4 = 0;
				last_pos = 3;
			}
			if(ch == KEY_LEFT){
				y4--;
				if(y4 < 0)
					y4 = COLS-1;
				last_pos = 4;
			}
			if(ch == 'q'){
				endwin();
				return 0;
			}
		}

		mvaddch(x1, y1, '*');
		mvaddch(x2, y2, '*');
		mvaddch(x3, y3, '*');
		mvaddch(x4, y4, '*');
		refresh();
	}
	endwin();
	return 0;

}

