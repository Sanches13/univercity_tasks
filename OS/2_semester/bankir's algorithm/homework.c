#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

//function that displays a matrix 4x4 on the screen 
int print_matrix(int M[4][4], int x, int y)
{
	int i, j, c_x;
	c_x = x;
	for(i = 0; i < 4; i++)
	{
		x = c_x;
		for(j = 0; j < 4; j++)
		{
			mvprintw(y, x, "%d", M[i][j]);
			x = x + 2;
		}
		y = y + 2;
	}
	return 0;
}

//function that displays a matrix 4x1 on the screen 
int print_vector(int *V, int x, int y)
{
	int i;
	for(i = 0; i < 4; i++)
	{
		mvprintw(y, x, "%d", V[i]);
		x = x + 2;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	int i, j, counter, l, ch, x, y, block, c;
	block = x = y = 0;
	int T[4], A[4], C[4][4], R[4][4], keys[4];

	//filling the matrix with initial values
	for(i = 0; i < 4; i++)
	{
		keys[i] = 0;
		T[i] = 0;
		A[i] = 0;
		for(j = 0; j < 4; j++)
		{
			C[i][j] = 0;
			R[i][j] = 0;
		}
	}

	//initial ncurses mode
	initscr();

	//turn on the simple operation mode with function keys
	keypad(stdscr, TRUE);

	//turn off the display of the cursor
	curs_set(0);

	//turn off the display of entered characters
	noecho();

	//display matrices on the screen
	print_vector(T, 1, 1);
	print_matrix(C, 1, 10);
	print_vector(A, 30, 1);
	print_matrix(R, 30, 10);

	refresh();

	mvprintw(0, (LINES-1)/strlen("Press SPACE when you're done filling in the matrix T"), "%s", "Press SPACE when you're done filling in the matrix T");
	x = y = 1;
	i = 0;
	move(y, x);
	curs_set(1);

	//filling the T matrix until the space key is pressed
	while((ch=getch())!=0x20)
	{
		switch(ch){
		case '1':
			T[i] = 1;
			break;
		case '2':
			T[i] = 2;
			break;
		case '3':
			T[i] = 3;
			break;
		case '4':
			T[i] = 4;
			break;
		case '5':
			T[i] = 5;
			break;
		case '6':
			T[i] = 6;
			break;
		case '7':
			T[i] = 7;
			break;
		case '8':
			T[i] = 8;
			break;
		case '9':
			T[i] = 9;
			break;
		case '0':
			T[i] = 0;
			break;		

		//move between elements
		case KEY_LEFT:
			i--;
			if(i < 0)
				i = 0;
			x = x - 2;
			if(x < 1)
				x = 1;
			break;
		case KEY_RIGHT:
			i++;
			if(i > 3)
				i = 3;
			x = x + 2;
			if(x > 7)
				x = 7;
		break;
		}

		//display the matrix T on the screen
		print_vector(T, 1, 1);
		move(y, x);
		refresh();
	}

	//fill the A matrix
	for(i = 0; i < 4; i++)
		A[i] = T[i];

	//display the matrix T on the screen
	clear();

	print_vector(T, 1, 1);
	print_matrix(C, 1, 10);
	print_vector(A, 30, 1);
	print_matrix(R, 30, 10);

	refresh();
	sleep(1);

	while(block == 0)
	{
		//filling the "keys" with initial value
		for(i = 0; i < 4; i++)
			keys[i] = 0;

		mvprintw(0, (LINES-1)/strlen("Press SPACE when you're done filling in the matrix R"), "%s", "Press SPACE when you're done filling in the matrix R");

		//set the cursor to the starting position
		i = j = 0;
		x = 30;
		y = 10;
		move(y, x);

		//fill the R matrix
		while((ch=getch()) != 0x20)
		{
			switch(ch){
			case '1':
				R[i][j] = 1;
				break;
			case '2':
				R[i][j] = 2;
				break;
			case '3':
				R[i][j] = 3;
				break;
			case '4':
				R[i][j] = 4;
				break;
			case '5':
				R[i][j] = 5;
				break;
			case '6':
				R[i][j] = 6;
				break;
			case '7':
				R[i][j] = 7;
				break;
			case '8':
				R[i][j] = 8;
				break;
			case '9':
				R[i][j] = 9;
				break;
			case '0':
				R[i][j] = 0;
				break;	

			//move between elements
			case KEY_LEFT:
				j--;
				if(j < 0)
					j = 0;
				x = x - 2;
				if(x < 30)
					x = 30;
				break;
			case KEY_RIGHT:
				j++;
				if(j > 3)
					j = 3;
				x = x + 2;
				if(x > 36)
					x = 36;
				break;	
			case KEY_UP:
				i--;
				if(i < 0)
					i = 0;
				y = y - 2;
				if(y < 10)
					y = 10;
				break;	
			case KEY_DOWN:
				i++;
				if(i > 3)
					i = 3;
				y = y + 2;
				if(y > 16)
					y = 16;
				break;	
			}
			print_matrix(R, 30, 10);
			move(y, x);
			refresh();
		}

		clear();

		print_vector(T, 1, 1);
		print_matrix(C, 1, 10);
		print_vector(A, 30, 1);
		print_matrix(R, 30, 10);
		refresh();

		//if the matrix is empty, then finish the program
		counter = 0;
		for(i = 0; i < 4; i++)
			for(j = 0; j < 4; j++)
				counter = counter + R[i][j];

		if(counter == 0)
			block = 2;

		//By the task, if all elements of one of the rows of the matrix R
		//are equal to zero, then we release the resources occupied by the
		//completed process
		/*for(i = 0; i < 4; i++)
			if(R[i][0] == 0 && R[i][1] == 0 && R[i][2] == 0 && R[i][3] == 0)
			{
				A[0] = A[0] + C[i][0]; 
				A[1] = A[1] + C[i][1];
				A[2] = A[2] + C[i][2];
				A[3] = A[3] + C[i][3];
				C[i][0] = C[i][1] = C[i][2] = C[i][3] = 0;
			}
*/
		for(l = 0; l < 4 && block == 0; l++)
		{
			for(i = 0; i < 4; i++)
			{
				counter = 0;

				//if keys[i] == 1, then go to the next iteration
				//it means that we have already discovered the thread
				//that can be provided with resources
				for(j = 0; j < 4 && !keys[i]; j++)
				{
					if(R[i][j] > A[j])
					{
						counter = 1;
						//block = 1;
					}
				}
		
				//if counter != 0, then deadlock detected
				//else we fill in the matrices A[], C[], R[]
				if(counter == 0 && !keys[i]/* && block==0*/)
				{
					for(j = 0; j < 4; j++)
					{
						A[j] = A[j] - R[i][j];
						C[i][j] =/* C[i][j]*/ + R[i][j];
						R[i][j] = 0;
					}
					keys[i] = 1;
				}

				else
				{
					for(j = 0; j < 4; j++)
					{
						A[j] = A[j] + C[i][j];
						C[i][j] = 0;
						R[i][j] = 0;
					}
				}

				//display matrices on the screen
				print_vector(T, 1, 1);
				print_matrix(C, 1, 10);
				print_vector(A, 30, 1);
				print_matrix(R, 30, 10);
				refresh();
			}
			sleep(1);
		}

		//if at least key[i] is zero, then a deadlock is detected
		counter = 0;
		for(i = 0; i < 4; i++)
			counter = counter + keys[i];
		if(counter != 4 && block == 0)
			block = 1;
	}

	//print the message
        if(block == 2)
	        mvprintw(0, 0, "%s", "No deadlock");       
	else
	        mvprintw(0, 0, "%s", "DEADLOCK");
	
	refresh();
	sleep(2);
	endwin();
	return 0;
}

