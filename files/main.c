#include <stdio.h>
#include <string.h>
#include "logger.h"
#include "images.h"
#include "helpers.h"
#include "assembler.h"
#include "symbols.h"
#include "output_files.h"

/* open file stream and read and pass each line individually to next calls if first_pass() succeeded it will call second_pass() */
bool parse_as_file(const char* filename)
{
	FILE* file_stream = NULL;
	char statement[STATEMENT_SIZE];

	int instruction_counter = IC_START_VALUE;
	int data_counter = DC_START_VALUE;

	bool first_status = true, second_status = true;
	
	/* validate filename has .as extension */
	if (filename[strlen(filename) - 3] != '.' || 
		filename[strlen(filename) - 2] != 'a' || 
		filename[strlen(filename) - 1] != 's')
	{
		log_error("only .as files are supported");
		return false;
	}
	
	/* open file stream */
	file_stream = fopen(filename, "r");
	if (!file_stream)
	{
		log_info("failed reading %s, fopen() ret NULL", filename);
		return false;
	}
	
	/* first pass */
	for (current_line = 1; fgets(statement, sizeof(statement), file_stream) != NULL; current_line++)
	{
		if (is_comment_or_spaces(statement) || !strcmp(statement, ""))
			continue;
		
		if (!first_pass(statement, &instruction_counter, &data_counter))
		{
			first_status = false;
		}
	}
	
	ICF = instruction_counter;
	DCF = data_counter;
	add_value_to_type(ICF, DATA); /* adjust DATA symbols by adding icf */
	
	/* if first_status is non-zero call second pass() */
	if (first_status)
	{
		fseek(file_stream, 0, SEEK_SET); /* reset file pointer to start */
		instruction_counter = IC_START_VALUE; /* reset IC */
		
		for (current_line = 1; fgets(statement, sizeof(statement), file_stream) != NULL; current_line++)
		{	
			if (is_comment_or_spaces(statement))
				continue;
			
			if (!second_pass(statement, &instruction_counter))
			{
				second_status = false;
			}
		}
	}
	
	fclose(file_stream);
	
	return first_status && second_status;
}

int main(int argc, char** argv)
{
	int i;
	
	if (argc == 1)
	{
		log_info("please pass .as file/s");
		return EXIT_FAILURE;
	}
	
	for (i = 1; i < argc; i++)
	{
		char* file_name = argv[i];
		
		if (parse_as_file(file_name))
		{
			/* remove *.as extension */
			file_name[strlen(file_name) - 3] = '\0';
			
			create_ob_file(file_name);
			create_ext_file(file_name);
			create_ent_file(file_name);
			
			free_code_image();
			free_data_image();
			free_externals();
			free_symbols();
		}
	}
	
	return EXIT_SUCCESS;
}