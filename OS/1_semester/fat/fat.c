#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct dirs{
        char dir[11];
};

unsigned int word(unsigned char f_byte, unsigned char s_byte)
{
	unsigned int your_word;
	your_word = 0;
	your_word = (((your_word | s_byte) << 8) | f_byte);
	return your_word;
}

int dword(unsigned char f_byte, unsigned char s_byte, unsigned char t_byte, unsigned char fo_byte)
{
	int your_dword;
	your_dword = 0;
	your_dword = (((((((your_dword | fo_byte) << 8) | t_byte) << 8) | s_byte) << 8) | f_byte);
	return your_dword;
}

int print_boot(int sect_size, unsigned int fat_size, unsigned int root_entr,unsigned int res_sect, unsigned char fat_num, int sect_in_clus, char boot[])
{
	int i;
	printf("Sector size: %d bytes\n"
	       "Cluster size: %d sectors or %d bytes\n"
	       "Reserved sectors: %d sector\n"
	       "Number of tables FAT: %d tables\n"
	       "Number of entries in root directory: %d entries\n"
	       "Table FAT size: %d sectors or %d bytes\n"
	       "File system type: ", sect_size, sect_in_clus, sect_in_clus*sect_size, res_sect, fat_num, root_entr, fat_size, fat_size*sect_size);
	for(i = 54; i < 60; i++)
		printf("%c", boot[i]);
	printf("\n");
        return 0;
}

int file_addr(int ind, FILE *fp, unsigned int root_pos, unsigned int data_pos, unsigned int root_entr, int sect_size, int sect_in_clus, unsigned char *fat)
{
        unsigned char f_byte, s_byte, t_byte, fo_byte;
        unsigned int first_clus;
        int i, j, l, t, s, num, addr, size, clus_num, r;
        char buf[11], filename[12]; 
        printf("Enter number of files and directories:\n");
	scanf("%d", &num);

	if(num <= 0)
	{
		printf("You entered an invalid value\n");
		return -2;
	}

        struct dirs f[num];
	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 11; j++)
			f[i].dir[j] = 0x20;
	}

        for(i = 0; i < 12; i++)
                filename[i] = 0x20;

	for(i = 0; i < num-1; i++)
        {
                printf("Enter the %d directory: ", i+1);
         	scanf("%s", f[i].dir);
        }

        printf("Enter the file name: ");
	scanf("%s", filename);

        for(i = 0; i < num-1; i++)
        {
                for(j = 0; j < 11; j++)
                {
                        if(f[i].dir[j] == 0x00)
                                f[i].dir[j] = 0x20;
                }
        }

	for(j = 0; j < 12; j++)
	{
		if(filename[j] == 0x00)
			filename[j] = 0x20;
	}

        t = 0;
	for(j = 0; j < 12 && t == 0; j++)
	{
		if(filename[j] == '.')
		{
                        t++;
			f[num-1].dir[8] = filename[j+1];
			f[num-1].dir[9] = filename[j+2];
			f[num-1].dir[10] = filename[j+3];
			for(l = j; l < 8; l++)
				f[num-1].dir[l] = 0x20;
		}
	}	
        for(l = 0; l < j-1 && l < strlen(filename); l++)
                f[num-1].dir[l] = filename[l];

        if(ind == 1)
                f[num-1].dir[0] = 0xe5;

	s = 0;
        for(i = 0; i < root_entr && s != 11; i++)
	{
		s = 0;
		fseek(fp, root_pos+i*32, SEEK_SET);

		for(j = 0; j < 11; j++)
			buf[j] = fgetc(fp);

		for(j = 0; j < 11; j++)
		{
			if(buf[j] == f[0].dir[j])
				s++;
		}

		if(s == 11)
			addr = root_pos+i*32;
	}
	if(s != 11)
	{
		printf("No file in root directory\n");
		return -2;
	}

	s = 0;
	for(i = 1; i < num; i++)
	{
		fseek(fp, addr+26, SEEK_SET);
		f_byte = fgetc(fp);
		s_byte = fgetc(fp);
		first_clus = word(f_byte, s_byte);
		f_byte = fgetc(fp);
		s_byte = fgetc(fp);
		t_byte = fgetc(fp);
		fo_byte = fgetc(fp);
		size = dword(f_byte, s_byte, t_byte, fo_byte);
		clus_num = size / (sect_in_clus*sect_size);
		r = size % (sect_in_clus*sect_size);

		for(j = 0; j < clus_num && s != 11; j++)
		{        
			for(t = 0; t < sect_size*sect_in_clus/32 && s!=11; t++)
			{
				s = 0;
				fseek(fp, data_pos + (first_clus-2)*sect_in_clus*sect_size + j * 32, SEEK_SET);

				for(l = 0; l < 11; l++)
					buf[l] = fgetc(fp);

				for(l = 0; l < 11; l++)
				{
					if(buf[l] == f[i].dir[l])
						s++;
				}

				if(s == 11)
					addr = data_pos+(first_clus-2)*sect_in_clus*sect_size+j*32;
			}
			first_clus = fat[first_clus];
		}

		for(t = 0; t < r / 32 && s != 11; t++)
		{
			s = 0;
			fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+t*32, SEEK_SET);
			for(l = 0; l < 11; l++)
				buf[l] = fgetc(fp);

			for(l = 0; l < 11; l++)
			{
				if(buf[l] == f[i].dir[l])
					s++;
			}

			if(s==11)
				addr = data_pos+(first_clus-2)*sect_size*sect_in_clus+t*32;
		}

		if(s != 11)
		{
			printf("File or directory not found\n");
			return -2;
		}
	}
        return addr;
}

int print_file(FILE *fp, int addr, unsigned int data_pos, int sect_size, int sect_in_clus, unsigned char *fat)
{
	unsigned char f_byte, s_byte, t_byte, fo_byte;
	unsigned int first_clus;
	int i, j, size, clus_num, r;
	fseek(fp, addr+26, SEEK_SET);
	f_byte = fgetc(fp);
	s_byte = fgetc(fp);
	first_clus = word(f_byte, s_byte);
	f_byte = fgetc(fp);
	s_byte = fgetc(fp);
	t_byte = fgetc(fp);
	fo_byte = fgetc(fp);
	size = dword(f_byte, s_byte, t_byte, fo_byte);
	clus_num = size / (sect_size*sect_in_clus);
	r = size % (sect_in_clus*sect_size);

	for(i = 0; i < clus_num; i++)
	{
		fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus, SEEK_SET);
		for(j = 0; j < sect_size*sect_in_clus; j++)
			printf("%x ", fgetc(fp));
		first_clus = fat[first_clus];
	}
	fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus, SEEK_SET);
	for(i = 0; i < r; i++)
		printf("%x ", fgetc(fp));
	printf("\n");
        return 0;
}

int write_file(FILE *fp, unsigned char fat_num, unsigned int fat_pos, unsigned int fat_size,unsigned int data_pos, unsigned int root_pos, unsigned int root_entr, int sect_size, int sect_in_clus, unsigned char *fat)
{
	unsigned char byte, f_byte, s_byte, t_byte, fo_byte;
	unsigned int first_clus;
	int i, j, size, clus_num, r, num, t, s, l, addr, atr;
        char buf[11], filename[12]; 
        printf("Enter number of files and directories:\n");
	scanf("%d", &num);

	if(num <= 0)
	{
		printf("You entered an invalid value\n");
		return -2;
	}

        struct dirs f[num];
	for(i = 0; i < num; i++)
	{
		for(j = 0; j < 11; j++)
			f[i].dir[j] = 0x20;
	}
        for(i = 0; i < 12; i++)
                filename[i] = 0x20;
	for(i = 0; i < num-1; i++)
        {
                printf("Enter the %d directory: ", i+1);
         	scanf("%s", f[i].dir);
        }
        printf("Enter the file name: ");
	scanf("%s", filename);

        for(i = 0; i < num-1; i++)
        {
                for(j = 0; j < 11; j++)
                {
                        if(f[i].dir[j] == 0x00)
                                f[i].dir[j] = 0x20;
                }
        }

	for(j = 0; j < 12; j++)
        {
		if(filename[j] == 0x00)
			filename[j] = 0x20;
	}

        t = 0;
	for(j = 0; j < 12 && t == 0; j++)
	{
		if(filename[j] == '.')
		{
                        t++;
			f[num-1].dir[8] = filename[j+1];
			f[num-1].dir[9] = filename[j+2];
			f[num-1].dir[10] = filename[j+3];
			for(l = j; l < 8; l++)
				f[num-1].dir[l] = 0x20;
		}
	}	
        for(l = 0; l < j-1 && l < strlen(filename); l++)
                f[num-1].dir[l] = filename[l];

        s = 0;
        if(num > 1){
	for(i = 0; i < root_entr && s != 11; i++)
	{
		s = 0;
		fseek(fp, root_pos+i*32, SEEK_SET);
		for(j = 0; j < 11; j++)
			buf[j] = fgetc(fp);
		for(j = 0; j < 11; j++)
		{
			if(buf[j] == f[0].dir[j])
				s++;
		}
		if(s == 11)
		        addr = root_pos+i*32;
	}

	if(s != 11)
	{
		printf("No file in root directory\n");
		return -2;
	}

	        for(i = 1; i < num-1; i++)
	        {
		        fseek(fp, addr+26, SEEK_SET);
		        f_byte = fgetc(fp);
		        s_byte = fgetc(fp);
		        first_clus = word(f_byte, s_byte);
		        f_byte = fgetc(fp);
		        s_byte = fgetc(fp);
		        t_byte = fgetc(fp);
		        fo_byte = fgetc(fp);
		        size = dword(f_byte, s_byte, t_byte, fo_byte);
		        clus_num = size / (sect_in_clus*sect_size);
		        r = size % (sect_in_clus*sect_size);
			s = 0;

		        for(j = 0; j < clus_num && s != 11; j++)
		        {        
			        for(t = 0; t < sect_size*sect_in_clus/32 && s!=11; t++)
			        {
				        s = 0;
				        fseek(fp, data_pos + (first_clus-2)*sect_in_clus*sect_size + t * 32, SEEK_SET);
			
					for(l = 0; l < 11; l++)
						buf[l] = fgetc(fp);
			
					for(l = 0; l < 11; l++)
				        {
					        if(buf[l] == f[i].dir[l])
					        	s++;
				        }
			
					if(s == 11)
					        addr = data_pos+(first_clus-2)*sect_in_clus*sect_size+t*32;
			        }
			        first_clus = fat[first_clus];
		        }
			
			s = 0;
		        for(t = 0; t < r / 32 && s != 11; t++)
		        {
			        s = 0;
			        fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+t*32, SEEK_SET);
			
				for(l = 0; l < 11; l++)
			        	buf[l] = fgetc(fp);
			
				for(l = 0; l < 11; l++)
			        {
					if(buf[l] == f[i].dir[l])
					s++;
			        }
			
				if(s==11)
			        	addr = data_pos+(first_clus-2)*sect_size*sect_in_clus+t*32;
		        }

			if(s != 11)
			{
				printf("Directory not found\n");
				return -2;
			}
	        }

        fseek(fp, addr + 26, SEEK_SET);
	f_byte = fgetc(fp);
        s_byte = fgetc(fp);
        first_clus = word(f_byte, s_byte);
        f_byte = fgetc(fp);
        s_byte = fgetc(fp);
        t_byte = fgetc(fp);
 	fo_byte = fgetc(fp);
	size = dword(f_byte, s_byte, t_byte, fo_byte);  			        
	clus_num = size / (sect_size*sect_in_clus);
        r = size % (sect_in_clus*sect_size);
	s = 0;
	for(i = 0; i < clus_num && s != 11; i++)
	{
                for(j = 0; j < sect_size*sect_in_clus/32 && s != 11; j++)
		{
			fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+j*32, SEEK_SET);
			s = 0;

			for(l = 0; l < 11; l++)
			{
				byte = fgetc(fp);
				if(byte == 0x00)
					s++;
			}

			if(s==11)
			{
				fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+j*32,SEEK_SET);
				for(l = 0; l < 11; l++)
					fputc(f[num-1].dir[l], fp);
				printf("Enter atribute of file:\n"
				       "1. Only for reading;\n"
				       "2. Hidden;\n"
				       "3. System;\n"
				       "4. Directory;\n"
				       "5. Archival.\n");
				scanf("%d", &atr);
				switch (atr){
					case 1:
						byte = 0x01;
						break;
					case 2:
						byte = 0x02;
						break;
					case 3:
						byte = 0x04;
						break;
					case 4:
						byte = 0x10;
						break;
					case 5:
						byte = 0x20;
						break;
				}
				fputc(byte, fp);
				fseek(fp, 17, SEEK_CUR);
				fputc(0x01, fp);
				fseek(fp, -4, SEEK_CUR);
				t = 0;
				for(l = 0; l < fat_size*sect_size, t==0; l++)
				{
					if(fat[l] == 0x00)
					{
						fputc(l, fp);
						fat[l] = 0xff;
						t++;
					}
				}
			}
			first_clus = fat[first_clus];
		}
        }
	s = 0;
		for(i = 0; i < r/32 && s != 11; i++)
		{
                        fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+i*32, SEEK_SET);
			s = 0;
			for(l = 0; l < 11; l++)
			{
				byte = fgetc(fp);
				if(byte == 0x00)
					s++;
			}

			if(s==11)
			{
				fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+i*32,SEEK_SET);
				for(l = 0; l < 11; l++)
					fputc(f[num-1].dir[l], fp);
				printf("Enter atribute of file.\n"
                                       "1. Only for reading\n"
                                       "2. Hidden\n"
                                       "3. System\n"
                                       "4. Directory\n"
                                       "5. Archival\n");
                                scanf("%d", &atr);
                                switch (atr){
                                        case 1:
                                                byte = 0x01;
                                                break;
                                        case 2:
                                                byte = 0x02;
                                                break;
                                        case 3:
                                                byte = 0x04;
                                                break;
                                        case 4:
                                                byte = 0x10;
                                                break;
                                        case 5:
                                                byte = 0x20;
                                                break;
                                }
                                fputc(byte, fp);
				fseek(fp, 17, SEEK_CUR);
				fputc(0x01, fp);
				fseek(fp, -4, SEEK_CUR);
				t = 0;
				for(l = 0; l < fat_size*sect_size, t==0; l++)
				{
					if(fat[l] == 0x00)
					{
						fputc(l, fp);
						fat[l] = 0xff;
						t++;
					}
				}
			}
		}
		fseek(fp, fat_pos, SEEK_SET);
		for(i = 0; i < fat_num; i++)
		{
			for(j = 0; j < fat_size*sect_size; j++)
				fputc(fat[j], fp);
		}

		if(s != 11)
		{
			printf("Failed to write file, no free space\n");
			return -2;
		}
	}

	if(num == 1)
	{
		s = 0;
		for(i = 0; i < root_entr && s != 11; i++)
		{
			s = 0;
			fseek(fp, root_pos + i*32, SEEK_SET);
			for(l = 0; l < 11; l++)
			{
				byte = fgetc(fp);
				if(byte == 0x00)
					s++;
			}
			if(s == 11)
			{
				fseek(fp, root_pos+i*32, SEEK_SET);
				for(l = 0; l < 11; l++)
					fputc(f[0].dir[l], fp);
				printf("Enter atribute of file.\n"
                                       "1. Only for reading\n"
                                       "2. Hidden\n"
                                       "3. System\n"
                                       "4. Directory\n"
                                       "5. Archival\n");
                                scanf("%d", &atr);
                                switch (atr){
                                        case 1:
                                                byte = 0x01;
                                                break;
                                        case 2:
                                                byte = 0x02;
                                                break;
                                        case 3:
                                                byte = 0x04;
                                                break;
                                        case 4:
                                                byte = 0x10;
                                                break;
                                        case 5:
                                                byte = 0x20;
                                                break;
                                }
                                fputc(byte, fp);
				fseek(fp, root_pos+29+i*32, SEEK_SET);
				fputc(0x01, fp);
				fseek(fp, root_pos+i*32+26, SEEK_SET);
				t = 0;
				for(l = 0; l < fat_size*sect_size, t == 0; l++)
				{
					if(fat[l] == 0x00)
					{
						fputc(l, fp);
						fat[l] = 0xff;
						t++;
					}
				}
		       }
		}
		fseek(fp, fat_pos, SEEK_SET);
		for(i = 0; i < fat_num; i++)
		{
			for(j = 0; j < fat_size*sect_size; j++)
				fputc(fat[j], fp);
		}
		if(s != 11)
		{
			printf("Failed to write file, no free space\n");
			return -2;
		}
	}
        return 0;
}

int print_dir(int ind, FILE *fp, unsigned int first_clus, int size, int sect_size, int sect_in_clus, unsigned int data_pos, unsigned char *fat)
{
	unsigned char atr, f_byte, s_byte, t_byte, fo_byte, name[11];
	int i, clus_num, r, j, l;
	clus_num = size / (sect_in_clus*sect_size);
	r = size % (sect_in_clus*sect_size);
	for(i = 0; i < clus_num; i++)
	{
		for(j = 0; j < sect_size*sect_in_clus/32; j++)
		{
			fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+ j * 32, SEEK_SET);

			for(l = 0; l < 11; l++)
				name[l] = fgetc(fp);
			atr = fgetc(fp);

			if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] != 0xe5 && ind == 0)
			{
				for(l = 0; l < 11; l++)
					printf("%c", name[l]);
				printf("\n");
			}

                        if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] == 0xe5 && ind == 1)
			{
				for(l = 1; l < 11; l++)
					printf("%c", name[l]);
				printf("\n");
			}

                        if(atr == 0x10 && name[0] == 0xe5 && ind == 1)
                        {
                                for(l = 1; l < 11; l++)
                                        printf("%c", name[l]);
                                printf("\n");
                        }

			if(atr == 0x10 && name[0] != 0xe5)
			{
                                printf("_______________\n"
                                       "               |\n");
		
				for(l = 0; l < 11; l++)
					printf("%c", name[l]);
		
				printf("\n");
				fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+j*32+26, SEEK_SET);
				f_byte = fgetc(fp);
				s_byte = fgetc(fp);
		                first_clus = word(f_byte, s_byte);	
				f_byte = fgetc(fp);						                
                                s_byte = fgetc(fp);	
			        t_byte = fgetc(fp);
				fo_byte = fgetc(fp);			
				size = dword(f_byte, s_byte, t_byte, fo_byte);			                
                                print_dir(ind, fp, first_clus, size, sect_size, sect_in_clus, data_pos, fat);
                                printf("_______________|\n");
			}
		}
		first_clus = fat[first_clus];
	}
	
	for(i = 0; i < r/32; i++)
	{
		fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+i*32, SEEK_SET);
        
		for(l = 0; l < 11; l++)
				name[l] = fgetc(fp);

		atr = fgetc(fp);
		if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] != 0xe5 && ind == 0)
		{
			for(l = 0; l < 11; l++)					                                printf("%c", name[l]);
			printf("\n");
		}

		if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] == 0xe5 && ind == 1)
		{
			for(l = 1; l < 11; l++)					                                printf("%c", name[l]);
			printf("\n");
		}

		if(atr == 0x10 && name[0] == 0xe5 && ind == 1)
		{
			for(l = 1; l < 11; l++)
				printf("%c", name[l]);
			printf("\n");
		}

		if(atr == 0x10 && name[0] != 0xe5)
		{
			printf("_______________\n"
 			       "               |\n");

			for(l = 0; l < 11; l++)
				printf("%c", name[l]);
			printf(":\n");

			fseek(fp, data_pos+(first_clus-2)*sect_size*sect_in_clus+i*32+26, SEEK_SET);
			f_byte = fgetc(fp);
			s_byte = fgetc(fp);
			first_clus = word(f_byte, s_byte);	
			f_byte = fgetc(fp);						                s_byte = fgetc(fp);	
			t_byte = fgetc(fp);
			fo_byte = fgetc(fp);			
			size = dword(f_byte, s_byte, t_byte, fo_byte);
			print_dir(ind, fp, first_clus, size, sect_size, sect_in_clus, data_pos, fat);				
                                printf("_______________|\n");
			}
	}
        return 0;
}

int print_tree(int ind, FILE *fp, unsigned int root_pos, unsigned int root_entr, unsigned char *fat, unsigned int data_pos, int sect_size, int sect_in_clus)
{
        unsigned int first_clus;
        unsigned char atr, f_byte, s_byte, t_byte, fo_byte, name[11];
        int i, j, l, size;
        for(i = 0; i < root_entr; i++)
        {
	        fseek(fp, root_pos+i*32,SEEK_SET);
	        for(j = 0; j < 11; j++)
		        name[j] = fgetc(fp);
	        atr = fgetc(fp);

		if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] != 0xe5 && ind == 0)
	        {
	        	for(j = 0; j < 11; j++)
	        		printf("%c", name[j]);
        		printf("\n");
        	}	

		if((atr == 0x01 || atr == 0x02 || atr == 0x04 || atr == 0x08 || atr == 0x20) && name[0] == 0xe5 && ind == 1)
	        {
	        	for(j = 1; j < 11; j++)
	        		printf("%c", name[j]);
        		printf("\n");
        	}	

                if(atr == 0x10 && name[0] == 0xe5 && ind == 1)
                {
                        for(j = 1; j < 11; j++)
	        		printf("%c", name[j]);
        		printf("\n");
                }

		if(atr == 0x10 && name[0] != 0xe5)
        	{
			printf("_______________\n"
			       "               |\n");

			for(j = 0; j < 11; j++)
	        		printf("%c", name[j]);
		        printf(":\n");
		        fseek(fp, root_pos+i*32+26, SEEK_SET);
		        f_byte = fgetc(fp);
		        s_byte = fgetc(fp);
		        first_clus = word(f_byte, s_byte);
		        f_byte = fgetc(fp);
		        s_byte = fgetc(fp);
		        t_byte = fgetc(fp);
		        fo_byte = fgetc(fp);
		        size = dword(f_byte, s_byte, t_byte, fo_byte);
                        print_dir(ind, fp, first_clus, size, sect_size, sect_in_clus, data_pos, fat);
                        printf("_______________|\n");
        	}        
        }
        return 0;
}

int file_recovery(FILE *fp, int addr, unsigned char *fat, unsigned int fat_pos, unsigned char fat_num, unsigned int fat_size, int sect_size)
{
	int i, j;
	unsigned int first_clus;
	unsigned char f_byte, s_byte;
	fseek(fp, addr, SEEK_SET);
	fputc(0x4b, fp);
	fseek(fp, addr+26, SEEK_SET);
	f_byte = fgetc(fp);
	s_byte = fgetc(fp);
	first_clus = word(f_byte, s_byte);
	fat[first_clus] = 0xff;

	fseek(fp, fat_pos, SEEK_SET);
	for(i = 0; i < fat_num; i++)
	{
		for(j = 0; j < fat_size*sect_size; j++)
			fputc(fat[j], fp);
	}
        return 0; 
}

int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		printf("Please, use format <.exe> <.img> to perform operations or <.exe> for information about the program\n");
		return -1;
	}
        
        if(argc == 1)
	{
		printf("You decided to look at the instructions for using this program.This program can perform 7 operations on digital media image with fat12 file system. To use the operations, the second argument must be the name of the digital media image. The following operations have been implemented:\n"
		       "1. Getting information about the file system. To use this function, you need to enter the number 1 when a message with a set of operations is displayed.\n"
		       "2. Getting a tree of directories / files. To use this function, you need to enter the number 2.\n"
		       "3. Reading the file. To use this function, enter the number 3. This will display a message asking you to enter the number of files and directories. This number is equal to the number of files and directories in the full file name. For example, we need to read the file /dir1/dir2/file1.txt. In this situation, you need to enter the number 3. Then a message will appear that asks you to enter the names of the directories and the desired file. In our situation, first you need to enter dir1 and press the ENTER key, then enter dir2 and press the ENTER key, enter file1.txt and press the ENTER key. If the files exist, the names are entered correctly, then the contents of the file we need will be displayed on the screen.\n"
		       "4. Writing a file. To use this function, enter the number 4. As in the previous paragraph, you need to enter the number of files and directories, and then enter the names of the directories and the new file. If the directories exist, the names are entered correctly, the program will write the file correctly.\n"
		       "5. Obtaining a list of deleted objects. To use this function, enter the number 5. After that, a list of deleted objects will be displayed.\n"
		       "6. Receiving data from a remote object. To use this function, enter the number 6. Then, in the same way as in steps 3 and 4, enter the number of files and directories from the full file name. Then enter the names of the directories and the file you want one by one. If everything is done correctly, the contents of the deleted file will be displayed on the screen.\n"
		       "7. Recovering a deleted object. To use this function, enter the number 7. Then, in the same way as in steps 3, 4, 6, enter the number of directories and files from the full file name, then the names of directories and files. If everything is done correctly, the specified file will be restored.\n");
		return 0;
	}	
	int i, addr, sect_size, sect_in_clus, number;
	unsigned char boot[62], j, fat_num, *fat, *root;
	unsigned int fat_size, root_entr, res_sect, root_pos, data_pos, fat_pos;
        FILE *fp;
	fp = fopen(argv[1], "r+");
	if(fp == NULL)
	{
		printf("Please, check the entered file name and its existence\n");
		return -1;
	}
        for(i = 0; i < 64; i++)
	{
		j = fgetc(fp);
		if(i < 62)
		{
			boot[i] = j;
		}
	}
 
        if(boot[54] != 0x46 || boot[55] != 0x41 || boot[56] != 0x54)
        {
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }

        if(boot[21] != 0xf0 && boot[21] != 0xf8)
        {     
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }
        
        if(boot[37] != 0x00)
        {     
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }
        fseek(fp, 510, SEEK_SET);
        if(fgetc(fp) != 0x55)
        {     
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }
        if(fgetc(fp) != 0xaa)
        {     
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }
        sect_size = word(boot[11], boot[12]);
	res_sect = word(boot[14], boot[15]);
	root_entr = word(boot[17], boot[18]); 
	fat_size = word(boot[22], boot[23]);
	sect_in_clus = boot[13]; 
	fat_num = boot[16];
        
	fat = (unsigned char*)malloc(fat_size*sect_size*sizeof(unsigned char));
	fat_pos = sect_size*res_sect;
	fseek(fp, fat_pos, SEEK_SET);
	for(i = 0; i < fat_size*sect_size; i++)
		fat[i] = fgetc(fp);

	root = (unsigned char*)malloc(root_entr*32*sizeof(unsigned char));
	root_pos = sect_size*res_sect + fat_num*fat_size*sect_size;
	fseek(fp, root_pos, SEEK_SET);
	for(i = 0; i < root_entr*32; i++)
		root[i] = fgetc(fp);
	data_pos = root_pos+root_entr*32;
	printf("Choose operation. Enter a number of operation\n"
	       "1. Getting information about the file system.\n"
	       "2. Getting a tree of directories/files.\n"
	       "3. Reading file.\n"
	       "4. Writing file.\n"
	       "5. Getting a list of deleted objects.\n"
	       "6. Retrieving remote object data.\n"
	       "7. Recovering a deleted object.\n"
	       "Enter any other character to end the program\n");
	scanf("%d", &number);
	switch (number){
		case 1:
		{
			print_boot(sect_size, fat_size, root_entr, res_sect, fat_num, sect_in_clus, boot);
			break;
		}
	        case 2:
		{
			print_tree(0, fp, root_pos, root_entr, fat, data_pos, sect_size, sect_in_clus);
			break;
		}	
		case 3:
		{
			addr = file_addr(0, fp, root_pos, data_pos, root_entr, sect_size, sect_in_clus, fat);
			if(addr == -2)
				return -1;
			print_file(fp, addr, data_pos, sect_size, sect_in_clus, fat);
			break;
		}
		case 4:
		{
			addr = write_file(fp, fat_num, fat_pos, fat_size, data_pos, root_pos, root_entr, sect_size, sect_in_clus, fat);
			if(addr == -2)
				return -1;
			break;
		}
		case 5:
		{
			print_tree(1, fp, root_pos, root_entr, fat, data_pos, sect_size, sect_in_clus);
			break;
		}
		case 6:
		{
			addr = file_addr(1, fp, root_pos, data_pos, root_entr, sect_size, sect_in_clus, fat);
			if(addr == -2)
				return -1;
			print_file(fp, addr, data_pos, sect_size, sect_in_clus, fat);
			break;
		}
		case 7:
		{
			addr = file_addr(1, fp, root_pos, data_pos, root_entr, sect_size, sect_in_clus, fat);
			if(addr == -2)
				return -1;
			file_recovery(fp, addr, fat, fat_pos, fat_num, fat_size, sect_size);
			break;
		}
		default:
		{
			printf("You decided to finish the program or you entered an incorrect value\n");
		}
	}

	fclose(fp);
	free(fat);
	free(root);
        return 0;
}
