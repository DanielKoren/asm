#ifndef _HELPERS_H
#define _HELPERS_H

#include "globals.h"

 /*
 * returns true if character is ascii 
 */
bool is_ascii(char ch);

 /**
 * checks whether the given token is a directive or not
 * @param token token
 * @return whether the given token is a directive
 */
bool is_directive(const char* token);

 /**
 * checks whether the given token is an instruction or not
 * @param token token
 * @return whether the given token is an instruction
 */
bool is_instruction(const char* token);

 /**
 * checks whether the given line is a comments line or the given line has no content
 * @param token token
 * @return whether the given line is a comments line or has no content
 */
bool is_comment_or_spaces(const char* statement);

 /**
 * checks whether the given label is valid
 * @param label label
 * @return whether the given label is valid
 */
bool is_label_valid(const char* label);

 /**
 * checks whether the given label already exists
 * @param label
 * @return whether the given label already exists or been used
 */
bool is_label_exists(const char* label);

 /**
 * gets the instruction info from the instruction name
 * @param name of the instruction
 * @return returns the instruction from the instruction_arr 
 */
instruction_info_t get_instruction(const char* name);

 /**
 * uses strtok to obtain register as operand 
 * @return register in integer
 */
int get_operand_reg();
 
 /**
 * uses strtok to obtain 16-bit integer
 * @return number in integer
 */
int get_operand_num();

 /**
 * uses strtok to obtain label
 * @return label 
 */
char* get_operand_label();

 /**
 * checks if any tokens left in statement using strtok 
 * @return if any found return true
 */
bool check_if_token_left();

#endif
