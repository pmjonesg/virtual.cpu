#ifndef HARDWARE
#define HARDWARE

#include <stdint.h>

/* Because stdbool.h is not part of Visual Studio bool data type has been defined this way */
#define bool int
#define false 0
#define true 1

/* Main memory - 16KB */
#define MEMORY_MAX 0x4000

/* Registers - 0 to 12 and special registers SP, LR, and PC - 32-bit */
#define REG_NUM 16
#define SP R[13]
#define LR R[14]
#define PC R[15]

typedef struct
{
uint32_t	IR,
			ALU,
			R[REG_NUM],	
			MAR,
			MBR;

/* Instruction registers - 16-bit */
uint16_t	IR0,
			IR1;

/* Condition Code Registers */
bool		SIGN,
			CARRY,
			ZERO;
} registers;

/* Flags */
bool	STOP,
		IR_ACTIVE;

#define FILEN_MAX 128
#define HEX_MAX 0x4
#define REG_NUM 16
#define MAX32 0xFFFFFFFF
#define HEXDUMP_COLS 16
#define trace ic(&regs, memptr); regdump(&regs)

int intoi(char *buffer, uint8_t max, uint8_t hex, uint32_t *num);
void writefile(void *memory);
int loadfile(void *memory, unsigned int max);
void memdump(void *memptr, unsigned offset, unsigned length);
void memmod(void *memptr, unsigned offset);
void fetch(registers *regs, uint8_t *memptr);
void ic(registers *regs, uint8_t *memptr);
uint16_t IRsplit(unsigned ir, registers *regs);
void decode(uint16_t ir, uint16_t type, registers *regs, void *memptr);
void execute_dataproc(unsigned ir, unsigned type, registers *regs);
void execute_immediate(unsigned ir, unsigned type, registers *regs);
void execute_load(unsigned ir, unsigned type, registers *regs, uint8_t *memptr);
void execute_cbranch(unsigned ir, unsigned type, registers *regs);
void execute_push(uint16_t ir, uint16_t type, registers *regs);
void execute_ubranch(unsigned ir, unsigned type, registers *regs);
void regdump(registers *regs);
void zero(registers *regs);
void allflagchk(registers *regs, uint32_t regs1, uint32_t regs2);
void nzflagchk(registers *regs, uint32_t regs1, uint32_t regs2);

#endif