#include "helpers.h"
#include "logger.h"
#include "symbols.h"

#include <stdlib.h> /* atoi() */
#include <stdio.h>
#include <string.h>/* strlen() */
#include <ctype.h> /* isspace()/isalpha() */

bool is_ascii(char ch)
{
	return ((ch) >= 0 && (ch) <= 127);
}

const char *instructions_arr[] = 
{
	/* type R instructions */
	"add",
	"sub",
	"and",
	"or",
	"nor",
	"move",
	"mvhi",
	"mvlo",
	
	/* type I instructions */
	"addi",
	"subi",
	"andi",
	"ori",
	"nori",
	"bne",
	"beq",
	"blt",
	"bgt",
	"lb",
	"sb",
	"lw",
	"sw",
	"lh",
	"sh",
	
	/* type J instructions */
	"jmp",
	"la",
	"call",
	"stop"
};
const int instructions_arr_size = sizeof(instructions_arr) / sizeof(instructions_arr[0]);

const char *directives_arr[] = 
{
	".dd",
	".dh",
	".dw",
	".db", 
	".asciz"
	/*".entry",
	".extern"     taking care of them individually */
};
const int directives_arr_size = sizeof(directives_arr) / sizeof(directives_arr[0]);

bool is_directive(const char* token)
{
	int i = 0;
	
	for (i = 0; i < directives_arr_size; i++)
		if (!strcmp(token, directives_arr[i]))
			return true;
		
	return false;
}

bool is_instruction(const char* token)
{
	int i = 0;
	
	for (i = 0; i < instructions_arr_size; i++)
		if (!strcmp(token, instructions_arr[i]))
			return true;
	
	return false;
}

bool is_comment_or_spaces(const char* statement)
{
  int i = 0;
  bool status = false;

  if(statement[0] == ';')
    status = true;

	while(statement[i] != '\0')
	{
	if(isspace(statement[i]))
		i++;
	else
		break; /* it means there is one index that is not space which means this is definitely not a comment nor a spaces line */
	}

	if(statement[i] == '\0')
		status = true;


	return status;
}

bool is_label_valid(const char* label)
{	
	if (!isalpha(label[0]))
	{
		return false;
	}
	
	if (strlen(label) > 32)
	{
		return false;
	}
	
	/* label cannot have saved keywords */
	if (is_instruction(label))
		return false;
	
	if (is_directive(label))
		return false;
	
	if (!strcmp(label, ".extern") || !strcmp(label, ".entry"))
		return false;
	
	return true;
}

bool is_label_exists(const char* label)
{
	int value;
	enum symbol_attributes attributes;
	
	if (search_symbol(label, &value, &attributes))
	{
		return true;
	}
		
	return false;
}

instruction_info_t instructions_info_arr[] =
{
	{"add", "R", 1, 0},
	{"sub", "R", 2, 0},
	{"and", "R", 3, 0},
	{"or", "R", 4, 0},
	{"nor", "R", 5, 0},
	{"move", "R", 1, 1},
	{"mvhi", "R", 2, 1},
	{"mvlo", "R", 3, 1},
	
	{"addi", "I", 0, 10},
	{"subi", "I", 0, 11},
	{"andi", "I", 0, 12},
	{"ori", "I", 0, 13},
	{"nori", "I", 0, 14},
	{"bne", "I", 0, 15},
	{"beq", "I", 0, 16},
	{"blt", "I", 0, 17},
	{"bgt", "I", 0, 18},
	{"lb", "I", 0, 19},
	{"sb", "I", 0, 20},
	{"lw", "I", 0, 21},
	{"sw", "I", 0, 22},
	{"lh", "I", 0, 23},
	{"sh", "I", 0, 24},
	
	{"jmp", "J", 0, 30},
	{"la", "J", 0, 31},
	{"call", "J", 0, 32},
	{"stop", "J", 0, 63}
};

const int instructions_info_arr_size = sizeof(instructions_arr) / sizeof(instructions_arr[0]);

instruction_info_t get_instruction(const char* name)
{
	instruction_info_t result;
	int i;
	
	for (i = 0; i < instructions_info_arr_size; i++)
	{
		if (!strcmp(name, instructions_info_arr[i].name))
		{
			return instructions_info_arr[i];
		}
	}
	
	return result;
}
	
int get_operand_reg()
{
	char* register_str = NULL;
	int   register_int = -1;
	char* stol_ptr = NULL;
	
	register_str = strtok(NULL, " ,\n\t\r");
	if (!register_str)
	{
		log_error("missing operand");
		return -1;
	}
	
	if (register_str[0] != '$')
	{
		log_error("expected register as operand, invalid character");
		return -1;
	}
	
	register_str = register_str + 1;
	register_int = strtol(register_str, &stol_ptr, 10);

	if (*stol_ptr != '\0')
	{
		log_error("invalid register");
		return -1;
	}
	if (register_int < 0 || register_int > 31)
	{
		log_error("available registers are between $0 and $31");
		return -1;
	}
	
	return register_int;
}

int get_operand_num()
{
	char* num_str = NULL;
	int num_int = 0;
	char* ptr;
	
	num_str = strtok(NULL, ", ");
	if (!num_str)
	{
		log_error("missing operand");		
		return -1;
	}
	
	/* this checks if num_str has only numbers and converts string to long */
	num_int = strtol(num_str, &ptr, 10);
	if (*ptr != '\0')
	{
		log_error("second operand only supports signed 16-bit value");
		return -1;
	}
	
	if (num_int < MIN_SIGNED_16_BIT_RANGE || num_int > MAX_SIGNED_16_BIT_RANGE)
	{
		log_error("second operand only supports signed 16-bit value");
		return -1;
	}
	
	return num_int;
}

char* get_operand_label()
{
	char* label_name = strtok(NULL, " \n\t\r");
	if (!label_name)
	{
		log_error("missing operand label");
		return NULL;
	}
	
	if (!is_label_valid(label_name))
	{
		log_error("invalid name for label");
		return NULL;
	}
	
	return label_name;
}

bool check_if_token_left()
{
	char* token = strtok(NULL, " ,\r\n\t");
	return (token == NULL) ? false : true;
}