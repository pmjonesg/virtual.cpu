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

/* Memory */
uint8_t memptr[MEMORY_MAX];

/* registers holds all hardware except for memory */
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
uint8_t		SIGN,
			CARRY,
			ZERO;
} registers;

/* Initialize registers struct as global */
registers regs;

/* Flags */
bool	STOP,
		IR_ACTIVE;

#define FILEN_MAX 128
#define HEX_MAX 0x4
#define REG_NUM 16
#define MAX32 0xFFFFFFFF
#define HEXDUMP_COLS 16

int intoi(char *buffer, uint8_t max, uint8_t hex, uint32_t *num);
void writefile();
int loadfile(unsigned int max);
void memdump(unsigned offset, unsigned length);
void memmod(unsigned offset);
void fetch();
void ic();
uint16_t IRsplit(unsigned ir);
void decode(uint16_t ir, uint16_t type);
void execute_dataproc(unsigned ir, unsigned type);
void execute_immediate(unsigned ir, unsigned type);
void execute_load(unsigned ir, unsigned type);
void execute_cbranch(unsigned ir, unsigned type);
void execute_push(uint16_t ir, uint16_t type);
void execute_ubranch(unsigned ir, unsigned type);
void regdump();
void zero();
void allflagchk(uint32_t regs1, uint32_t regs2);
void nzflagchk(uint32_t regs1, uint32_t regs2);

#endif