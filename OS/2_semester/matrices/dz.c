#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <unistd.h>

/*DATA FORMAT IN FILE:
ELEMENTS OF ONE LINE OF THE MATRIX MUST BE SEPARATED BY SPACE;
AT THE END OF EACH LINE THERE IS A LINE FEED CHARACTER,
EXCEPT OF THE LAST ROW   */

//lines[0] and cols[0] store the number of lines and columns of the first matrix
//lines[1] and cols[1] store the number of lines and columns of the second matrix

//f_matr stores the elements of the first matrix
//s_matr stores the elements of the second matrix

//f_file, s_file, t_file store file names

typedef struct{
	int lines[2], cols[2];
	long long int **f_matr, **s_matr, **res_matr;
	char *f_file, *s_file, *t_file;
}matrix;

static void *read_f_file(void *data)
{
	matrix *p = (matrix*)data;
	FILE *fp;
	int i, j, num;
	long long int c, size;
	char *filename;

//if the num is 0, then we work with the first file
//if it is 1, then we work with the second file

	for(num = 0; num < 2; num++)	
	{
		if(num == 0)
			filename = p->f_file;
		if(num == 1)
			filename = p->s_file;

		//Checking for the file existence	
	
		if((fp = fopen(filename, "r")) == NULL)
		{
			printf("Error when opening %d file\n", num+1);
			exit(0);
		}
		
		//Check if file is empty
	
		fseek(fp, 0, SEEK_END);
		if(ftell(fp) == 0)
		{
			printf("%d file is empty\n", num+1);
			exit(0);
		}	
		
		//Checking for correct file content

		fseek(fp, 0, SEEK_SET);
		c = fgetc(fp);
		while(c != EOF)
		{
			//Check for invalid character

			if(c!='-' && c!=' ' && c!='\n' && (c<'0' || c >'9'))
			{
				printf("Invalid symbol in %d file\n", num+1);
				exit(0);
			}

			//If the element is negative

			if(c == '-' && ftell(fp) != 1)
			{
				fseek(fp, -2, SEEK_CUR);	
				c = fgetc(fp);

				//Check the correctness of the symbol before the minus

				if(c == ' ' || c == '\n')
				{
					fseek(fp, 1, SEEK_CUR);
					c = fgetc(fp);

					//Check the correctness of the symbol after the minus

					if(c < '0' || c > '9')
					{
						printf("Invalid character after the minus sign in %d file\n", num+1);
						exit(0);
					}
					else
						fseek(fp, -1, SEEK_CUR);
				}
				else
				{
					printf("Invalid character before the minus sign in %d file\n", num+1);
					exit(0);
				}
			}
			
			//If the element[0][0] is negative

			if(c == '-' && ftell(fp) == 1)
			{
				//Check the correctness of the symbol after the minus

				c = fgetc(fp);
				if(c < '0' || c > '9')
				{
					printf("Invalid character after the minus sign in %d file\n", num+1);
					exit(0);
				}
				else
					fseek(fp, -1, SEEK_CUR);
			}
			
			//If the next character is space

			if(c == ' ' && ftell(fp) != 1)
			{
				fseek(fp, -2, SEEK_CUR);	
				c = fgetc(fp);

				//Check the correctness of the symbol before the space

				if(c >= '0' || c <= '9')
				{
					fseek(fp, 1, SEEK_CUR);
					c = fgetc(fp);

					//Check the correctness of the symbol after the space

					if((c < '0' || c > '9') && c != '-')
					{
						printf("Invalid character after the space in %d file\n", num+1);
						exit(0);
					}
					else
						fseek(fp, -1, SEEK_CUR);
				}

				else
				{
					printf("Invalid character before the space in %d file\n", num+1);
					exit(0);
				}

			}

			//If the space is the first character

			if(c == ' ' && ftell(fp) == 1)
			{
				printf("The space is the first character in %d file\n", num+1);
				exit(0);
			}
			c = fgetc(fp);
		}
		
		fseek(fp, 0, SEEK_SET);
		p->lines[num] = 0; 
		p->cols[num] = 0;
		c = fgetc(fp);

		//Count the number of columns

		while(c != '\n')
		{
			if(c == ' ')
				p->cols[num]++;
			c = fgetc(fp);
		}
		p->cols[num]++;	
		
		fseek(fp, 0, SEEK_SET);
		size = 0;
		c = fgetc(fp);

		//Count the number of lines


		while(c != EOF)
		{
			if(c == '\n')
				p->lines[num]++;
			if(c == ' ')
				size++;
			c = fgetc(fp);
		}

		//size stores the count of all spaces
		//the number of spaces must be equal to the product of
		//number of lines by the number of (columns-1)

		if(size != p->lines[num]*(p->cols[num]-1))
		{
			printf("Check the number of elements in %d file\n", num+1);
			exit(0);
		}

		if(p->lines[num] == 0)
		{
			printf("Number of lines in the %d file is equal zero\n", num+1);
			exit(0);
		}
		
		long long int **matrix = (long long int**)malloc(p->lines[num]*sizeof(long long int));
		for(i = 0; i < p->lines[num]; i++)
			matrix[i] = (long long int*)malloc(p->cols[num]*sizeof(long long int));

		//Read matrix elements

		fseek(fp, 0, SEEK_SET);
		for(i = 0; i < p->lines[num]; i++)
		{
			for(j = 0; j < p->cols[num]; j++)
				fscanf(fp, "%lld ", &matrix[i][j]);
		} 
	
		if(num == 0)
		{
			p->f_matr = (long long int**)malloc(p->lines[num]*sizeof(long long int));
			for(i = 0; i < p->lines[num]; i++)
				p->f_matr[i] = (long long int*)malloc(p->cols[num]*sizeof(long long int));
			for(i = 0; i < p->lines[num]; i++)
				for(j = 0; j < p->cols[num]; j++)
					p->f_matr[i][j] = matrix[i][j];
		}
		if(num == 1)
		{
			p->s_matr = (long long int**)malloc(p->lines[num]*sizeof(long long int));
			for(i = 0; i < p->lines[num]; i++)
				p->s_matr[i] = (long long int*)malloc(p->cols[num]*sizeof(long long int));
			for(i = 0; i < p->lines[num]; i++)
				for(j = 0; j < p->cols[num]; j++)
					p->s_matr[i][j] = matrix[i][j];
		}

		for(i = 0; i < p->lines[num]; i++)
			free(matrix[i]);
		free(matrix);

		fclose(fp);
	}
	printf("The first function is performed correctly\n");
	return NULL;
}


static void *mul_func(void *data)
{
	matrix *p = (matrix*)data;
	int i, j, s;
	
	if(p->cols[0] != p->lines[1])
	{
		printf("Number of columns of first matrix is not equal to the number lines of second matrix\n");
		exit(0);
	}	

	p->res_matr = (long long int**)malloc(p->lines[0]*sizeof(long long int));
	for(i = 0; i < p->lines[0]; i++)
		p->res_matr[i] = (long long int*)malloc(p->cols[1]*sizeof(long long int));

	//Multiply matrices

	for(i = 0; i < p->lines[0]; i++)
	{
		for(j = 0; j < p->cols[1]; j++)
		{
			p->res_matr[i][j] = 0;
			for(s = 0; s < p->cols[0]; s++)
				p->res_matr[i][j] += (p->f_matr[i][s])*(p->s_matr[s][j]);
		}
	}

	printf("The second function is performed correctly\n");
	return NULL;
}

static void *write_func(void *data)
{
	matrix *p = (matrix*)data;
	FILE *ff;
	int i, j;
	if((ff = fopen(p->t_file, "w")) == NULL)
	{
		printf("Error when opening third file\n");
		exit(0);
	}

	//Write the resulting matrix to the third file

	for(i = 0; i < p->lines[0]; i++)
	{
		sleep(0.1);
		for(j = 0; j < p->cols[1]; j++)
			fprintf(ff, "%lld ", p->res_matr[i][j]);
		fprintf(ff, "%c", 0x0A);
	}

	fclose(ff);
	printf("The third function is performed correctly\n");
	return NULL;
}

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		printf("Please, use format %s <file1.txt> <file2.txt> <newfile.txt>\n", argv[0]);
		return -1;
	}
	
	int i, j;
	
	pthread_t* pid = (pthread_t*)malloc(4* sizeof(pthread_t));
	matrix thread_data;
	
	thread_data.f_file = argv[1];
	thread_data.s_file = argv[2];
	thread_data.t_file = argv[3];
     
	void *retval_1, *retval_2, *retval_3;
	pthread_create(&(pid[0]), NULL, read_f_file, &thread_data);
	pthread_join(pid[0], &retval_1);

	pthread_create(&(pid[1]), NULL, mul_func, &thread_data);
	pthread_create(&(pid[2]), NULL, write_func, &thread_data);
	pthread_join(pid[1], &retval_2);
	pthread_join(pid[2], &retval_3);

	free(pid);	
	printf("The Main function is performed correctly\n");
	return 0;
}
