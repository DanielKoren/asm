#include "output_files.h"
#include "logger.h"
#include "images.h"
#include "symbols.h"

#include <stdio.h>
#include <string.h>

 /** 
 * Creates .ob file and stores the machine code
 * @param filename the file's name
 */
void create_ob_file(const char* filename)
{
	char new_filename[MAX_FILE_NAME_LENGTH];
	FILE* file_stream;
	int i = 0, j = 0, k = 0;

	strncpy(new_filename, filename, sizeof(new_filename));
	strncat(new_filename, ".ob", sizeof(new_filename));

	file_stream = fopen(new_filename , "w");
	if (!file_stream)
	{
		log_info("failed to open %s", new_filename);
		return;
	}
	
	/* print icf, dcf */
	fprintf(file_stream, "     %d %d\n", ICF - IC_START_VALUE, DCF - DC_START_VALUE);
	
	reverse_code_image();
	{
		struct image_node_t* node = code_head;
		while (node)
		{
			if (j == 4) /*new line*/
			{
				fprintf(file_stream, "\n");
				j=0;
			}
			if (j == 0) /*address*/
			{
				int address = i + IC_START_VALUE;
				fprintf(file_stream, "%04d", address);
			}
			fprintf(file_stream, " %02x", node->buffer & 0xFF);
			j++;
			i++;
			node = node->next;
		}
	}
	reverse_data_image();
	{
		struct image_node_t* node = data_head;
		while (node)
		{
			if (j == 4) /*new line*/
			{
				fprintf(file_stream, "\n");
				j=0;
			}
			if (j == 0) /*address*/
			{
				int address = (i + IC_START_VALUE) + k + DC_START_VALUE;
				fprintf(file_stream, "%04d", address);
			}
			fprintf(file_stream, " %02x", node->buffer & 0xFF);
			j++;
			i++;
			node = node->next;
		}
	}

	fclose(file_stream);	
	
	log_success("%s file generated successfully", new_filename);
}

 /** 
 * Creates .ext file if any external labels are found
 * @param filename the file's name
 */
void create_ext_file(const char* filename)
{
	char new_filename[MAX_FILE_NAME_LENGTH];
	FILE* file_stream;
	struct symbol_node_t* node = external_head;
	int num_externals = 0;
	
	/* check if any external labels are present before creating file */
	while (node)
	{
		num_externals++;
		
		node = node->next;
	}
		
	if (!num_externals)
		return;
	
	strncpy(new_filename, filename, sizeof(new_filename));
	strncat(new_filename, ".ext", sizeof(new_filename));
	
	file_stream = fopen(new_filename, "w");
	if (!file_stream)
	{
		log_info("failed to open %s", new_filename);
		return;
	}
	
	node = external_head;
	while (node)
	{
		fprintf(file_stream, "%s %d\n", node->name, node->value);
	
		node = node->next;
	}
	
	fclose(file_stream);
	
	log_success("%s file generated successfully", new_filename);
}

 /**
 * Creates .ent file if any entry labels are output file
 * @param filename the file's name
 */
void create_ent_file(const char* filename)
{
	char new_filename[MAX_FILE_NAME_LENGTH];
	FILE* file_stream;
	struct symbol_node_t* node = symbol_head;
	int num_entries = 0;
	
	/* check if any entry symbol exist is empty before creating file */
	while (node)
	{
		if (node->attr == ENTRY || 
			node->attr == (CODE | ENTRY) || 
			node->attr == (DATA | ENTRY))
		{
			num_entries++;
		}
		node = node->next;
	}
	
	if (!num_entries)
		return;
	
	strncpy(new_filename, filename, sizeof(new_filename));
	strncat(new_filename, ".ent", sizeof(new_filename));
	
	file_stream = fopen(new_filename, "w");
	if (!file_stream)
	{
		log_info("failed to open %s", new_filename);
		return;
	}
	
	node = symbol_head;
	while (node)
	{
		if (node->attr == ENTRY ||
			node->attr == (CODE | ENTRY) || 
			node->attr == (DATA | ENTRY))
		{
			fprintf(file_stream, "%s %d\n", node->name, node->value);
		}
		
		node = node->next;
	}
	
	fclose(file_stream);
	
	log_success("%s file generated successfully", new_filename);
}
