#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include "parser.h"

#define BUFFER_SIZE 1024


static int whitespace_check(char c) /** return true if whitespace */
{
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int get_problem_line(char* buffer, size_t buffer_size, FILE* file, SATSystem* system)
{
	while (fgets(buffer, buffer_size, file))
	{
		char* ptr = buffer;

		while (*ptr && whitespace_check(*ptr)) ptr++;

		if (*ptr == '\0' || *ptr == 'c') continue;

		if (*ptr == 'p')
		{
			char file_format[10];
			memset(file_format, 0, sizeof(file_format));

			int scan_return_value;

			scan_return_value =
				sscanf(ptr, "p %9s %d %d", file_format, &system->cols, &system->rows);

			if (scan_return_value == 3 && strcmp(file_format, "cnf") == 0 &&
				system->cols > 0 && system->rows > 0)
			{
				return 1;
			}

		}

	}
	return 0;
}

int parse_clauses(char* buffer, size_t buffer_size, FILE* file, SATSystem* system)
{
	int current_clause = 0;

	while (fgets(buffer, buffer_size, file))
	{
		char* ptr = buffer;

		while (*ptr && whitespace_check(*ptr)) ptr++;

		if (*ptr == '\0' || *ptr == 'c' || *ptr == 'p') continue;

		while (*ptr && current_clause < system->rows)
		{
			long raw_value; /** output of strtol */
			char* next_ptr;
			int index;

			while (*ptr && whitespace_check(*ptr)) ptr++;

			raw_value = strtol(ptr, &next_ptr, 10);

			if (next_ptr == ptr)
			{
				ptr++;
				continue;
			}
			ptr = next_ptr;
			if (raw_value == 0)
			{
				current_clause++;
				//continue;
				break;
			}

			index = (raw_value > 0) ? raw_value : -raw_value;
			index -= 1; /** variables are in range 1-n, therefore we change bounds*/

			if (index < (system->cols - 1))
			{
				if (raw_value > 0)
				{
					system->matrix[current_clause][index] = 1; /** */
				}
				else
				{
					system->matrix[current_clause][index] = 2; /** - symbolizez negation, we conver it folowing the formula -x_1 = 2x_1 + 1*/

					system->matrix[current_clause][system->cols - 1] =
						(system->matrix[current_clause][system->cols - 1] + 2) % 3;


				}
			}
			else
			{
				fprintf(stderr, "Warning: Index of variable %d exceeds possible max index %d\n", index + 1, system->cols - 1);
			}


		}

		if (current_clause >= system->rows) { break; }
	}

	return current_clause;
}

SATSystem* parse_cnf_to_matrix(const char* cnf_file)
{
	FILE* file = NULL;
	SATSystem* system;
	char buffer[BUFFER_SIZE];
	int current_clause = 0;
	int i, j;

	if (!cnf_file)
	{
		fprintf(stderr, "Error: file or path provided is NULL\n");
		return NULL;
	}

	file = fopen(cnf_file, "r");
	if (!file)
	{
		fprintf(stderr, "Error: Could not open file '%s' \n Error: '%s', Code: '%d' \n", cnf_file, strerror(errno), errno);
		return NULL;
	}

	system = malloc(sizeof(SATSystem));
	if (!system)
	{
		fprintf(stderr, "Error: Memory Allocation for SATSystem failed\n");
		fclose(file);
		return NULL;
	}

	system->rows = 0;
	system->cols = 0;
	system->matrix = NULL;
	system->Atlas = NULL;

	if (!get_problem_line(buffer, sizeof(buffer), file, system))
	{
		fprintf(stderr, "Error: Failed to parse / Invalid or missing problem line in CNF file\n");
		free(system);
		fclose(file);
		return NULL;
	}

	system->cols++; /** additional column for solutions*/

	system->matrix = malloc(system->rows * sizeof(int*));
	if (!system->matrix)
	{
		fprintf(stderr, "Error: Memory Allocation for system->matrix rows failed\n");
		free(system);
		fclose(file);
		return NULL;
	}

	for (i = 0; i < system->rows; i++)
	{
		system->matrix[i] = calloc(system->cols, sizeof(int));
		if (!system->matrix[i])
		{
			fprintf(stderr, "Error: Memory Alocation for system->matrix[%d] failed\n", i);
			for (j = 0; j < i; j++) { free(system->matrix[j]); }

			free(system->matrix);
			free(system);
			fclose(file);
			return NULL;

		}

		system->matrix[i][system->cols - 1] = 1; /** system is equal to 1 if and only if exactly one of three variables is true*/
	}

	rewind(file);
	current_clause = parse_clauses(buffer, sizeof(buffer), file, system);

	fclose(file);

	if (current_clause < system->rows)
	{
		fprintf(stderr, "Warning: File contains fewer clauses %d then decleared %d\n", current_clause, system->rows);

	}

	return system;

}