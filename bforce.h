#ifndef BFORCE_H
#define BFORCE_H

#include "parser.h"
/**
 * @brief Validates solution, computes values for pivot variables
 * 		  and stores them in Atlas array.
 *
 * Function computes values for all pivot variables and stores them
 * in the Atlas array, which is reused as solution vector.
 *
 * Any calculated pivot variable equal to 2 is considered invalid,
 * which stems from the fact that x_n in {0,1} (either true or false).
 *
 * @param system Pointer to the SATSystem structure, includes:
 * 				 - matrix as representation of the system of equations,
 * 				 - its dimensions (rows, cols),
 * 				 - the Atlas array, which stores :
 * 					-Initially: pivot placement,
 * 					-Finally: the solution vector.
 *
 * @return int   Returns 0 if any RHS value is equal to 2
 * 				 otherwise returns the rank of matrix.
 *
 */
int solve_for_id(SATSystem* system);

/**
* @brief Moves non-pivot values to the LHS and constant to RHS,
* 		  substitutes 2 for pivot variables, calculates the rank of the matrix.
*
* This function moves non-pivot values to the LHS by negating them which in mod 3
* arithmetic is equivalent to subtraction. This is only done theoretically; the same
* operation is applied to the constant value. For later solving, it substitutes 2
* for pivot variables and adds constant value to them.
*
*
* @param system Pointer to the SATSystem structure, includes:
* 				 - matrix as representation of the system of equations,
* 				 - its dimensions (rows, cols),
* 				 - the Atlas array, which stores :
* 					-Initially: pivot placement,
* 					-Finally: the solution vector.
*
* @return int 	 Returns rank of matrix;
*
*
*/
int rearrange_rhs_lhs(SATSystem* system);

/**
 * @brief Iterates through possible(with practical limitations) solution vectors,
 *  	  substituting values, computes solutions, and validating each of solution candidate
 *
 *
 * This function calculates possibilities equal to 2^(cols - rows),
 * this is done by left shift operators (<<).
 *
 * There was no point in creating Big int variable type for (cols - rows) > 63
 * since computation time becomes impractical starting around (cols - rows) = 35.
 *
 * For each possibility solution vector is being generated,from binary representation
 * of current sample number,using ternary operator and bitwise operation, stored bit by bit in Atlas array.
 *
 * Calculates the value of each row  by multiplying coefficients with corresponding variable values
 * and summing them. If any row's solution equals 2, vector is considered invalid because
 * variables are binary (0 or 1).
 *
 *
 *
 * @param system			Pointer to the SATSystem structure, includes:
 * 				 			- matrix as representation of the system of equations,
 * 				 			- its dimensions (rows, cols),
 * 				 			- the Atlas array, which stores :
 * 								-Initially: pivot placement,
 * 								-Finally: the solution vector.
 *
 * @param variable_index	Array holding indices of non-pivot variables in	matrix.
 * 							Length is (cols - rows). ex. variable_index[0]= first non-pivot variable index.
 *
 * @param pivot_holder		Array holding indices of pivot variables in	matrix.
 * 							Length is rows(rank). ex. pivot_holder[0] = first pivot variable index.
 *
 *
 * @return int 				Returns 1 if a valid solution vector is found, 0 otherwise
 *
 */
int bruteforce(SATSystem* system, int* variable_index, int* pivot_holder);

/**
 * @brief Calculates pivots variable by substituting solution vector for non-pivot variables
 * 		  and adding constant.
 *
 * The function first reverses operation performed on solution column by adding 1 and negating it.
 * Then it calculates each variable solution by multiplying coefficients by corresponding non-pivot values from solution vector
 * and summing them, adding constant and the end. Finally, it stores the results in the correct
 * pivot positions within Atlas array, completing full solution vector.
 *
 *
 *@param system			Pointer to the SATSystem structure, includes:
 * 				 			- matrix as representation of the system of equations,
 * 				 			- its dimensions (rows, cols),
 * 				 			- the Atlas array, which stores :
 * 								-Initially: pivot placement,
 * 								-Finally: the solution vector.
 *
 * @param variable_index	Array holding indices of non-pivot variables in	matrix.
 * 							Length is (cols - rows). ex. variable_index[0]= first non-pivot variable index.
 *
 * @param pivot_holder		Array holding indices of pivot variables in	matrix.
 * 							Length is rows(rank). ex. pivot_holder[0] = first pivot variable index.
 *
 *
 *
 */
void calculate_pivots(SATSystem* system, int* variable_index, int* pivot_holder);

/**
 * @brief "Driver function" that manages solving process.
 * 		It performs operations on matrix by calling other functions,
 * 		validates data, defines and allocates memory for arrays.
 *
 *
 *@param system			Pointer to the SATSystem structure, includes:
 * 				 			- matrix as representation of the system of equations,
 * 				 			- its dimensions (rows, cols),
 * 				 			- the Atlas array, which stores :
 * 								-Initially: pivot placement,
 * 								-Finally: the solution vector.
 *
 * @return Pointer			Returns pointer to SATSystem structure where solution is located.
 *
 */
SATSystem* find_solution(SATSystem* system);
#endif // BFORCE_H

