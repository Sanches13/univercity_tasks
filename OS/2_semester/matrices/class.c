#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

/*DATA FORMAT IN FILE:
ELEMENTS OF ONE LINE OF THE MATRIX MUST BE SEPARATED BY SPACE;
AT THE END OF EACH LINE THERE IS A LINE FEED CHARACTER,
EXCEPT OF THE LAST ROW   */

typedef struct{
	int size;	
	long long int **first, **second;
	char *filename;
}data;

static void *mul_func(void *thread_data)
{
	data *d = (data*)thread_data;
	int i, j, s;
	for(i = 0; i < d->size; i++)
	{
		for(j = 0; j < d->size; j++)
		{
			d->second[i][j] = 0;
            		for(s = 0; s < d->size; s++)
                		d->second[i][j] += (d->first[i][s])*(d->first[s][j]); 
		}
	}
	printf("Message from potok1\n");
        return NULL;
}

static void *write_func(void *thread_data)
{
	data *d = (data*)thread_data;
	int i, j;
	FILE *fd;
	fd = fopen(d->filename, "w");
	for(i = 0; i < d->size; i++)
	{
		sleep(0.5);
		for(j = 0; j < d->size; j++)
			fprintf(fd, "%10lld", d->second[i][j]);
		fprintf(fd, "%c", 0x0A);
	}
	printf("Message from potok2\n");
	fclose(fd);
	return NULL;
}

int main(int argc, char* argv[])
{
	if(argc != 3)
	{
		printf("Please, use format <.exe> <file1.txt> <file2.txt>\n");
		return -1;
	}	

	int i, j, n, s;
	long long int c;
	FILE *fp;
	if((fp = fopen(argv[1], "r")) == NULL)
	{
		printf("File is not found\n");
		return -1;
	}

	n = 0; 
	c = fgetc(fp);
	while(c != 0x0A){
		if(c == ' ')
			n++;
		c = fgetc(fp);
	}	
	n++;
	
	long long int** matrix = (long long int**)malloc(n*sizeof(long long int));
	long long int** res = (long long int**)malloc(n*sizeof(long long int));
	for(i = 0; i < n; i++)
	{
		matrix[i] = (long long int*)malloc(n*sizeof(long long int));
		res[i] = (long long int*)malloc(n*sizeof(long long int));
	}
	
	fseek(fp, 0, SEEK_SET);
	for(i = 0; i < n; i++)
	{
		for(j = 0; j < n; j++)
		{
			fscanf(fp, "%lld ", &matrix[i][j]);
			/*fscanf(fp, "%lld", &c);
            		if((c != 0x0A) && (c != ' '))
                		matrix[i][j] = c;*/
        	}
        } 
	
	data d;
	d.size = n;
	d.first = matrix;
	d.second = res;
	d.filename = argv[2];
	pthread_t pid_1, pid_2;

    	void *retval_1, *retval_2;
    	pthread_create(&pid_1, NULL, mul_func, &d);
	pthread_create(&pid_2, NULL, write_func, &d);
    	
	pthread_join(pid_1, &retval_1);
    	pthread_join(pid_2, &retval_2);

	for(i = 0; i < n; i++)
	{
		free(matrix[i]);
		free(res[i]);
	}

	free(matrix);
	free(res);
	fclose(fp);
	return 0;
}
