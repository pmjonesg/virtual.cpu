#include <stdio.h>
#include <string.h>
#include "hardware.h"

/* The purpose of this function is to write the contents of memory from *memory to
   file_size into a file. */
void writefile(void *memory)
{
	char fname[FILEN_MAX], fsize_buff[HEX_MAX + 1], *pos;
	uint32_t fsize;
	FILE *file;

	printf("Enter the name of the file to write: ");
	fgets(fname, FILEN_MAX, stdin);
	if ((pos = strchr(fname, '\n')) != NULL)
		*pos = '\0';

	printf("Enter the number of bytes to write: ");
	intoi(fsize_buff, HEX_MAX, 1, &fsize);
	if (fsize > MEMORY_MAX)
		printf("Only 16384 bytes were written to file due to memory size.\n");
	file = fopen(fname, "wb");
	fwrite(memory, 1, fsize, file);
	printf("%X bytes have been written to file %s", fsize, fname);
	fclose(file);
}