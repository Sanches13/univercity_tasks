#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char* argv[])
{
	long long int sum, q;
	int i, j, max, dec[64], end[128];
	char str[64], alf[] = {'0','1','2','3','4','5','6',
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
        while(!feof(fp) && max < 64)
	{
		str[max]=fgetc(fp);
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
        
	printf("Index of symbol in alphabet: ");
	for(i = 0;i < max;i++)
	{
		for(j = 0;j < 29;j++)
		{
			if(str[i]==alf[j])
			{
				dec[i]=j;
				printf("%d ", dec[i]);
			}
			
		}
	}
	printf("\n");
        
	q = 0;
	sum = 0;
	for(i = 0; i < max; i++)
	{
		sum = q*29+dec[i];
		q = sum;
	}
	printf("Decimal representation of string: %lld\n", sum);

	max = 0;
	while(sum > 0)
	{
		end[max] = sum % 256;
	        sum = sum / 256;	
		max++;
	}
        
	printf("Result: ");
	for(i=max-1;i>=0;i--)
	{
		printf("%c", end[i]);
	}

	printf("\n");
	fclose(fp);
	return 0;
}
