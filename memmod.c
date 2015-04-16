#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hardware.h"

void memmod(void *memptr, unsigned offset)
{
	char new_input[sizeof(uint16_t) + 1];
	unsigned int value;

	while(1) 
	{
		printf("Enter a new value value for address 0x%04X (in hex), '.' to exit: ", offset);
		fgets(new_input, sizeof(uint16_t) + 1, stdin);
		
		/* Exit function if '.' is entered */
		if(strcmp(new_input, ".\n") == 0 || strcmp(new_input, ".") == 0)
			break;
		/* Check if input is a valid Hex number */
		else if(!isxdigit(new_input[0]) || !isxdigit(new_input[1])) 
		{
			printf("Enter a valid 2 digit hex number.\n");
			continue;
		}
		/* If valid hex number modify memory and increment memory pointer */
		else
		{
			sscanf(new_input, "%X", &value);
			getchar();
			printf("New value is: %02X\n", value);
			((char*)memptr)[offset] = value;
			offset++;
		}
	}
}