#ifndef _GLOBALS_H
#define _GLOBALS_H

/* colors */


/* for main() usage */
#define EXIT_FAILURE -1
#define EXIT_SUCCESS 0

/* bool was introduced in c99 */
typedef int bool;
#define false 0
#define true 1

/* data ranges */
#define MIN_SIGNED_8_BIT_RANGE -128
#define MAX_SIGNED_8_BIT_RANGE 127
#define MIN_SIGNED_16_BIT_RANGE -32768
#define MAX_SIGNED_16_BIT_RANGE 32767
#define MIN_SIGNED_32_BIT_RANGE -2147483648
#define MAX_SIGNED_32_BIT_RANGE 2147483647

/* various constants */
#define FIRST_REGISTER 0 
#define LAST_REGISTER 31

#define MAX_FILE_NAME_LENGTH 255 + 1
#define STATEMENT_SIZE 80 + 1 
#define LABEL_SIZE 32
#define INSTRUCTION_SIZE 4

#define IC_START_VALUE 100
#define DC_START_VALUE 0

/* instruction counter and data counter - used to store the sizes between passes*/
int ICF, DCF;

/* this will be used mainly for log_error to determine where the error is occured */
extern int current_line;

/* structures for symbol table */
enum symbol_attributes
{
	CODE = 1 << 0,
	DATA = 1 << 1,
	ENTRY = 1 << 2,
	EXTERNAL = 1 << 3
}symbol_attributes;

/* this represents a single node inside symble table using linked list */
typedef struct symbol_node_t
{
	struct symbol_node_t* next;
	char 			name[32];
	int 			value;
	enum symbol_attributes attr;
}symbol_node_t;

/* this represents a single node inside code and data images using linked list */
typedef struct image_node_t
{
	struct image_node_t* next;
	unsigned char buffer;
}image_node_t;

/* using this structure to obtain instruction information */
typedef struct
{
	char name[5]; /* instruction name */
	char type[1]; /* R, I, J */
	int funct;
	int opcode;
}instruction_info_t;

/* structure used for 'R' type instructions */
typedef struct 
{
	int unused : 6;
	int funct : 5;
	int rd : 5; /* dest */
	int rt : 5; /* additional reg */
	int rs : 5; /* src */
	int opcode : 6;
}opcode_r_t;

/* structure used for 'I' type instructions */
typedef struct
{
	int immed : 16;
	int rt : 5;
	int rs : 5;
	int opcode : 6;
}opcode_i_t;

/* structure used for 'J' type instructions */
typedef struct
{
	int address : 25;
	int reg : 1;
	int opcode : 6;
}opcode_j_t;


#endif