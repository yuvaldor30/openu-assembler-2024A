#ifndef _SECOND_RUN_H
#define _SECOND_RUN_H


/* function declaration */

/*
 * Description: Copies the contents of a file representing the second run into another file, encrypting certain parts.
 * Parameters:
 *    - fileName: The name of the file to generate.
 *    - lines: An array containing information about the lines.
 * Returns: None.
 */
void generateEncryptedFile(char *fileName, int *lines);

/*
 * Description: Updates the values associated with symbols in the symbol table based on the given symbol type.
 * Parameters:
 *    - symbolTable: The symbol table to update.
 *    - symbol: The symbol type to update.
 *    - value: The value to add to the symbols of the specified type.
 * Returns: None.
 */
void assemblerUpdateTable(HashTable symbolTable, int symbol, int value);

/*
 * Description: Generates an extra file based on the symbol table, containing symbols of a specified type.
 * Parameters:
 *    - fileName: The name of the file to generate.
 *    - symbolTable: The symbol table containing the symbols to include in the file.
 *    - extention: The extension type of the file.
 *    - type: The type of symbols to include in the file.
 * Returns: None.
 */
void generateExtraFile(char *fileName, HashTable symbolTable, int extention, int type);

/*
 * Description: Checks the entry file for entry symbols and updates the symbol table accordingly.
 * Parameters:
 *    - readFile: The entry file to check.
 *    - symbolTable: The symbol table to update.
 *    - fileName: The name of the file being processed.
 * Returns: An integer representing the status of the operation.
 */
int entryFileCheck(FILE *readFile, HashTable symbolTable, char *fileName);

/*
 * Description: Completes the code file by processing labels and writing binary lines.
 * Parameters:
 *    - readFile: The code file to read.
 *    - writeFile: The file to write the completed code to.
 *    - symbolTable: The symbol table containing label information.
 * Returns: The number of lines processed.
 */
int codeFileComplete(FILE *readFile, FILE *writeFile, HashTable symbolTable);

/*
 * Description: Processes a line of the code file containing a label and writes the corresponding binary line.
 * Parameters:
 *    - text: The line of code containing the label.
 *    - symbolTable: The symbol table containing label information.
 *    - file: The file to write the binary line to.
 * Returns: An integer representing the status of the operation.
 */
int labelBinaryLine(char *text, HashTable symbolTable, FILE *file);

/*
 * Description: Copies data lines from one file to another while adding line numbers.
 * Parameters:
 *    - readFile: The file to read from.
 *    - writeFile: The file to write to.
 *    - line: The starting line number.
 * Returns: The updated line number after processing.
 */
int dataFileCopy(FILE *readFile, FILE *writeFile, int line);

#endif