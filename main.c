/*
*	Name:			Peter Jones
*	Date:			January 30, 2015
*	Description:	This program declares an array of 16KB which acts as the memory of the computer.
*			It will include the following flags:
*
*			d	dump memory
*			g	go – run the entire program
*			l	load a file into memory	(Allows you to specify the file name.)
*			m	memory modify
*			q	quit
*			r	display registers
*			t	trace – execute one instruction
*			w	write file
*			z	reset all registers to zero
*			?, h	display list of commands
*
*			But only flags l, w, h, and q will be implemented this week.
*/

#define _CRT_SECURE_NO_WARNINGS /* This line is specifically to disable warnings in Visual Studio*/
#define DMEMORY_MAX 16384
#define XMEMORY_MAX 0x4000 
#define FILEN_MAX 128
#define HEX_MAX 0x4
#define REG_NUM 16
#define MAX32 0xFFFFFFFF

#ifndef HEXDUMP_COLS
#define HEXDUMP_COLS 16
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

void WriteFile(void *memory);
int LoadFile(void *memory, unsigned int max);
void MemDump(void *memptr, unsigned offset, unsigned length);
void MemMod(void *memptr, unsigned offset);
void Reset();
void fetch();
void execute_dataproc(unsigned ir, unsigned type);
void execute_immediate(unsigned ir, unsigned type);
void execute_load(unsigned ir, unsigned type);
void Trace();
void RegDump();

#define SP R[13]
#define LR R[14]
#define PC R[15]

/* Main memory - 16KB */
unsigned char memptr[DMEMORY_MAX];

/* Registers */
unsigned long	IR,
		R[REG_NUM],	
		MAR,
		MBR;

unsigned short	IR0,
		IR1;

/* Flags */
bool		STOP,
		IR_ACTIVE,
		SIGN,
		CARRY,
		ZERO;

int main(int argc, char **argv) {
	
	char offset_input[HEX_MAX], length_input[HEX_MAX];
	unsigned int offset, length;
	char command;
	unsigned int c, i;
	bool valid = false;

	Reset();

	while (1) {
		printf("Enter an option (h to display help): ");
		scanf("\n %c", &command);
		getchar();
		if (command < 97)
			command = tolower(command);

		switch (command) {
			case 'd':
				/* Check for valid input for offset */
				printf("Enter the offset: ");
				fgets(offset_input, HEX_MAX + 1, stdin);
				for(i = 0; i < HEX_MAX; i++)
				{
					if(!isxdigit(offset_input[i]))
						break;
					else
						valid = true;	
				}
				if(!valid)
				{
					printf("Enter a valid hex digit\n");
					break;					
				}
				sscanf(offset_input, "%X", &offset);
				/* Flush input */
				//while(getchar() != '\n');

				if(offset > XMEMORY_MAX)
				{
					printf("Value entered is not valid.\n");
					break;
				}
				else if(offset < 0)
				{
					printf("Offset must be a positive integer between 0x0000 and 0x4000\n");
					break;
				}
				printf("Offset is: %#X\n", offset);
				

				printf("Enter the length of memory dump: ");
				fgets(length_input, HEX_MAX + 1, stdin);
				valid = false;
				for(i = 0; i < HEX_MAX; i++)
				{
					if(!isxdigit(length_input[i]))
					{
						break;
					}
					else
						valid = true;	
				}
				if(!valid)
				{
					printf("Enter a valid hex digit\n");
					break;					
				}
				sscanf(length_input, "%X", &length);
				/* Flush input */
				//while(getchar() != '\n');

				if(length > XMEMORY_MAX)
				{
					printf("Value entered is not valid.\n");
					break;
				}
				else if((length + offset) > XMEMORY_MAX)
				{
					printf("Length exceeds memory limit\n");
					break;
				}
				printf("length is: %X\n", length);

				MemDump(memptr, offset, (length - 1));
				break;
			case 'g':
				printf("This option runs the entire program.\n");
				break;
			case 'l':
				LoadFile(&memptr, DMEMORY_MAX);
				break;
			case 'm':
				printf("Enter the starting address to modify: ");
				fgets(offset_input, HEX_MAX + 1, stdin);
				for(i = 0; i < HEX_MAX; i++)
				{
					if(!isxdigit(offset_input[i]))
						break;
					else
						valid = true;	
				}
				if(!valid)
				{
					printf("Enter a valid hex digit\n");
					break;					
				}
				sscanf(offset_input, "%X", &offset);
				if(offset > XMEMORY_MAX)
				{
					printf("Value entered is not valid.\n");
					break;
				}
				else if(offset < 0)
				{
					printf("Offset must be a positive integer between 0x0000 and 0x4000\n");
					break;
				}
				printf("Offset is: %X\n", offset);

				MemMod(memptr, offset);
				break;
			case 'q':
				exit(0);
			case 'r':
				RegDump();
				break;
			case 't':
				Trace();
				break;
			case 'w':
				WriteFile(&memptr);
				break;
			case 'z':
				Reset();
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

/* The purpose of this function is to load a stored file into memory,
   and display the number of bytes read in decimal and hex. */
int LoadFile(void *memory, unsigned int max) {
	char file_name[FILEN_MAX], *pos;
	int bytes_read, c;
	FILE *file;

	printf("Enter the name of the file to load: ");
	fgets(file_name, FILEN_MAX, stdin);
	if ((pos = strchr(file_name, '\n')) != NULL)
		*pos = '\0';	
	//while ((c = getchar()) != '\n' && c != EOF);
	file = fopen(file_name, "rb");
	if (file != NULL) {
		fread(memory, 1, DMEMORY_MAX, file);
		bytes_read = ftell(file);
		if(bytes_read = DMEMORY_MAX)
			printf("%d (Decimal) %02x (Hex) bytes read from file, and has been truncated\n", bytes_read, bytes_read);
		else
			printf("%d (Decimal) %x (Hex) bytes read from file\n", bytes_read, bytes_read);
		fclose(file);
	}
	else
		perror("The following error has occurred");

}

/* The purpose of this function is to write the contents of memory from *memory to
   file_size into a file. */
void WriteFile(void *memory)
{
	char file_name[FILEN_MAX], *pos;
	int file_size, c;
	FILE *file;

	printf("Enter the name of the file to write: ");
	fgets(file_name, FILEN_MAX, stdin);
	if ((pos = strchr(file_name, '\n')) != NULL)
		*pos = '\0';
	while ((c = getchar()) != '\n' && c != EOF);

	printf("Enter the number of bytes to write: ");
	scanf(" %d", &file_size);
	if (file_size > DMEMORY_MAX)
		printf("Only 16384 bytes were written to file due to memory size.\n");
	file = fopen(file_name, "wb");
	fwrite(&memory, 1, file_size, file);
	fclose(file);
}

void MemDump(void *memptr, unsigned offset, unsigned length) 
{
	unsigned int i, j, k;
	
	for(i = offset; i < (offset + length); i += HEXDUMP_COLS)
	{
		putchar('\n');
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
	putchar('\n');
}

void MemMod(void *memptr, unsigned offset)
{
	char new_input[2];
	unsigned int value;

	while(1) 
	{
		printf("Enter a new value value for address 0x%04X (in hex), '.' to exit: ", offset);
		fgets(new_input, 3, stdin);
		
		/* Exit function if '.' is entered */
		if(strcmp(new_input, ".\n") == 0)
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

void Reset()
{
	R[0] = 0x00000000;
	R[1] = 0x00000000;
	R[2] = 0x00000000;
	R[3] = 0x00000000;
	R[4] = 0x00000000;
	R[5] = 0x00000000;
	R[6] = 0x00000000;
	R[7] = 0x00000000;
	R[8] = 0x00000000;
	R[9] = 0x00000000;
	R[10] = 0x00000000;
	R[11] = 0x00000000;
	R[12] = 0x00000000;
	SP = 0x00000000;
	LR = 0x00000000;
	PC = 0x00000000;
	MBR = 0x0000000;
	MAR = 0x00000000;
	IR0 = 0x0000;
	IR1 = 0x0000;
	IR = 0x00000000;
	IR_ACTIVE = 0;
	STOP = 0;
	SIGN = 0;
	ZERO = 0;
	CARRY = 0;
}

void fetch()
{
	int i;

	MAR = PC;
	for(i = 0; i < 4; i++)
	{
		MBR = MBR << 8;
		MBR += memptr[MAR++];
	}
	IR = MBR;
	PC = PC + sizeof(float);
}

void decode(unsigned ir, unsigned type)
{
	switch(type)
	{
		case 0x0:
			execute_dataproc(ir, type);
			break;
		case 0x1:
			execute_load(ir, type);
			break;
		case 0x2:
		case 0x3:
			printf("immediate operation\n");
			execute_immediate(ir, type);
			break;
		case 0x4:
			printf("Conditional Branch\n");
			break;
		case 0x5:
			printf("Push/Pull\n");
			break;
		case 0x6:
			printf("Unconditional Branch\n");
			break;
		case 0x7:
			STOP = 1;
			break;
		default:
			perror("Type not Found\n");
			break;
	}
}

unsigned IRsplit(unsigned ir)
{
	IR0 = (IR >> 16);
	IR1 = IR;

	unsigned int type;

	if(ir == 0) 
	{
		return type = (IR0 & 0xE000) >> 13;
	}
	else
		return type = (IR1 & 0xE000) >> 13;
}

void execute_dataproc(unsigned ir, unsigned type)
{
	unsigned int operation = (ir & 0xF00) >> 11, source = (ir & 0xF0) >> 4, destination = (ir & 0x000F);
	switch(operation)
	{
		// ADC
		case 0x5:
			iscarry(source, destination, CARRY);
			break;
		default:
			perror("Operation not found");	
	}
}

/**********************************************************
*   iscarry()- determine if carry is generated by addition: op1+op2+C
*     C can only have value of 1 or 0.
************************************************************/
int iscarry(unsigned long op1,unsigned long op2, unsigned C){
if ((op2 == MAX32)&&(C == 1)) 
	return(1); // special case where op2 is at MAX32
return((op1 > (MAX32 - op2 - C))?1:0);
}

void execute_load(unsigned ir, unsigned type)
{
	unsigned int load_bit = (ir & 0x800) >> 11, word_bit = (ir & 0x400) >> 10, address = (ir & 0x0F0) >> 4, destination = (ir & 0x000F);
	int i = 0;
	
	switch(load_bit)
	{
		// Store
		case 0:
			// Word
			if(word_bit == 0)	
			{
				MAR = R[address];
				MBR = R[destination];
				for(i = 1; i >= 0; i--)
				{
					memptr[MAR + i] = MBR;
					MBR = MBR >> 8;
				}
			}
			// Byte
			else if(word_bit == 1)
			{
				MAR = R[address];
				MBR = R[destination];
				memptr[MAR] = MBR;
			}
			break;
		// Load
		case 1:
			// Load word
			if(word_bit == 0)
			{
				MAR = R[address];
				for(i = 1; i >= 0; i--)
				{
					MBR += memptr[MAR + i];
					MBR = MBR >> 8;
				}
				R[destination] = MBR;
			}
			// Load byte
			else if(word_bit == 1)
			{
				MAR = R[address] & 0x000000FF;
				MBR = memptr[MAR];
				R[destination] = MBR;
			}
			break;
	}	
}

void execute_immediate(unsigned ir, unsigned type)
{
	unsigned int opcode = (ir & 0x3000) >> 12, value = (ir & 0x0FF0) >> 4, destination = (ir & 0x000F);
	switch(opcode)
	{
		/* MOV */
		case 0x0:
			R[destination] = value;
			break;
		/* CMP */
		case 0x1:
			R[destination] -= value;
			break;
		/* ADD */
		case 0x2:
			R[destination] += value;	
			break;
		/* SUB */
		case 0x3:
			R[destination] -= value;	
			break;
		
	}
}

void ic()
{
	if(IR_ACTIVE == 0)
	{
		fetch();
		unsigned int type = IRsplit(IR_ACTIVE);
		decode(IR0, type);
		printf("R0 is: %X\n", IR0);
		IR_ACTIVE = 1;
	}
	else
	{
		unsigned int type = IRsplit(IR_ACTIVE);
		decode(IR1, type);
		printf("R1 is: %X\n", IR1);
		IR_ACTIVE = 0;
	}
}

void Trace()
{
	ic();
	RegDump();
}

void RegDump()
{
	printf("R0\t R1\t  R2\t   R3\t    R4\t     R5\t      R6       R7\tR8\n");
	printf("%08lX %08lX %08lX %08lX %08lX %08lX %08lX %08lX %08lX\n", R[0], R[1], R[2], R[3], R[4], R[5], R[6], R[7], R[8]);
	printf("R9\t R10\t  R11\t   R12\t    R13(SP)  R14(LR)  R15(PC)  MBR\tMAR\n");  
	printf("%08lX %08lX %08lX %08lX %08lX %08lX %08lX %08X %08lX\n", R[9], R[10], R[11], R[12], SP, LR, PC, MBR, MAR);
	printf("IR\t IR Flag  Stop\t   CCR(Sign, Zero, Carry)\n");
	printf("%08X %1i\t  %1i\t   %1i%1i%1i\n", IR, IR_ACTIVE, STOP, SIGN, ZERO, CARRY);
}
