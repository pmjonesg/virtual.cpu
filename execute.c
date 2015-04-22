#include <stdio.h>
#include <stdint.h>
#include "hardware.h"

/* Masks definitions */
/* Data Processing */
#define DATA_OP ((ir & 0xF00) >> 8)
#define DATA_RN ((ir & 0xF0) >> 4)
#define DATA_RD (ir & 0x000F)
/* Load/Store */
#define LOAD_B ((ir & 0x800) >> 11)
#define LOAD_W ((ir & 0x400) >> 10)
#define LOAD_RN ((ir & 0x0F0) >> 4)
#define LOAD_RD (ir & 0x000F)
/* Immediate Operations */
#define IMM_OP ((ir & 0x3000) >> 12)
#define IMM_VALUE ((ir & 0x0FF0) >> 4)
#define IMM_RD (ir & 0x000F)
/* Conditional Branch */
#define CBRANCH_COND ((ir & 0xF00) >> 8)
#define CBRANCH_ADDR (ir & 0x00FF)
/* Push/Pull */
#define PUSH_LOAD_B ((ir & 0x0800) >> 11)
#define PUSH_EXTRA_B ((ir & 0x0400) >> 10)
#define PUSH_HIGH_B ((ir & 0x0100) >> 8)
#define PUSH_REG_LIST (ir & 0x00FF)
/* Unconditional Branch */
#define UBRANCH_LINK ((ir & 0x1000) >> 12)
#define UBRANCH_OFFSET (ir & 0x0FFF)

/* Mask Definitions */
/* Data Processing */
#define DATA_ADC 0x5
#define DATA_ADD 0x4
#define DATA_AND 0x0
#define DATA_BIC 0xE
#define DATA_CMP 0xA
#define DATA_EOR 0x1
#define DATA_LSL 0x7
#define DATA_LSR 0x6
#define DATA_MOV 0xD
#define DATA_MVN 0xF
#define DATA_ORR 0xC
#define DATA_ROR 0xB
#define DATA_SUB 0x2
#define DATA_SXB 0x3
#define DATA_TEQ 0x9
#define DATA_TST 0x8
#define FIRST_BIT 0x0001
/* Immediate Operations */
#define MOV_IMM 0x0
#define CMP_IMM 0x1
#define ADD_IMM 0x2
#define SUB_IMM 0x3
/* Conditional Branch Codes */
#define CBRANCH_EQ 0x0000
#define CBRANCH_NE 0x0001
#define CBRANCH_CS 0x0002
#define CBRANCH_CC 0x0003
#define CBRANCH_MI 0x0004
#define CBRANCH_PL 0x0005
#define CBRANCH_HI 0x0008
#define CBRANCH_LS 0x0009
#define CBRANCH_AL 0x000E

void execute_dataproc(unsigned ir, unsigned type, registers *regs)
{
	int i = 0;
	switch(DATA_OP)
	{
		case DATA_ADC:
			regs->ALU = regs->R[DATA_RD] + DATA_RN + regs->CARRY;
			regs->R[DATA_RD] = regs->ALU;
			allflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_ADD:
			regs->ALU = regs->R[DATA_RD] + regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			allflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_AND:
			regs->ALU = regs->R[DATA_RD] & regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_BIC:
			regs->ALU = regs->R[DATA_RD] & ~regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_CMP:
			regs->ALU = regs->R[DATA_RD] & ~regs->R[DATA_RN] + 1;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_EOR:
			regs->ALU = regs->R[DATA_RD] ^ regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_LSL:
			regs->ALU = regs->R[DATA_RD] << regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			allflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_LSR:
			regs->ALU = regs->R[DATA_RD] >> regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			allflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_MOV:
			regs->R[DATA_RD] = regs->R[DATA_RN];
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_MVN:
			regs->R[DATA_RD] = ~regs->R[DATA_RN];
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_ORR:
			regs->ALU = regs->R[DATA_RD] | regs->R[DATA_RN];
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_ROR:
			for(i = 0; i < regs->R[DATA_RN]; i++)
			{
				regs->CARRY = regs->R[DATA_RD] & 0x0001;
				if(regs->CARRY)
					regs->ALU = (regs->R[DATA_RD] >> 1) | 0x80000000;
			}
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_SUB:
			regs->ALU = regs->R[DATA_RD] + ~regs->R[DATA_RN] + 1;
			regs->R[DATA_RD] = regs->ALU;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_SXB:
			regs->R[DATA_RD] = (signed)regs->R[DATA_RN];
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_TEQ:
			regs->ALU = regs->R[DATA_RD] ^ regs->R[DATA_RN] + 1;
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		case DATA_TST:
			regs->ALU = regs->R[DATA_RD] & regs->R[DATA_RN];
			nzflagchk(regs, regs->R[DATA_RD], regs->R[DATA_RN]);
			break;
		default:
			perror("Operation not found");	
	}
}

void execute_load(unsigned ir, unsigned type, registers *regs, uint8_t *memptr)
{
	int i = 0;
	uint32_t mask = 0xFF000000;
	switch(LOAD_B)
	{
		// Store
		case 0:
			// Word
			if(LOAD_W == 0)	
			{
				regs->MAR = regs->R[LOAD_RN];
				regs->MBR = regs->R[LOAD_RD];
				for(i = 0; i < 4; i++)
				{
					/* Move MSB first into memory, then the rest */
					memptr[regs->MAR++] = (regs->MBR & mask) >> (8 * (3 - i));
					mask = mask >> 8;
				}
			}
			// Byte
			else if(LOAD_W == 1)
			{
				regs->MAR = regs->R[LOAD_RN];
				regs->MBR = regs->R[LOAD_RD];
				memptr[regs->MAR] = regs->MBR;
			}
			break;
		// Load
		case 1:
			// Load word
			if(LOAD_W == 0)
			{
				regs->MAR = regs->R[LOAD_RN];
				for(i = 0; i < 4; i++)
				{
					regs->MBR = regs->MBR << 8;
					regs->MBR += memptr[regs->MAR++];
				}
				regs->R[LOAD_RD] = regs->MBR;
			}
			// Load byte
			else if(LOAD_W == 1)
			{
				regs->MAR = regs->R[LOAD_RN] & 0x000000FF;
				regs->MBR = memptr[regs->MAR];
				regs->R[LOAD_RD] = regs->MBR;
			}
			break;
	}	
}

void execute_immediate(unsigned ir, unsigned type, registers *regs)
{
	switch(IMM_OP)
	{
		case MOV_IMM:
			regs->ALU = IMM_VALUE;
			regs->R[IMM_RD] = regs->ALU;
			nzflagchk(regs, regs->R[IMM_RD], IMM_VALUE);
			break;
		case CMP_IMM:
			regs->ALU = regs->R[IMM_RD] - IMM_VALUE;
			allflagchk(regs, regs->ALU, IMM_VALUE);
			break;
		case ADD_IMM:
			regs->ALU = regs->R[IMM_RD] + IMM_VALUE;
			regs->R[IMM_RD] = regs->ALU;
			allflagchk(regs, regs->R[IMM_RD], IMM_VALUE);
			break;
		case SUB_IMM:
			regs->ALU = regs->R[IMM_RD] + ~IMM_VALUE + 1;
			regs->R[IMM_RD] = regs->ALU;
			allflagchk(regs, regs->R[IMM_RD], IMM_VALUE);
			break;
	}
}

void execute_cbranch(unsigned ir, unsigned type, registers *regs)
{
	/* Use hold value to sign CBRANCH_ADDR */
	int8_t temp = CBRANCH_ADDR;
	int32_t offset = (int32_t) temp;
	/* Decrement PC to reset the branch fetch */
	regs->PC -= 2;

	switch(CBRANCH_COND)
	{
		case CBRANCH_EQ:
			if(regs->ZERO == true)
				regs->PC += offset;
			break;
		case CBRANCH_NE:
			if(regs->ZERO == false)
				regs->PC += offset;	
			break;
		case CBRANCH_CS:
			if(regs->CARRY = true)
				regs->PC += offset;
			break;
		case CBRANCH_CC:
			if(regs->CARRY = false)
				regs->PC += offset;
			break;
		case CBRANCH_MI:
			if(regs->SIGN == true)
				regs->PC += offset;
			break;
		case CBRANCH_PL:
			if(regs->SIGN == false)
				regs->PC += offset;
			break;
		case CBRANCH_HI:
			if(regs->CARRY == true && regs->ZERO == false)
				regs->PC += offset;
			break;
		case CBRANCH_LS:
			if(regs->CARRY == false && regs->ZERO == true)
				regs->PC += offset;
			break;
		case CBRANCH_AL:
			regs->PC += offset;
			break;
	}
	regs->IR = 0;
	IR_ACTIVE = 0;
}

void execute_push(uint16_t ir, uint16_t type, registers *regs, uint8_t *memptr)
{
	uint8_t i;

	regs->SP--;
	printf("sp: %08X\n", regs->SP);
	regs->MAR = regs->SP;
	for(i = 3; i >= 0; i--)
	{
		memptr[regs->MAR + i] = regs->R[PUSH_REG_LIST];
		regs->MBR = regs->MBR >> 8;
	}
}

void execute_ubranch(unsigned ir, unsigned type, registers *regs)
{
	if(UBRANCH_LINK == 0)
	{
		regs->PC = UBRANCH_OFFSET;
	}
	else
	{
		regs->LR = regs->PC;
		regs->PC = UBRANCH_OFFSET;
	}
}