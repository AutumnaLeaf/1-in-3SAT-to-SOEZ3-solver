#ifndef ELIM_H
#define ELIM_H

#include "parser.h"

extern const unsigned char multi_t[3][3]; // multiplication table over Z_3
extern const unsigned char addt_t[3][3]; // addition table over Z_3
extern const unsigned char negate[3]; // negation table over Z_3

/**
 * @brief Performs Gaussian elimination modulo 3 on the matrix representing
 * 		  system of equations
 *
 * Function Transforms matrix into row-echelon form over Z_3
 * Iterates through columns to:
 * -Find non-zero  value that we take as pivot
 * -After finding swaps row positions if it took more than one iteration
 * -Normalize such that pivot becomes 1
 * -Eliminate all other non-zero elements in the same column by:
 *	-Taking non zero value to eliminate from pivot column
 *  -Negating it
 *  -Multiplying pivot row by it
 *  -Adding this multiplied pivot to the current row (mod 3)
 *
 * It uses modular arithmetic (mod 3) which is performed using lookup tables
 *
 * @param system Pointer to the SATSystem structure containing
 * 				 matrix,matrix dimensions and solutions information.
 *
 */
void gaussian_elimination_mod3(SATSystem* system);

/**
 * @brief Checks matrix for zero rows or contradiction reducing it to RREF
 *
 * Iterates through all rows to identify and remove zero rows.
 * Detects contradictions where all coefficients in row are equal to zero
 * but the corresponding solution value is non-zero, therefore
 * no solution exists.
 *
 * @param system system Pointer to the SATSystem structure containing
 * 				 matrix,matrix dimensions and solutions information.
 *
 * @return int   Returns 0 in case of contradiction and 1 otherwise
 *
 */
int rref(SATSystem* system);

/**
 * @brief Reduces matrix using gausian_elimination_mod3 followed by rref
 * 		  Populates system->Atlas with positions of pivots
 *
 * Transforms matrix into reduced row echelon form over Z_3.
 * Populates system->Atlas with pivot positions.
 *
 * Atlas array has length of system->cols, pivot position is marked
 * as 1 non pivot as 0
 *
 * @param system 		system Pointer to the SATSystem structure containing
 * 				 		matrix,matrix dimensions and solutions information.
 *
 * @return SATSystem*	Returns pointer to the system structure
 *
 */
SATSystem* reduce_matrix(SATSystem* system);
#endif // !ELIM_H

