#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#define bool int
#define false 0
#define true 1

/* Takes input from stdin up to a value of max with overflow checking.
 * Allows for hex or any recognition.
 */
int intoi(char *buffer, uint8_t max, uint8_t hex, uint32_t *num)
{ 
	uint8_t len;
	fgets(buffer, max + 1, stdin);
	len = strlen(buffer);
	//if(len == max) len++;

	/* If hex value is set check for valid hex digits */
	if(hex != false)
	{
		int i = 0;
		for(i = 0; i < len - 1; i++)
		{
			if(!isxdigit(buffer[i]))
			{
				printf("Enter a valid hex digit\n");
				/* flush stdin here */
				fseek(stdin, 0, SEEK_SET);
				return -1;
			}
		}
	}
	sscanf(buffer, " %X", num);
}