#include "images.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>

/* data image */
struct image_node_t* data_head = NULL;

int get_data_size()
{
	int size = 0;
	struct image_node_t* node = data_head;
	while (node)
	{
		size++;
		node = node->next;
	}
	return size;
}

void insert_data_image(unsigned char buffer)
{
	struct image_node_t* node = malloc(sizeof(image_node_t));
	if (!node)
	{
		log_info("failed to allocate memory.\n");
		return;
	}
	
	node->buffer = buffer;
	node->next = NULL;
	
	/* initialise list */
	if (!data_head)
	{
		data_head = node;
	}
	else
	{
		node->next = data_head;
		data_head = node;
	}
}

void reverse_data_image()
{
	struct image_node_t* prev = NULL;
	struct image_node_t* current = data_head;
	struct image_node_t* next = NULL;
	while (current != NULL) 
	{
		next = current->next;
	
		current->next = prev;
	
		prev = current;
		current = next;
	}
	data_head = prev;
}

void free_data_image()
{
   struct image_node_t* prev = data_head;
 
    while (data_head)
    {
        data_head = data_head->next;
        free(prev);
        prev = data_head;
    }
}

/* code image */
struct image_node_t* code_head = NULL;

int get_code_size()
{
	int size = 0;
	struct image_node_t* node = code_head;
	while (node)
	{
		size++;
		node = node->next;
	}
	return size;
}

void insert_code_image(unsigned char buffer)
{
	struct image_node_t* node = malloc(sizeof(image_node_t));
	if (!node)
	{
		log_info("failed to allocate memory.\n");
		return;
	}
	
	node->buffer = buffer;
	node->next = NULL;
	
	/* initialise list */
	if (!code_head)
	{
		code_head = node;
	}
	else
	{
		node->next = code_head;
		code_head = node;
	}
}

void update_code_image(unsigned char buffer, int index)
{
	struct image_node_t* node = code_head;

	int i = 0;
	while (node)
	{
		if (i == index)
		{
			node->buffer = buffer;
			break;
		}
		i++;
		node = node->next;
	}
}

void reverse_code_image()
{
	struct image_node_t* prev = NULL;
	struct image_node_t* current = code_head;
	struct image_node_t* next = NULL;
	while (current != NULL) 
	{
		next = current->next;
	
		current->next = prev;
	
		prev = current;
		current = next;
	}
	code_head = prev;
}

void free_code_image()
{
   struct image_node_t* prev = code_head;
 
    while (code_head)
    {
        code_head = code_head->next;
        free(prev);
        prev = code_head;
    }
}