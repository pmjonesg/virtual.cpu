/*
 *	Name:			Peter Jones
 *	Date:			January 30, 2015
 *	Description:	This program declares an array of 16KB which acts as the memory of the computer.
 *					It will include the following flags:
 *
 *					d	dump memory
 *					g	go – run the entire program
 *					l	load a file into memory	(Allows you to specify the file name.)
 *					m	memory modify
 *					q	quit
 *					r	display registers
 *					t	trace – execute one instruction
 *					w	write file
 *					z	reset all registers to zero
 *					?, h	display list of commands
 *
 *					But only flags l, w, h, and q will be implemented this week.
 */

//#define _CRT_SECURE_NO_WARNINGS /* This line is specifically to disable warnings in Visual Studio*/

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "hardware.h"

registers regs;
uint8_t memptr[MEMORY_MAX];

int main(int argc, char **argv) {

	char offset_input[HEX_MAX + 1], length_input[HEX_MAX + 1];
	uint32_t offset, length;
	char command;
	unsigned int i;
	bool valid = false;

	zero(&regs);

	while (true) {
		printf("Enter an option (h to display help): ");
		scanf(" %c", &command);
		getchar();
		if (command < 97)
			command = tolower(command);

		switch (command) {
			case 'd':
				/* Input must be taken before calling the function as it takes specific arguments */
				printf("(offset?) ");
				if(intoi(offset_input, HEX_MAX, 1, &offset) == -1)
					break;
				printf("(length?) ");
				if(intoi(length_input, HEX_MAX, 1, &length) == -1)
					break;
				memdump(memptr, offset, (length - 1));
				break;
			case 'g':
				while(!STOP)
					ic(&regs, memptr);
				break;
			case 'l':
				loadfile(&memptr, MEMORY_MAX);
				break;
			case 'm':
				printf("(offset?) ");
				if(intoi(offset_input, HEX_MAX + 1, 1, &offset) == -1)
					break;
				if(offset > MEMORY_MAX || offset < 0)
				{
					printf("Offset must be a positive integer between 0x0000 and 0x4000\n");
					break;
				}
				memmod(memptr, offset);
				break;
			case 'q':
				return 0;
			case 'r':
				regdump(&regs);
				break;
			case 't':
				trace;
				break;
			case 'w':
				writefile(&memptr);
				break;
			case 'z':
				printf("Reinitializing registers... ");
				zero(&regs);
				printf("Done\n");
				break;
			case '?':
			case 'h':
				printf("\nd	dump memory\n"
					"g	go - run the entire program\n"
					"l	load a file into memory	(Allows you to specify the file name.)\n"
					"m	memory modify\n"
					"q	quit\n"
					"r	display registers\n"
					"t	trace - execute one instruction\n"
					"w	write file\n"
					"z	reset all registers to zero\n"
					"?, h	display list of commands\n\n");
				break;
			default:
				printf("Unrecognized command, please consult commands with h\n");
		}
	}
	getchar();
	getchar();
	return 0;
}