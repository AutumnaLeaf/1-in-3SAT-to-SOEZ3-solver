#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h> 
#include "parser.h"

void free_system_data(SATSystem* system)
{
	int i;

	if (system)
	{
		if (system->matrix)
		{
			for (i = 0; i < system->rows; i++) { free(system->matrix[i]); }

			free(system->matrix);
		}

		if (system->Atlas) { free(system->Atlas); }

		free(system);

	}
}

int write_system_to_file(SATSystem* system, const char* output_file)
{
	FILE* file = NULL;
	int i, j;

	if (!system || !output_file) return 0;

	file = fopen(output_file, "w");
	if (!file)
	{
		fprintf(stderr, "Error: Could not open file for writing '%s' \n Error: '%s', Code: '%d' \n", output_file, strerror(errno), errno);
		return 0;
	}

	fprintf(file, "# Atlas:\n");
	if (system->Atlas == NULL)
	{
		fprintf(stderr, "Not Loaded\n");
	}
	else
	{
		for (i = 0; i < system->cols; i++)
		{
			fprintf(file, "[%d]", system->Atlas[i]);
		}
		fprintf(file, "\n");
	}

	fprintf(file, "# Matrix: %d clauses (rows) x %d variables (columns)\n", system->rows, system->cols - 1);

	for (i = 0; i < system->rows; i++)
	{
		for (j = 0; j < system->cols; j++)
		{
			fprintf(file, "%d ", system->matrix[i][j]);

		}
		fprintf(file, "\n");
	}

	fclose(file);
	return 1;


}
