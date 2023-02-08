#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>
#include <string.h>

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
int print_vector(int V[], int x, int y)
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
	int i, j, counter, l;

	//filling the matrix with initial values
	int keys[] = {0, 0, 0, 0};
	int T[] = {9, 9, 9, 9};
	int A[] = {4, 1, 5, 6};
	int C[4][4] = {{2, 3, 1, 0}, {2, 5, 1, 1}, {1, 0, 2, 1}, {0, 0, 0, 1}};
	int R[4][4] = {{3, 2, 1, 2}, {2, 0, 5, 6}, {2, 1, 8, 2}, {1, 0, 3, 2}};

	//if argc > 1, then R[2][2] = 8, else R[2][2] = 7
	if(argc == 1)
		R[2][2] = 7;

	//initial ncurses mode
	initscr();

	//turn off the display of entered characters
	noecho();

	//turn on the simple operation mode with function keys
	keypad(stdscr, TRUE);

	//turn off the display of the cursor
	curs_set(0);

	for(l = 0; l < 4; l++)
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
					counter = 1;
			}
	
			//if counter != 0, then deadlock detected
			//else we fill in the matrix A[]
			if(counter == 0 && !keys[i])
			{
				for(j = 0; j < 4; j++)
				{
					R[i][j] = 0;
					A[j] = A[j] + C[i][j];
				}
				keys[i] = 1;
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

	counter = 0;

	//if at least key[i] is zero, then a deadlock is detected
	for(i = 0; i < 4; i++)
		counter = counter + keys[i];

	//print the message
        if(counter == 4)
	        mvprintw(0, 0, "%s", "No deadlock");
        else
	        mvprintw(0, 0, "%s", "DEADLOCK");

	refresh();
	
	sleep(3);
	endwin();
	return 0;
}
