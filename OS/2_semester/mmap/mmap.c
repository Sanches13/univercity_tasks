#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <time.h>

typedef struct boot_sector{
	//size of sector, number of sectors in a one cluster, number of reserved sectors
	int sect_size, sect_in_clus, res_sect;

	//number of FAT tables, FAT size
	int fat_num, fat_size;

	//number of root entries, root directory offset, offset of data, offset table FAT
	int root_entr, root_pos, data_pos, fat_pos;
}boot;

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

int main(int argc, char *argv[])
{
	boot boot_record;
	int fd;
	long long int length;
	char *ptr;

	if(argc != 2)
	{
		printf("Please, use format <%s> <.img>\n", argv[0]);
		return -1;
	}

	if(0 > (fd = open(argv[1], O_RDWR)))
	{
		perror("open");
		return -1;
	}

	if( -1 == (length = lseek(fd, 0, SEEK_END)))
	{
		perror("lseek");
		close(fd);
		return -1;
	}

	if(MAP_FAILED == (ptr = mmap(NULL, length, PROT_READ, MAP_SHARED, fd, 0)))
	{
		perror("mmap");
		close(fd);
		return -1;
	}

	if((ptr[54] != 0x46 || ptr[55] != 0x41 || ptr[56] != 0x54) ||
	   (ptr[37] != 0x00) || (ptr[510] != 0x55 && ptr[511] != 0xaa))
        {
                printf("The file system of this image is not FAT, or this file in not an image\n");
                return -1;
        }
        
	int i;

	//read information about file system from the boot sector
	boot_record.sect_size = word(ptr[11], ptr[12]);
	boot_record.res_sect = word(ptr[14], ptr[15]);
	boot_record.root_entr = word(ptr[17], ptr[18]); 
	boot_record.fat_size = word(ptr[22], ptr[23]);
	boot_record.sect_in_clus = ptr[13]; 
	boot_record.fat_num = ptr[16];

	boot_record.fat_pos = (boot_record.sect_size)*(boot_record.res_sect);

	(boot_record.root_pos) = (boot_record.sect_size)*(boot_record.res_sect) + (boot_record.fat_num)*(boot_record.fat_size)*(boot_record.sect_size);

	(boot_record.data_pos) = (boot_record.root_pos)+(boot_record.root_entr*32);

	int j;

	//print some information about file system	
	
	printf("Size of sector: %d bytes\n"
	       "Reserved sectors: %d sectors\n"
	       "Number of root entries: %d entries\n"
	       "FAT size: %d sectors\n"
	       "Number of sectors in a one cluster: %d\n"
	       "Number of FAT tables: %d\n"
	       "Offset table FAT: 0x%x\n"
	       "Root directory offset: 0x%x\n"
	       "Offset of data: 0x%x\n"
	       "File system type: ",
		boot_record.sect_size, boot_record.res_sect, boot_record.root_entr, boot_record.fat_size, boot_record.sect_in_clus, boot_record.fat_num, boot_record.fat_pos, boot_record.root_pos, boot_record.data_pos);
	
	for(i = 54; i < 60; i++)
		printf("%c", ptr[i]);
	printf("\n");

	printf("Directories of this file system:\n");

	//print directories of file system

	for(i = boot_record.root_pos; i < boot_record.data_pos - 1024; i += 32)
	{
		if(ptr[i + 11] == 0x10)
		{
			printf("   ");
			for(j = i; j < i + 11; j++)
				printf("%c", ptr[j]);
			printf("\n");
		}
	}

	close(fd);
	return 0;
}
