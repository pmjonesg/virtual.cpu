#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "hardware.h"

/* The purpose of this function is to load a stored file into memory,
   and display the number of bytes read in decimal and hex. */
int loadfile(void *memory, uint32_t max) {
	char file_name[FILEN_MAX], *pos;
	int bytes_read;
	FILE *file;

	printf("Enter the name of the file to load: ");
	fgets(file_name, FILEN_MAX, stdin);
	if ((pos = strchr(file_name, '\n')) != NULL)
		*pos = '\0';	

	file = fopen(file_name, "rb");
	if (file != NULL) {
		/* Determine number of bytes in file */
		fseek(file, 0, SEEK_END);
		bytes_read = ftell(file);
		rewind(file);
		
		/* Read file with max as the limit (normally it would be MEMORY_MAX) */
		if(bytes_read == max)
		{
			fread(memory, 1, max, file);
			printf("%d (Decimal) %02x (Hex) bytes read from file %s, and has been truncated\n", bytes_read, bytes_read, file_name);
		}
		else
		{
			fread(memory, 1, bytes_read, file);
			printf("%d (Decimal) %x (Hex) bytes read from file %s\n", bytes_read, bytes_read, file_name);
		}
		fclose(file);
	}
	else
		perror("The following error has occurred");
}