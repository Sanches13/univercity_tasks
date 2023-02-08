#include <stdio.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
int main()
{
	int i;
	double print_time, print_speed, num_symbols;
        char your_str[1024];
        time_t start_print, end_print;

        printf("Press the key ENTER to end the program\n");
        sleep(2);
	printf("Print speed test. Get ready\n");
	for(i = 3; i >=0; i--)
	{
		if(i == 0)
			printf("Go!\n");
		else
		{
			printf("%d\n", i);
			sleep(1);
		}
	}
        
	start_print = time(NULL);
	fgets(your_str, 1024, stdin);
	end_print = time(NULL);

	num_symbols = ((float)strlen(your_str))-1;
	print_time = difftime(end_print, start_print);
	print_speed = num_symbols / print_time;

	printf("Number of characters printed: %f\n"
	       "Time to print characters: %f\n"
               "Your print speed is %f characters per second", num_symbols, print_time, print_speed);
	printf("\n");
        return 0;

}
