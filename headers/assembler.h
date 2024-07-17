#ifndef _ASSEMBLER_H
#define _ASSEMBLER_H

/* defines */

#define COMPILER_START_MEMORY 100 /* Size of the reserved memory at the start of the compiler */
#define COMPILER_LAST_MEMORY 4096 /* Size of the reserved memory at all */

/* function declaration */

/*
 * Description: Assembles multiple files specified by their names in the command-line arguments.
 *              Checks if each file exists before starting the assembly process.
 * Parameters:
 *    - argv: An array of strings containing the command-line arguments.
 *    - argc: The number of command-line arguments.
 * Returns: None.
 */
void assemblerFiles(char const *argv[], int argc);

/*
 * Description: Generates intermediate and final files from the input assembly file.
 *              Starts the assembler process for the specified file.
 * Parameters:
 *    - fileName: The name of the input assembly file.
 * Returns: None.
 */
void generateFiles(const char *fileName);

/*
 * Description: Runs the pre-assembler phase for the given file.
 * Parameters:
 *    - fileName: The name of the assembly file to be pre-assembled.
 * Returns: An integer indicating the success (VALID) or failure (INVALID) of the pre-assembler phase.
 */
int runPreAssembler(char *fileName, HashTable macroTable);

/*
 * Description: Runs the first phase of the assembler for the given file.
 * Parameters:
 *    - fileName: The name of the assembly file to be processed.
 *    - symbolTable: The symbol table used for storing symbols and their addresses.
 * Returns: An integer indicating the success (VALID) or failure (INVALID) of the first phase.
 */
int runFirst(char *fileName, HashTable symbolTable);

/*
 * Description: Runs the final phase of the assembler for the given file.
 * Parameters:
 *    - fileName: The name of the assembly file to be processed.
 *    - symbolTable: The symbol table used for storing symbols and their addresses.
 * Returns: An integer indicating the success (VALID) or failure (INVALID) of the final phase.
 */
int runFinal(char *fileName, HashTable symbolTable);

#endif