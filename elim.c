#include <stdio.h>
#include <stdlib.h>
#include "parser.h"
#include "elim.h"
#include "utils.h"

const unsigned char multi_t[3][3] =
{
	{ 0, 0, 0 },
	{ 0, 1, 2 },
	{ 0, 2, 1 }


};

const unsigned char addt_t[3][3] =
{
	{ 0, 1, 2 },
	{ 1, 2, 0 },
	{ 2, 0, 1 }


};

const unsigned char negate[3] = { 0, 2, 1 };

void gaussian_elimination_mod3(SATSystem* system)
{
	int row, col, j;
	int pivot_row = 0;

	for (col = 0; col < system->cols - 1; col++)
	{
		int pivot = pivot_row;

		while (pivot < system->rows && system->matrix[pivot][col] == 0) { pivot++; }

		if (pivot < system->rows)
		{
			if (pivot != pivot_row)
			{
				int* temp = system->matrix[pivot_row];
				system->matrix[pivot_row] = system->matrix[pivot];
				system->matrix[pivot] = temp;

			}

			int pivot_value = system->matrix[pivot_row][col];

			if (pivot_value == 2) // normalize pivot value
			{
				for (j = col; j < system->cols; j++)
				{
					system->matrix[pivot_row][j] = multi_t[system->matrix[pivot_row][j]][2]; // to do that we multiply by 2 
				}
			}

			for (row = 0; row < system->rows; row++)
			{
				if (row == pivot_row || system->matrix[row][col] == 0) { continue; }

				int cr_val = system->matrix[row][col];
				int neg_cr_val = negate[cr_val];

				for (j = col; j < system->cols; j++)
				{
					int sub = multi_t[system->matrix[pivot_row][j]][neg_cr_val];
					system->matrix[row][j] = addt_t[system->matrix[row][j]][sub];

				}
			}
			pivot_row++;

		}
	}

}

int rref(SATSystem* system)
{
	int row, col;
	int c_sum = 0;

	for (row = 0; row < system->rows; row++)
	{
		c_sum = 0;

		for (col = 0; col < system->cols - 1; col++)
		{
			if (system->matrix[row][col] != 0) { break; }
			c_sum += 1; // holds number of 0 coefficients in current row
		}
		if (row < system->rows - 1 && c_sum == system->cols - 1 && system->matrix[row][system->cols - 1] == 0)
		{
			free(system->matrix[row]);
			system->matrix[row] = system->matrix[system->rows - 1];
			system->matrix[system->rows - 1] = NULL;
			system->rows--;
			row--;

		}
		else if (row == system->rows - 1 && c_sum == system->cols - 1 && system->matrix[row][system->cols - 1] == 0)
		{
			free(system->matrix[row]);
			system->matrix[row] = NULL;
			system->rows--;

		}
		else if (c_sum == system->cols - 1 && system->matrix[row][system->cols - 1] != 0)
		{
			return 0;
		}
	}
	return 1;
}

SATSystem* reduce_matrix(SATSystem* system)
{
	int row, col;
	int is_pivot = 0;

	system->Atlas = calloc(system->cols, sizeof(int));
	if (!system->Atlas)
	{
		fprintf(stderr, "Error: Memory Allocation for Atlas failed\n");
		free_system_data(system);
		return NULL;
	}

	gaussian_elimination_mod3(system);

	if (!rref(system))
	{
		fprintf(stderr, "System has no solutions 0=1\n");
		free_system_data(system);
		exit(EXIT_SUCCESS);
	}

	for (col = 0; col < system->cols - 1; col++)
	{
		is_pivot = 0;

		for (row = 0; row < system->rows; row++)
		{
			is_pivot += system->matrix[row][col];
		}

		system->Atlas[col] = (is_pivot == 1) ? 1 : 0;
	}

	return system;
}
