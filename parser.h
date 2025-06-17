#ifndef PARSER_H
#define PARSER_H



/**
 * @brief Stores the data extracted from CNF file,
 * 		  including dimension of the matrix, matrix itself,
 *        and Atlas
 *
 * Structure stores matrix, which is representation of
 * 1-in-3-SAT problem, (cnf format) converted to
 * the system of linear equations over Z_3,
 * represented in a form of matrix along with dimensions,
 * and Atlas array used during solving.
 *
 */
typedef struct
{
	int rows; /**< number of clauses */
	int cols; /**< number of variables + solution column*/
	int* Atlas; /**< temporary pivot position tracker, later solutions output*/
	int** matrix; /**< stores system of equations as 2D matrix*/
}SATSystem;

/**
 * @brief Parses cnf file for problem line
 *
 * Extracts number of variables and clauses from CNF  problem line
 * Return 1 if problem line is found successfully, otherwise return 0
 *
 * @param buffer		Pointer to buffer.
 *
 * @param buffer_size	Size of buffer.
 *
 * @param file			Pointer to an already opened CNF file.
 *
 * @param system 		Pointer to the SATSystem structure needed to store information.
 *
 * @return int 			1 if problem line found, 0 otherwise.
 */
int get_problem_line(char* buffer, size_t buffer_size, FILE* file, SATSystem* system);

/**
 * @brief Parses clauses from the CNF file
 *
 * Places variables in their corresponding positions in system->matrix
 *
 * clauses correspond to rows,
 * variables correspond to columns.
 *
 * @param buffer			Pointer to buffer.
 *
 * @param buffer_size		size of buffer.
 *
 * @param file				Pointer to an already opened CNF file.
 *
 * @param system 			Pointer to the SATSystem structure needed to store information.
 *
 * @return int				current_clause is equal to number of parsed clauses.
 */
int parse_clauses(char* buffer, size_t buffer_size, FILE* file, SATSystem* system);

/**
 * @brief Parses cnf file and converts clauses into the matrix to use as system of solutions
 *
 * Opens file following specified path, initializes and allocates memory for
 * system structure as well as its elements, uses get_problem_line
 * and parse_clauses to properly represent cnf file as matrix
 *
 * @param cnf_file 		Path to the file specified for parsing and conversion to matrix
 *
 * @return SATSystem*		Returns Pointer to the system structure or NULL in case of parsing
 * 						or memory allocation failing
 *
 */
SATSystem* parse_cnf_to_matrix(const char* cnf_file);

#endif // !PARSER_H

