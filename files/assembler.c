#include "assembler.h"
#include "logger.h"
#include "symbols.h"
#include "helpers.h"
#include "images.h"
#include <string.h>/* strtok() */
#include <stdlib.h>/* atoi() */
#include <ctype.h> /* isdigit() */

int current_line = 0; /* */

bool handle_string(int* dc_ptr);
bool handle_data(const char* operation, int* dc_ptr);
bool handle_instruction(const char* operation, int* ic_ptr);

bool update_i_instruction(const char* operation, int opcode, int* ic_ptr);
bool update_j_instruction(const char* operation, int opcode, int* ic_ptr);

bool first_pass(char* statement, int* ic_ptr, int* dc_ptr)
{
	char* token = NULL;
	bool label_found = false;
	char label_str[LABEL_SIZE];

	token = strtok(statement, " \n\t\r");
	
	/* first check if label is defined */
	if (token[strlen(token) - 1] == ':')
	{
		/* remove last character e.g. : */
		token[strlen(token) - 1] = '\0';
		
		if (!is_label_valid(token))
		{
			log_error("invalid name for label");
			return false;
		}
		
		if (is_label_exists(token))
		{
			log_error("label is already defined");
			return false;
		}
		
		label_found = true;
		strcpy(label_str, token);
		token = strtok(NULL, " \n\t\r");
	}
	
	/* add normal or extern label to symbol table if found and ignore .entry statements - they're processed in second pass */
	if (is_directive(token) && label_found)
	{
		insert_symbol(label_str, *dc_ptr, DATA);
	}
	else if (is_instruction(token) && label_found)
	{
		insert_symbol(label_str, *ic_ptr, CODE);
	}
	else if (!strcmp(token, ".extern"))
	{
		int value;
		enum symbol_attributes attributes;
		bool label_exist;
		
		char* label_name = get_operand_label();
		if (!label_name)
			return false;
		
		label_exist = search_symbol(label_name, &value, &attributes);
		if (label_exist && attributes != EXTERNAL)
		{
			log_error("local label cannot be declared as external");
			return false;
		}
		
		insert_symbol(label_name, 0, EXTERNAL);
		return true;
	}
	else if (!strcmp(token, ".entry"))
	{
		/* 'entry' statements will be processed in second_pass()
			this will make sure that the label exist */
		char* label_name = strtok(NULL, " \n\r\t");
		if (!label_name)
		{
			log_error("missing operand label");
			return false;
		}
		
		return true;
	}
	
	/* store data in global arrays accordingly */
	if (!strcmp(token, ".asciz"))
	{
		return handle_string(dc_ptr);
	}
	else if (!strcmp(token, ".dw") || 
			!strcmp(token, ".dh")  || 
			!strcmp(token, ".db"))
	{
		return handle_data(token, dc_ptr);
	}
	else if (is_instruction(token))
	{
		return handle_instruction(token, ic_ptr);
	}
	
	log_error("unrecognized keyword statement %s", token);
	
	return false;
}

bool second_pass(char* statement, int* ic_ptr)
{
	char* token = NULL;
	
	token = strtok(statement, " \n\t\r");
	
	/* if first token is label, skip to the next token */
	if (token[strlen(token) - 1] == ':')
	{
		token = strtok(NULL, " \n\t\r");
	}
	
	if (!strcmp(token, ".entry"))
	{
		int value;
		enum symbol_attributes attributes;
		
		char* label_name = get_operand_label();
		if (!label_name)
			return false;
		
		if (!search_symbol(label_name, &value, &attributes))
		{
			log_error("label is not defined");
			return false;
		}
		
		update_symbol(label_name, value, attributes | ENTRY);
		return true;
	}

	if (is_instruction(token))
	{
		instruction_info_t info = get_instruction(token);
		if (info.type[0] == 'I')
		{
			if (!strcmp(token, "bne") || 
				!strcmp(token, "beq") || 
				!strcmp(token, "blt") || 
				!strcmp(token, "bgt"))
			{
				return update_i_instruction(token, info.opcode, ic_ptr);
			}
		}
		if (info.type[0] == 'J') 
		{
			if (!strcmp(token, "jmp") ||
				!strcmp(token, "la") ||
				!strcmp(token, "call"))
			{
				return update_j_instruction(token, info.opcode, ic_ptr);
			}
		}
		else
			(*ic_ptr) += INSTRUCTION_SIZE; /* every struct is 4 bytes */
	}
	
	
	
	return true;
}

 /*
 * handles '.asciz' operation, including error checks for incorrect syntax or malfunctionings
 * @param dc_ptr the data counter pointer
 * @return whether we could handle '.asciz' operation on a certain given operand
 */
bool handle_string(int* dc_ptr)
{
	char* str;
	int i;
	
	str = strtok(NULL, "\"");
	if (!str)
	{
		log_error("missing operand string");
		return false;
	}
	
	/* check for valid characters */
	for (i = 0; i < strlen(str); i++)
	{
		if (is_ascii(str[i]) == 0)
		{
			log_error("invalid character in string");
			return false;
		}
	}
	
	/* store each character in our global variable */
	for (i = 0; i < strlen(str); i++)
	{
		insert_data_image(str[i]);
		(*dc_ptr)++;
	}
	
	insert_data_image('\0');
	(*dc_ptr)++;
	
	return true;
}

 /*
 * handles '.db' , '.dh' , '.dw' operations, including error checks for correct size ranges 
 * @param operation the given operation (.db / .dh / .dw)
 * @return Whether we could handle the data operations on certain given operands
 */
bool handle_data(const char* operation, int* dc_ptr)
{
	char* token;
	long num;
	char* ptr;
	int temp_num;
	
	token = strtok(NULL, " ,\t\r\n\v\f");
	if (!token)
	{
		log_error("missing operand data");
		return false;
	}
	
	while (token != NULL)
	{
		num = strtol(token, &ptr, 10);
		if (!num)
		{
			log_error("unexpected character found, allowed only integers");
			return false;
		}	
		if (!strcmp(operation, ".db"))
		{
			if (num < MIN_SIGNED_8_BIT_RANGE || num > MAX_SIGNED_8_BIT_RANGE)
			{
				log_error("invalid size, only 8-bit signed value is allowed");
				return false;
			}
			insert_data_image(num);
			(*dc_ptr)++;
		}
		if (!strcmp(operation, ".dh"))
		{
			if (num < MIN_SIGNED_16_BIT_RANGE || num > MAX_SIGNED_16_BIT_RANGE)
			{
				log_error("invalid size, only 16-bit signed value is allowed");
				return false;
			}
			
			temp_num = (num & 0xFF);
			insert_data_image(temp_num);
			(*dc_ptr)++;
			
			temp_num = (num >> 8) & 0xFF;
			insert_data_image(temp_num);
			(*dc_ptr)++;
		}
		if (!strcmp(operation, ".dw"))
		{
			if (num < MIN_SIGNED_32_BIT_RANGE || num > MAX_SIGNED_32_BIT_RANGE)
			{
				log_error("invalid size, only 32-bit signed value is allowed");
				return false;
			}
			/* using bit-shift register to obtain each byte */
			temp_num = (num & 0xFF);
			insert_data_image(temp_num);
			(*dc_ptr)++;
			
			temp_num = (num >> 8) & 0xFF;
			insert_data_image(temp_num);
			(*dc_ptr)++;
			
			temp_num = (num >> 16) & 0xFF;
			insert_data_image(temp_num);
			(*dc_ptr)++;
			
			temp_num = (num >> 24) & 0xFF;
			insert_data_image(temp_num);
			(*dc_ptr)++;
		}
		
		token = strtok(NULL, " ,\t\r\n\v\f");
	}

	return true;
}

 /*
 * handles R/I/J types of instructions, each one of the type receives a specific binary encoding 
 * @param operation the given operation (that is either R/I/J type of operation)
 * @param ic_ptr the instruction counter pointer
 */
bool handle_instruction(const char* operation, int* ic_ptr)
{
	instruction_info_t info = get_instruction(operation);
	
	char* opcode_ptr;
	int   opcode_len;
	int i;
	
	if (info.type[0] == 'R')
	{
		opcode_r_t opcode_data;		
		int reg_value;
		
		/* analyze 3 operand instruction */
		if (info.opcode == 0)
		{
			opcode_data.unused = 0;
			opcode_data.funct = info.funct;
			/* getting first register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rs = reg_value;
			
			/* getting second register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rt = reg_value;
			
			/* getting third register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.rd = reg_value;
			opcode_data.opcode = info.opcode;
		}
		/* analyze 2 operand instruction */
		else if (info.opcode == 1)
		{
			opcode_data.unused = 0;
			opcode_data.funct = info.funct;
			/* getting first register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rs = reg_value;
			/* getting second register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rd = reg_value;
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.rt = 0;
			opcode_data.opcode = info.opcode;
		}
		
		opcode_len = sizeof(opcode_data);
		opcode_ptr = malloc(opcode_len);
		if (!opcode_ptr)
		{
			log_info("failed allocating dynamic memory.\n");
			return false;
		}
		memcpy(opcode_ptr, &opcode_data, opcode_len);
		
		for (i = 0; i < opcode_len; i++)
		{
			insert_code_image(opcode_ptr[i]);
			(*ic_ptr)++;
		}
		
		free(opcode_ptr);
				
		return true;
	}
	else if (info.type[0] == 'I')
	{
		opcode_i_t opcode_data;
		int reg_value;
		int num_value;
		char* label_name = NULL;
		
		/* handle register, register, label */
		if (!strcmp(operation, "bne") || 
			!strcmp(operation, "beq") || 
			!strcmp(operation, "blt") || 
			!strcmp(operation, "bgt"))
		{
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rs = reg_value;
			
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rt = reg_value;
			
			label_name = strtok(NULL, " \n\t\r");
			if (!label_name)
			{
				log_error("missing operand");
				return false;
			}
			if (!is_label_valid(label_name))
			{
				log_error("invalid name for label");
				return false;
			}
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.immed = 0;
			opcode_data.opcode = info.opcode;
		}
		/* handle register, number, register */
		else
		{
			/* first operand*/
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			opcode_data.rs = reg_value;
			
			/* second operand */
			num_value = get_operand_num();
			if (num_value == -1)
				return false;
			opcode_data.immed = num_value;
			
			/* getting third register */
			reg_value = get_operand_reg();
			if (reg_value == -1)
				return false;
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.rt = reg_value;
			opcode_data.opcode = info.opcode;	
		}
		opcode_len = sizeof(opcode_data); /* 4 bytes */
		opcode_ptr = malloc(opcode_len);
		if (!opcode_ptr)
		{
			log_info("failed allocating dynamic memory.\n");
			return false;
		}
		memcpy(opcode_ptr, &opcode_data, opcode_len);
		
		for (i = 0; i < opcode_len; i++)
		{
			insert_code_image(opcode_ptr[i]);
			(*ic_ptr)++;
		}
		
		free(opcode_ptr);
		
		return true;
	}
	else if (info.type[0] == 'J')
	{
		opcode_j_t opcode_data;
		
		if (!strcmp(info.name, "jmp"))
		{
			int reg_value;
			char* token = strtok(NULL, " \n\t");
			if (!token)
			{
				log_error("missing operand");
				return false;
			}
			
			/* labels cannot start with numbers */
			if (isdigit(token[0]))
			{
				log_error("invalid jmp operand");
				return false;
			}
			
			/* operand can be register */
			if (token[0] == '$')
			{
				const int val = 1;
				char* ptr = NULL;
				
				token = token + 1;
				reg_value = strtol(token, &ptr, 10);
		
				if (reg_value < 0 || reg_value > 31)
				{
					log_error("available registers are between $0 and $31");
					return false;
				}
				
				opcode_data.address = reg_value;
				opcode_data.reg = val;
			}
			/* operand is a label */
			else if (is_label_valid(token))
			{
				opcode_data.address = 0;
				opcode_data.reg = 0;
			}
			else
			{
				log_error("invalid jmp operand.\n");
				return false;
			}
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.opcode = info.opcode;
		}
		else if (!strcmp(info.name, "la"))
		{
			char* label_name = get_operand_label();
			if (!label_name)
			{
				return false;
			}
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.address = 0;
			opcode_data.reg = 0;
			opcode_data.opcode = info.opcode;
		}
		else if (!strcmp(info.name, "call"))
		{
			char* label_name = get_operand_label();
			if (!label_name)
			{
				return false;
			}
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
			
			opcode_data.address = 0;
			opcode_data.reg = 0;
			opcode_data.opcode = info.opcode;
		}
		else if (!strcmp(info.name, "stop"))
		{
			opcode_data.address = 0;
			opcode_data.reg = 0;
			opcode_data.opcode = info.opcode;
			
			if (check_if_token_left())
			{
				log_error("invalid characters found");
				return false;
			}
		}
		
		opcode_len = sizeof(opcode_data);
		opcode_ptr = malloc(opcode_len);
		if (!opcode_ptr)
		{
			log_info("failed allocating dynamic memory.\n");
			return false;
		}
		memcpy(opcode_ptr, &opcode_data, opcode_len);
		
		for (i = 0; i < opcode_len; i++)
		{
			insert_code_image(opcode_ptr[i]);
			(*ic_ptr)++;
		}
		
		free(opcode_ptr);
		
		return true;
	}
	
	return true;
}

 /*
 * updates the I instructions in the symbol table after first pass succeeded
 * @param operation the given operation (I type)
 * @param opcode the opcode of the instruction
 * @param ic_ptr the instruction counter pointer
 * return whether the update succeeded or not
 */
bool update_i_instruction(const char* operation, int opcode, int* ic_ptr)
{
	opcode_i_t opcode_data;
	char* opcode_ptr;
	int opcode_len;
	int i;
	int reg_value;
	char* label_name = NULL;
	
	int value;
	enum symbol_attributes attributes;
	
	opcode_data.opcode = opcode;
	reg_value = get_operand_reg();
	if (reg_value == -1)
		return false;
	opcode_data.rs = reg_value;
	
	reg_value = get_operand_reg();
	if (reg_value == -1)
		return false;
	opcode_data.rt = reg_value;
	
	label_name = strtok(NULL, " \n\t\r");
	if (!label_name)
	{
		log_error("missing operand");
		return false;
	}
	if (!is_label_valid(label_name))
	{
		log_error("invalid name for label");
		return false;
	}
	
	/* check if label is defined in our current file */
	if (!search_symbol(label_name, &value, &attributes))
	{
		log_error("label is not found");
		return false;
	}
	
	if (attributes == EXTERNAL)
	{
		log_error("label is external");
		return false;
	}
	
	opcode_data.immed = value - (*ic_ptr);
	
	opcode_len = sizeof(opcode_data); /* 4 bytes */
	opcode_ptr = malloc(opcode_len);
	if (!opcode_ptr)
	{
		log_info("failed allocating dynamic memory");
		return false;
	}
	memcpy(opcode_ptr, &opcode_data, opcode_len);
	
	for (i = 0; i < opcode_len; i++)
	{
		update_code_image(opcode_ptr[i], get_code_size() - (*ic_ptr - IC_START_VALUE) - 1);
		(*ic_ptr)++;
	}
	
	free(opcode_ptr);
	
	return true;
}

 /*
 * updates the J instructions in the symbol table after first pass succeeded
 * @param operation the given operation (J type)
 * @param opcode the opcode of the instruction
 * @param ic_ptr the instruction counter pointer
 * return whether the update succeeded or not
 */
bool update_j_instruction(const char* operation, int opcode, int* ic_ptr)
{
	opcode_j_t  opcode_data;
	int 		opcode_len;
	char* 		opcode_ptr;

	char* 		label_name;
		
	int i;
	
	int value;
	enum symbol_attributes attributes;
	
	label_name = strtok(NULL, " \n\t\r");
	if (!label_name)
	{
		log_error("missing operand");
		return false;
	}
	
	/* skip registers */
	if (label_name[0] == '$')
	{
		(*ic_ptr) += INSTRUCTION_SIZE;
		return true;
	}
	
	/* check if label exist */
	if (!search_symbol(label_name, &value, &attributes))
	{
		log_error("label is not defined");
		return false;
	}
	
	if (attributes == EXTERNAL)
	{
		insert_external(label_name, *ic_ptr);
	}
	
	opcode_data.address = value;
	opcode_data.reg = 0;
	opcode_data.opcode = opcode;
	
	opcode_len = sizeof(opcode_data);
	opcode_ptr = malloc(opcode_len);
	if (!opcode_ptr)
	{
		log_info("failed allocating dynamic memory.\n");
		return false;
	}
	memcpy(opcode_ptr, &opcode_data, opcode_len);
	
	for (i = 0; i < opcode_len; i++)
	{
		update_code_image(opcode_ptr[i], get_code_size() - (*ic_ptr - IC_START_VALUE) - 1);
		(*ic_ptr)++;
	}
	
	free(opcode_ptr);
	return true;
}
