#include <stdio.h>
#include <linux/input.h>
#include <sys/time.h>

int main()
{
	struct input_event q[6];
	FILE *f = fopen("/dev/input/event2", "rb");
	if(f==NULL)
	{
		printf("Error\n");
		return -1;
	}
	int i, code, j, begin, end;
	i = 0;
	j = 1;
        printf("Press the key ESC to end the program\n");
	struct timeval time;
	while(1)
	{
		fread(&q, sizeof(struct input_event), 6, f);
		for(i = 0; i < 6; i++)
		{ 
			if(j==1 && i > 2)
				begin = q[i].time.tv_sec;

			if(q[i].code == 1)
			{
				if(j==1)
				{
					printf("No keys pressed\n");
					return 0;
				}
				printf("\n");
				end = q[i-2].time.tv_sec;
              			j = j - 1;
				end = end - begin;
				printf("Number of characters printed: %d\n", j);
				printf("Time to print characters: %d\n", end);
				printf("Your print speed is %f\n", ((float) j / end));
				return 0;
			}
		       
			printf("Code: %d, ", q[i].code);
			printf("Number of symbols: %d, ", j);
		        printf("Time: %ld\n", q[i].time.tv_sec);
		}
		printf("-------------\n");
		j++;
	}
	return 0;
}
