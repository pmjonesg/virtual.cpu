#include <stdio.h>
#include "hardware.h"

void regdump()
{
	printf("R0\t R1\t  R2\t   R3\t    R4\t     R5\t      R6       R7\tR8\n");
	printf("%08X %08X %08X %08X %08X %08X %08X %08X %08X", regs.R[0], regs.R[1], regs.R[2], regs.R[3], regs.R[4], regs.R[5], regs.R[6], regs.R[7], regs.R[8]);
	printf("R9\t R10\t  R11\t   R12\t    R13(SP)  R14(LR)  R15(PC)  MBR\tMAR\n");  
	printf("%08X %08X %08X %08X %08X %08X %08X %08X %08X", regs.R[9], regs.R[10], regs.R[11], regs.R[12], regs.SP, regs.LR, regs.PC, regs.MBR, regs.MAR);
	printf("IR\t IR Flag  Stop\t   CCR(Sign, Zero, Carry)\n");
	printf("%08X %1i\t  %1i\t   %1i%1i%1i\n", regs.IR, IR_ACTIVE, STOP, regs.SIGN, regs.ZERO, regs.CARRY);
}
