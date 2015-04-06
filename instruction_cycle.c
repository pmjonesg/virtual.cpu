#include <stdio.h>
#include "hardware.h"

void ic(registers *regs, void *memptr)
{
	uint16_t type;
	if(IR_ACTIVE == 0)
	{
		fetch(regs, memptr);
		type = IRsplit(IR_ACTIVE, regs);
		decode(regs->IR0, type, regs, memptr);
		printf("R0 is: %X\n", regs->IR0);
		IR_ACTIVE = 1;
	}
	else
	{
		unsigned int type = IRsplit(IR_ACTIVE, regs);
		decode(regs->IR1, type, regs, memptr);
		printf("R1 is: %X\n", regs->IR1);
		IR_ACTIVE = 0;
	}
}

uint16_t IRsplit(unsigned ir, registers *regs)
{
	unsigned int type;

	regs->IR0 = (regs->IR >> 16);
	regs->IR1 = regs->IR;

	if(ir == 0) 
	{
		return type = (regs->IR0 & 0xE000) >> 13;
	}
	else
		return type = (regs->IR1 & 0xE000) >> 13;
}

void fetch(registers *regs, uint8_t *memptr)
{
	int i;
	regs->MAR = regs->PC;
	for(i = 0; i < 4; i++)
	{
		regs->MBR = regs->MBR << 8;
		regs->MBR += memptr[regs->MAR++];
	}
	regs->IR = regs->MBR;
	regs->PC = regs->PC + sizeof(float);
}

void decode(uint16_t ir, uint16_t type, registers *regs, uint8_t *memptr)
{
	switch(type)
	{
		case 0x0:
			execute_dataproc(ir, type, regs);
			break;
		case 0x1:
			execute_load(ir, type, regs, memptr);
			break;
		case 0x2:
		case 0x3:
			printf("immediate operation\n");
			execute_immediate(ir, type, regs);
			break;
		case 0x4:
			execute_cbranch(ir, type, regs);
			break;
		case 0x5:
			printf("Push/Pull\n");
			break;
		case 0x6:
			execute_ubranch(ir, type, regs);
			break;
		case 0x7:
			STOP = 1;
			break;
		default:
			perror("Type not Found\n");
			break;
	}
}