#ifndef UTILS_H
#define UTILS_H

#include "parser.h"

/**
 * @brief Frees memory of system structure as well as its elements
 *
 *
 * Frees memory of system,rows, cols, Atlas and matrix
 *
 * @param system	Pointer to the SATSystem structure containing
 * 					matrix,matrix dimensions and solutions information.
 */
void free_system_data(SATSystem* system);

/**
 * @brief Writes Atlas, dimensions rows x cols and matrix
 * 		  to the specified file
 *
 *
 *
 * @param system		Pointer to the SATSystem structure containing
 * 						matrix,matrix dimensions and solutions information
 *
 * @param output_file  	Path to the output file it will be automatically created
 * 						if doesn't exist yet
 *
 * @return int			Returns 1 if successfull and 0 if there were issues
 *
 */
int write_system_to_file(SATSystem* system, const char* output_file);

#endif // !UTILS_H


