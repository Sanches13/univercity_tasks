#include <stdio.h>
#include <pthread.h>
#include <ncurses.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
//global variable, contains the current warehouse occupancy
int product, flag;

//procedure that reduces the number of bins in warehouse
static void *consumer(void *data)
{
	while(flag == 0){
		//we suspend the work of the thread
		usleep(80000);
		//check if the warehouse is full
		pthread_mutex_lock(&mutex);
		if(product > 0)		
			product--;
		pthread_mutex_unlock(&mutex);
	}
}

//procedure increasing the number of bins in warehouse
static void *producer(void *data)
{
	while(flag == 0){	
		//we suspend the work of the thread
		usleep(60000);
		//check if the warehouse is full.
		//If there is room, we increase
		pthread_mutex_lock(&mutex);
		if(product < LINES/3)		
			product++;
		pthread_mutex_unlock(&mutex);
	}
}

//procedure displaying warehouse "boundaries"
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

int main()
{
	pthread_t cons, prod;

	int ch, i , j;
	void *retv1, *retv2;

	flag = 0;

	initscr();	
	curs_set(0);
	keypad(stdscr, TRUE);

	//the function halfdelay turns on the mode when the getch() function	
	//can wait no more than N tens of a second for pressing the key
	halfdelay(1);
	clear();
	
	//initial product quantity
	product = 0;
	pthread_create(&prod, NULL, producer, NULL);
	pthread_create(&cons, NULL, consumer, NULL);
	while(flag == 0){

		if((ch = getch()) == 'q')
			flag++;

		clear();
		//print a warehouse
		print_arena();
		//print the quantity of product
		for(i = LINES/3+1; i < LINES/3+product; i++)
			for(j = COLS/3+1; j < 2*COLS/3; j++)
				mvaddch(i, j, '#');
		refresh();
	}
	pthread_join(cons, &retv1);
	pthread_join(prod, &retv2);
	
	endwin();
	return 0;
}
