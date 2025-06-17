#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "elim.h"
#include "utils.h"


int solve_for_id(SATSystem* system)
{
	int row;
	int rank = 0;

	for (row = 0; row < system->rows; row++)
	{
		rank += system->Atlas[row];

		if (system->matrix[row][system->cols - 1] == 2)
		{
			return 0;
		}
		system->Atlas[row] = system->matrix[row][system->cols - 1];


	}
	return rank;
}

int rearrange_rhs_lhs(SATSystem* system)
{
	int rank = 0;
	int row, col;

	for (col = 0; col < system->cols; col++)
	{
		for (row = 0; row < system->rows; row++)
		{
			if (system->Atlas[col] == 0)
			{
				system->matrix[row][col] = negate[system->matrix[row][col]]; // carrying non-pivots to LHS, solutions to RHS
			}
			if (col == system->cols - 1)
			{
				system->matrix[row][col] = addt_t[system->matrix[row][col]][2];// we set pivots  = 2 and add solution to them ^
			}

		}
		rank += system->Atlas[col];
	}

	return rank;

}

int bruteforce(SATSystem* system, int* variable_index, int* pivot_holder)
{
	int row, col, solution;
	unsigned long long sample;
	int flag = 0;
	unsigned long long possibilities = 1ULL << (system->cols - system->rows); // 2^(system->cols - system->rows) we move ones to the left

	for (sample = 0; sample < possibilities; sample++)
	{
		for (col = 0; col < system->cols - system->rows; col++)
		{
			system->Atlas[variable_index[col]] = (sample & 1ULL << col) ? 1 : 0;

		}
		flag = 1;

		for (row = 0; row < system->rows; row++)
		{
			solution = 0;

			for (col = 0; col < system->cols - system->rows; col++)
			{
				solution = addt_t[multi_t[system->Atlas[variable_index[col]]][system->matrix[row][variable_index[col]]]][solution];
				/**
				 * Calculate solution for LHS by iterating though every entry in row adding it,
				 * Calculate single entry by multiplying coefficient * value
				 * coefficient -> system->matrix[row][variable_index[col]]
				 * value from vector-> system->Atlas[variable_index[col]]
				 */

			}
			if (solution == 2)
			{
				flag = 0;
				break;

			}
		}
		if (flag)
		{
			break;
		}
	}
	return flag;

}

void calculate_pivots(SATSystem* system, int* variable_index, int* pivot_holder)
{
	int row, col;
	int solution;

	for (row = 0; row < system->rows; row++)
	{
		system->matrix[row][system->cols - 1] = negate[addt_t[system->matrix[row][system->cols - 1]][1]];
		//reverse operation performed on ssolution column add 1, then negate, equals original value 

		solution = 0;
		for (col = 0; col < system->cols - system->rows; col++)
		{
			solution = addt_t[multi_t[system->Atlas[variable_index[col]]][system->matrix[row][variable_index[col]]]][solution];
			/**single value calculated by substituting value form solution vector and multiplying by coefficient
			 * Add all of this to get solution for current row
			 *
			 */
		}

		solution = addt_t[solution][system->matrix[row][system->cols - 1]];
		//To get complete RHS solution value we add constant

		system->Atlas[pivot_holder[row]] = solution;
	}

}



SATSystem* find_solution(SATSystem* system)
{
	int row, col;

	int* variable_index = calloc((system->cols - system->rows), sizeof(int));
	if (!variable_index)
	{
		fprintf(stderr, "Error: Memory Allocation for variable_index failed\n");
		free_system_data(system);
		return NULL;
	}

	int* pivot_holder = calloc(system->rows, sizeof(int));
	if (!pivot_holder)
	{
		fprintf(stderr, "Error: Memory Allocation for pivot_holder failed\n");
		free_system_data(system);
		return NULL;
	}

	int inc = 0;
	int inc2 = 0;
	for (col = 0; col < system->cols; col++)
	{
		if (system->Atlas[col] == 0)
		{
			variable_index[inc] = col;
			inc += 1;
		}
		else
		{
			pivot_holder[inc2] = col;
			inc2 += 1;
		}

	}


	if (system->rows == system->cols - 1)
	{
		int rank = solve_for_id(system);

		if (system->rows != rank)
		{
			fprintf(stderr, "Error: Number of pivots not equal to number of rows\n");
			free_system_data(system);
			return NULL;
		}
		if (!(rank))
		{
			fprintf(stderr, "(I)System has no solutions satisfying solutions\n");
			free_system_data(system);
			exit(EXIT_SUCCESS);

		}

	}
	else
	{

		if (system->rows != rearrange_rhs_lhs(system))
		{
			fprintf(stderr, "Error: Number of pivots not equal to number of rows\n");
			free_system_data(system);
			return NULL;
		}

		if ((system->cols - system->rows) > 63)
		{
			fprintf(stderr, "To large to handle in realistic time with this approach\n");
			free_system_data(system);
			exit(EXIT_FAILURE);

		}
		if (bruteforce(system, variable_index, pivot_holder))
		{
			calculate_pivots(system, variable_index, pivot_holder);
			fprintf(stderr, "System has solutions");
		}
		else
		{
			fprintf(stderr, "System has no solutions all possiblities checked\n");
		}


	}
	free(variable_index);
	free(pivot_holder);
	return system;




}
