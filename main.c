#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "elim.h"
#include "bforce.h"
#include "utils.h"

int main(int argc, char* argv[])
{
	const char* cnf_file;
	const char* output_file1;
	const char* output_file2;
	const char* output_file3;

	if (argc < 2 || argc > 5) {
		fprintf(stderr, "Usage: %s <cnf_file> [output_file1] [const char* output_file2] [const char* output_file3;]\n", argv[0]);
		return 1;
	}
	
	cnf_file = argv[1];
	output_file1 = (argc >= 3) ? argv[2] : NULL;
	output_file2 = (argc >= 4) ? argv[3] : NULL;
	output_file3 = (argc >= 5) ? argv[4] : NULL;

	
	SATSystem* system = parse_cnf_to_matrix(cnf_file);
	if (!system)
	{
		fprintf(stderr, "Error: failed to parse cnf_file\n");
		return 1;
	}

	if (output_file1)
	{
		if (write_system_to_file(system, output_file1))
		{
			printf("Saved to: %s\n", output_file1);
		}
		else
		{
			fprintf(stderr, "Error: writing to file failed\n");
			free_system_data(system);
			return 1;
		}

	}

	reduce_matrix(system);
	if (!system)
	{
		fprintf(stderr, "Error: failed to parse cnf_file\n");
		return 1;
	}

	if (output_file2)
	{
		if (write_system_to_file(system, output_file2))
		{
			printf("Saved to: %s\n", output_file2);
		}
		else
		{
			fprintf(stderr, "Error: writing to file failed\n");
			free_system_data(system);
			return 1;
		}

	}

	find_solution(system);

	if (!system)
	{
		fprintf(stderr, "Error: failed to parse cnf_file\n");
		return 1;
	}

	if (output_file3)
	{
		if (write_system_to_file(system, output_file3))
		{
			printf("Saved to: %s\n", output_file3);
		}
		else
		{
			fprintf(stderr, "Error: writing to file failed\n");
			free_system_data(system);
			return 1;
		}

	}

	free_system_data(system);
	return 0;


}
