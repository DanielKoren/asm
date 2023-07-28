#include "symbols.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* symbol table */

struct symbol_node_t* symbol_head = NULL;

int get_symbols_size()
{
	int size = 0;
	struct symbol_node_t* node = symbol_head;
	while (node)
	{
		size++;
		node = node->next;
	}
	return size;
}

void insert_symbol(const char* name, int value, enum symbol_attributes attr)
{
	struct symbol_node_t* node = malloc(sizeof(struct symbol_node_t));
	if (!node)
	{
		log_info("[-] failed allocating dynamic memory.\n");
		return;
	}
	
	node->attr = attr;
	strcpy(node->name, name);
	node->value = value;
	node->next = NULL;
	
	/* initialise symbol_head */
	if (!symbol_head)
	{
		symbol_head = node;
	}
	else
	{
		node->next = symbol_head;
		symbol_head = node;
	}
}

bool search_symbol(const char* name, int* value, enum symbol_attributes* attr)
{
	struct symbol_node_t* node = symbol_head;
	while (node)
	{
		if (!strcmp(node->name, name))
		{
			*value = node->value;
			*attr = node->attr;
			return true;
		}
		node = node->next;
	}
	return false;
}

void update_symbol(const char* name, int value, enum symbol_attributes attr)
{
	struct symbol_node_t* node = symbol_head;
	while (node)
	{
		if (!strcmp(node->name, name))
		{
			node->value = value;
			node->attr = attr;
			return;
		}
		node = node->next;
	}
}

void add_value_to_type(int value, enum symbol_attributes attr)
{
	struct symbol_node_t* node = symbol_head;
	while (node)
	{
		if (node->attr == attr)
		{
			node->value += value;
		}
		node = node->next;
	}
}

void free_symbols()
{
    struct symbol_node_t* prev = symbol_head;
 
    while (symbol_head)
    {
        symbol_head = symbol_head->next;
        free(prev);
        prev = symbol_head;
    }
}

/* external table */

struct symbol_node_t* external_head = NULL;

int get_externals_size()
{
	int size = 0;
	struct symbol_node_t* node = external_head;
	while (node)
	{
		size++;
		node = node->next;
	}
	return size;
}


void insert_external(const char* name, int value)
{
	struct symbol_node_t* node = malloc(sizeof(struct symbol_node_t));
	if (!node)
	{
		log_info("[-] failed allocating dynamic memory.\n");
		return;
	}
	
	node->attr = EXTERNAL;
	strcpy(node->name, name);
	node->value = value;
	node->next = NULL;
	
	/* initialise symbol_head */
	if (!external_head)
	{
		external_head = node;
	}
	else
	{
		node->next = external_head;
		external_head = node;
	}
}

void free_externals()
{
    struct symbol_node_t* prev = external_head;
 
    while (external_head)
    {
        external_head = external_head->next;
		free(prev);
		prev = external_head;
	}
}