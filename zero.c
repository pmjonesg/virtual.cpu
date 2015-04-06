#include <stdio.h>
#include "hardware.h"

void zero(registers *regs)
{
	int i;
	for(i = 0; i < REG_NUM; i++)
		regs->R[i] = 0x00000000; 
	regs->ALU = 0x00000000;
	regs->MBR = 0x00000000;
	regs->MAR = 0x00000000;
	regs->IR0 = 0x0000;
	regs->IR1 = 0x0000;
	regs->IR = 0x00000000;
	regs->SIGN = 0;
	regs->ZERO = 0;
	regs->CARRY = 0;
	IR_ACTIVE = 0;
	STOP = 0;
}
