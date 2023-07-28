#ifndef _SYMBOLS_H
#define _SYMBOLS_H

#include "globals.h"

 /* global variable represents the head of our symbol table */
extern symbol_node_t* symbol_head;

 /** 
 * insert a node to the symbol table, using a name , value and attribute
 * @param name name of the node
 * @param value IC or DC value
 * @param attr the attribute of the node
 */
void insert_symbol(const char* name, int value, enum symbol_attributes attr);

 /** 
 * checks whether a given symbol is existent in the symbol table, if it exists, it puts the data of the node that we searched 
 * to the parameters given to the function
 * @param name name of the node
 * @param value IC or DC value
 * @param attr the attribute of the node
 * @return whether a given symbol is existent
 */
bool search_symbol(const char* name, int* value, enum symbol_attributes* attr);

 /** 
 * updates certain node value in the symbol table
 * @param name the name of the node
 * @param value IC or DC value
 * @param attr the attribute of the node 
 */
void update_symbol(const char* name, int value, enum symbol_attributes attr);

 /** 
 * adjusting value in node according to type
 * @param value adjustment value
 * @param attr the attribute of the node
 */ 
void add_value_to_type(int value, enum symbol_attributes attr);

 /**
 * frees the allocated dynamic memory for the linked list
 */
void free_symbols();

 /* global variable represents the head of our external table */
extern symbol_node_t* external_head;
 /** 
 * insert a node to the external table, attribute will stay EXTERNAL
 * @param name name of the node
 * @param memory address of where its being used
 */
void insert_external(const char* name, int value);

 /**
 * frees the allocated dynamic memory for the linked list
 */
void free_externals();

#endif
