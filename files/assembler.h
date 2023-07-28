#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

#include "globals.h"

 /*
 * 
 * @param statement representation of the line
 * @param ic_ptr the instruction counter pointer
 * @param dc_ptr the data counter pointer
 * @return Whether first pass succeeded or not
 */
bool first_pass(char* statement, int* ic_ptr, int* dc_ptr);

/*
 * Activates the second pass on the given file
 * @param statement representation of the line
 * @param ic_ptr the instruction counter pointer
 * @return Whether second pass succeeded or not
 */   
bool second_pass(char* statement, int* ic_ptr);

#endif
