#include <stdio.h>
#include "hardware.h"

/* Implementation of the Instruction Cycle,
 * First it fetches 2 instructions if the IR_ACTIVE flag is clear,
 * then the type of the instruction is determined (Data Processing, Load/Store, Immediate, Conditional Branch, Push/Pull, Unconditional Branch, Stop)
 * following by the deconding of the function and finally set the IR_ACTIVE flag accordingly.
 */
void ic(registers *regs, uint8_t *memptr)
{
	uint16_t type;
	if(IR_ACTIVE == 0)
	{
		fetch(regs, memptr);
		type = IRsplit(IR_ACTIVE, regs);
		decode(regs->IR0, type, regs, memptr);
		IR_ACTIVE = 1;
	}
	else
	{
		unsigned int type = IRsplit(IR_ACTIVE, regs);
		decode(regs->IR1, type, regs, memptr);
		IR_ACTIVE = 0;
	}
}

/* Splits the IR register into R0 and R1 registers, for easier instruction management */
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

/* Sets MAR = PC, fetches 4 bytes from memory and stores them in MBR, copies the contents of MBR to IR, and finally increments the PC */
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

/* Determines what type of instruction and calls appropriate execute function */
void decode(uint16_t ir, uint16_t type, registers *regs, uint8_t *memptr)
{
	switch(type)
	{
		/* Data Processing */
		case 0x0:
			execute_dataproc(ir, type, regs);
			break;
		/* Load/Store */
		case 0x1:
			execute_load(ir, type, regs, memptr);
			break;
		/* Immediate Operations */
		case 0x2:
		case 0x3:
			execute_immediate(ir, type, regs);
			break;
		/* Conditional Branch */
		case 0x4:
			execute_cbranch(ir, type, regs);
			break;
		/* Push/Pull */
		case 0x5:
			execute_push(ir, type, regs);
			break;
		/* Unconditional Branch */
		case 0x6:
			execute_ubranch(ir, type, regs);
			break;
		/* Stop */
		case 0x7:
			STOP = 1;
			break;
		default:
			perror("Type not Found\n");
			break;
	}
}