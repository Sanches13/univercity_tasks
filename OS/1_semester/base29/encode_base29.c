#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

int main(int argc, char* argv[])
{
	long long int sum, n, s;
	int i, j, max, *bin;
	char str[64], end[128], alf[] = {'0','1','2','3','4','5','6',
		               '7','8','9','A','B','C','D','E','F',
			       'G','H', 'I', 'J','K','L','M','N','O','P',
			       'Q','R','S'};
	
        if ( argc != 2 ) 
	{
		printf("Please, use format: <./exe> <file_name>\n");
		return -1;
	}

	FILE* fp;
	fp = fopen(argv[1], "r");
	if(fp == NULL)
	{
		printf("File could not be opened");
		return -2;
	}
	else
		printf("File opened!\n");
	
	printf("File contents: ");

	max = 0;
	while(!feof(fp) && max<64)
	{
	    str[max] = fgetc(fp);
	    if(str[max]==EOF)
	    {
		    printf("File reading finished\n");
		    max--;
		    break;
	    }
	    printf("%c", str[max]);
	    max++;
	}
	printf("Number of symbols in the file: %d\n", max);
	
	bin = (int*)malloc(max*8*sizeof(int));
	j = 0;
	for(i = 0;i < max; i++)
	{	
	    for(s = 7; s >= 0; s--)
	    {
                 if(str[i] % 2 == 0)
		 {
			bin[j+s] = 0;
			str[i] = str[i] / 2;
		 }
		 else
		 {
			 bin[j+s] = 1;
			 str[i] = str[i] / 2;
	         }

	    }
	    j = j + 8;
	}

	max = max*8;
        sum = 0;
	n = max - 1;
	for(i = 0;i < max;i++)
	{
                s = 1;
                for(j = 0; j < n; j++)
		{
			s = s * 2;
		}
                sum = sum + bin[i] * s;
                n--;		
        }
        printf("Decimal representation of the file content: %lld\n", sum);
	
	max = 0;
	for(i = 0; sum > 0; i++)
	{
                s = sum % 29;
		end[i] = alf[s];
		sum = sum / 29;
		max++;
	}

	printf("Representation of file content in Base29 encoding:\n");
	for(i = max - 1; i >= 0; i--)
	{
		printf("%c", end[i]);
	}
	printf("\n");

	free(bin);
	fclose(fp);
	return 0;
}
