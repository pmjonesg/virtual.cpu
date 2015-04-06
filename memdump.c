#include <stdio.h>
#include <stdint.h>
#include <ctype.h>
#include "hardware.h"

#define HEXDUMP_COLS 16

/* Displays the contents of memory */
void memdump(void *memptr, unsigned offset, unsigned length) 
{
	uint32_t i, j, k, end;

	end = offset + length;

	for(i = offset; i < end; i += HEXDUMP_COLS)
	{
		if(i >= MEMORY_MAX)
		{
			i = 0x0000;
			end -= MEMORY_MAX;
		}
		if(i != offset) putchar('\n');
		printf("%04X\t", i);

		for(j = i; j < (i + HEXDUMP_COLS); j++)
		{
			printf("%02X ", ((unsigned char*)memptr)[j]);

			/* If the length is reached before the end of the line exit loop */
			if(j == (length + offset))
			{
				printf("\n\t");
				for(k = i; k <= (length + offset); k++)
					if(isprint(((char*)memptr)[k]))
						printf(" %c ", ((char*)memptr)[k]);
					else
						printf(" . ");
				break;
			}
			/* Printf the ASCII character of hex values if possible, else print '.' */ 
			if(j == (i + HEXDUMP_COLS) - 1)
			{
				printf("\n\t");
				for(k = i; k < (i + HEXDUMP_COLS); k++)
				{
					if(isprint(((char*)memptr)[k]))
						printf(" %c ", ((char*)memptr)[k]);
					else
						printf(" . ");
				}
			}
		}
	}
	putchar('\n');
}