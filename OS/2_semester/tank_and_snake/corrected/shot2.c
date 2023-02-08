#include <ncurses.h>

typedef struct{
	int x, y, x_tower, y_tower, x_bul, y_bul, num_bul, bul_tower;
	char new_tank[3][3], tower, hor_tank[3][3], vert_tank[3][3];
}tank;
//function that displays the tank on the screen

int bullet_spawn(void *data)
{
	tank *p = (tank*)data;
	//check how many bullets are on the field now
	if(p->num_bul == 0)
	{
		switch(p->tower){
		case '|':
			if(p->y_tower < p->y)
			{
				p->y_bul = p->y_tower - 1;
				p->bul_tower = 1;
			}
			else
			{
				p->y_bul = p->y_tower + 1;
				p->bul_tower = 2;
			}

			p->x_bul = p->x_tower;
			break;

		case '-':
			if(p->x_tower < p->x)
			{
				p->x_bul = p->x_tower - 1;
				p->bul_tower = 3;
			}
			else
			{
				p->x_bul = p->x_tower + 1;
				p->bul_tower = 4;
			}

			p->y_bul = p->y_tower;
			break;

		case '/':
			if(p->y_tower < p->y)
			{
				p->y_bul = p->y_tower - 1;
				p->x_bul = p->x_tower + 1;
				p->bul_tower = 5;
			}
			else
			{
				p->y_bul = p->y_tower + 1;
				p->x_bul = p->x_tower - 1;
				p->bul_tower = 6;
			}

			break;

		case 0x5C:
			if(p->y_tower < p->y)
			{
				p->y_bul = p->y_tower - 1;
				p->x_bul = p->x_tower - 1;
				p->bul_tower = 7;
			}
			else
			{
				p->y_bul = p->y_tower + 1;
				p->x_bul = p->x_tower + 1;
				p->bul_tower = 8;
			}

			break;
		}
		p->num_bul = 1;
	}
	return 0;
}

int print_tank(void *data)
{
//display each part of the tank separately
	tank *p = (tank*)data;

	mvaddch(p->y-1, p->x-1, p->new_tank[0][0]);
	mvaddch(p->y-1, p->x, p->new_tank[0][1]);
	mvaddch(p->y-1, p->x+1, p->new_tank[0][2]);
	mvaddch(p->y, p->x-1, p->new_tank[1][0]);
	mvaddch(p->y, p->x, p->new_tank[1][1]);
	mvaddch(p->y, p->x+1, p->new_tank[1][2]);
	mvaddch(p->y+1, p->x-1, p->new_tank[2][0]);
	mvaddch(p->y+1, p->x, p->new_tank[2][1]);
	mvaddch(p->y+1, p->x+1, p->new_tank[2][2]);

	mvaddch(p->y_tower, p->x_tower, p->tower);

	if(p->num_bul == 1)
	{
		if(p->y_bul >= 0 && p->y_bul <= LINES-1 && p->x_bul >=0 && p->x_bul <= COLS-1)
		{
			mvaddch(p->y_bul, p->x_bul, 'o');
			switch(p->bul_tower){
			case 1:
				p->y_bul--;
				break;
			case 2:
				p->y_bul++;
				break;
			case 3:
				p->x_bul--;
				break;
			case 4:
				p->x_bul++;
				break;
			case 5:
				p->y_bul--;
				p->x_bul++;
				break;
			case 6:
				p->y_bul++;
				p->x_bul--;
				break;
			case 7:
				p->y_bul--;
				p->x_bul--;
				break;
			case 8:
				p->y_bul++;
				p->x_bul++;
				break;
			}
		}
		else
			p->num_bul = 0;
	}
	return 0;
}

int main(void)
{
	int i, j, ch;
	initscr();
	keypad(stdscr, TRUE);

//place the tank in the center of the screen, the turret pointing up

	tank tank_p;
	tank_p.hor_tank[0][0] = tank_p.hor_tank[0][1] = tank_p.hor_tank[0][2] =
	tank_p.hor_tank[2][0] = tank_p.hor_tank[2][1] = tank_p.hor_tank[2][2] =
	tank_p.vert_tank[0][0] = tank_p.vert_tank[1][0] = tank_p.vert_tank[2][0] = 
	tank_p.vert_tank[0][2] = tank_p.vert_tank[1][2] = tank_p.vert_tank[2][2] = '#';

	tank_p.hor_tank[1][0] = tank_p.hor_tank[1][2] =
	tank_p.vert_tank[0][1] = tank_p.vert_tank[2][1] = ' ';
	
	tank_p.hor_tank[1][1] = tank_p.vert_tank[1][1] = '*';

	tank_p.num_bul = 0;

	for(i = 0; i < 3; i++)
		for(j = 0; j < 3; j++)
			tank_p.new_tank[i][j] = tank_p.vert_tank[i][j];

	tank_p.y = LINES/2;
	tank_p.x = COLS/2;
	tank_p.y_tower = tank_p.y-1;
	tank_p.x_tower = tank_p.x;
	tank_p.tower = '|';

//display the tank on the screen, then display the turret

	print_tank(&tank_p);
	mvaddch(tank_p.y_tower, tank_p.x_tower, tank_p.tower);
	refresh();
	curs_set(0);

//until the 'u' key is pressed, read the pressed keys
	timeout(100);
	while((ch=getch())!= KEY_BACKSPACE)
	{
		clear();
		switch(ch){

//by pressing the arrows, change the coordinates 
//of the center and the turret of the tank 

		case KEY_UP:
			tank_p.y--;
			tank_p.y_tower--;

//if the center coordinate now lies on one of the screen borders
//move the tank to the opposite border

			if(tank_p.y == 0){
				tank_p.y++;
				tank_p.y_tower++;
			}
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					tank_p.new_tank[i][j] = tank_p.vert_tank[i][j];
			break;
		case KEY_DOWN:
			tank_p.y++;
			tank_p.y_tower++;
			if(tank_p.y == LINES-1){
				tank_p.y--;
				tank_p.y_tower--;
			}
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					tank_p.new_tank[i][j] = tank_p.vert_tank[i][j];
			break;
		case KEY_RIGHT:
			tank_p.x++;
			tank_p.x_tower++;
			if(tank_p.x == COLS-1){
				tank_p.x--;
				tank_p.x_tower--;
			}
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					tank_p.new_tank[i][j] = tank_p.hor_tank[i][j];
			break;
		case KEY_LEFT:
			tank_p.x--;
			tank_p.x_tower--;
			if(tank_p.x == 0){
				tank_p.x++;
				tank_p.x_tower++;
			}
			for(i = 0; i < 3; i++)
				for(j = 0; j < 3; j++)
					tank_p.new_tank[i][j] = tank_p.hor_tank[i][j];
			break;

//by pressing the 'q' key, turn the turret up and to the right
//change the coordinates of the turret

		case 'q':
			tank_p.tower = 0x5C;    // '\'
			tank_p.y_tower = tank_p.y-1;
			tank_p.x_tower = tank_p.x-1;
			break;

//by pressing the 'w' key, turn the turret up

		case 'w':
			tank_p.tower = '|';
			tank_p.y_tower = tank_p.y-1;
			tank_p.x_tower = tank_p.x;
			break;		

//by pressing the 'e' key, turn the turret up and to the left

		case 'e':
			tank_p.tower = 0x2F;	// '/'
			tank_p.y_tower = tank_p.y-1;
			tank_p.x_tower = tank_p.x+1;
			break;		

//by pressing the 'a' key, turn the turret left

		case 'a':
			tank_p.tower = '-';
			tank_p.y_tower = tank_p.y;
			tank_p.x_tower = tank_p.x-1;
			break;

//by pressing the 'd' key, turn the turret right

		case 'd':
			tank_p.tower = '-';
			tank_p.y_tower = tank_p.y;
			tank_p.x_tower = tank_p.x+1;
			break;

//by pressing the 'q' key, turn the turret down and to the left

		case 'z':
			tank_p.tower = 0x2F;	// '/'
			tank_p.y_tower = tank_p.y+1;
			tank_p.x_tower = tank_p.x-1;
			break;

//by pressing the 'q' key, turn the turret down

		case 'x':
			tank_p.tower = '|';
			tank_p.y_tower = tank_p.y+1;
			tank_p.x_tower = tank_p.x;
			break;

//by pressing the 'q' key, turn the turret down and to the right

		case 'c':
			tank_p.tower = 0x5C;	// '\'
			tank_p.y_tower = tank_p.y+1;
			tank_p.x_tower = tank_p.x+1;
			break;

		case 's':
			bullet_spawn(&tank_p);
			break;
		}
//display the tank and the turret on the screen

		print_tank(&tank_p);
		refresh();
	}

	endwin();
	return 0;

}

