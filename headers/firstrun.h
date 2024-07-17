#ifndef _FIRST_RUN_H
#define _FIRST_RUN_H

#include "node.h"
#include "general.h"

/* function declaration */

/*
 * Description: Parses a command line from the assembly code, generates the corresponding binary code, and writes it to the output file.
 * Parameters:
 *    - buffer: The command line from the assembly code.
 *    - label: A boolean flag indicating whether the line contains a label.
 *    - symbolTable: The symbol table.
 *    - IC: The instruction counter.
 *    - file: The output file to write the binary code.
 * Returns: Returns VALID if the command line is parsed successfully, or an error code if there is a syntax error or other issue.
 */
int lineCommand(const char *buffer, bool label, HashTable symbolTable, int *IC, FILE *file);

/*
 * Description: Writes the first line of binary code for a command.
 * Parameters:
 *    - cmd: A pointer to the Command structure representing the command.
 *    - file: The output file to write the binary code.
 *    - operandAddress: An array containing the address types of the operands.
 * Returns: None.
 */
void commandBinaryFirstLine(Command *cmd, FILE *file, int operandAddress[2]);

/*
 * Description: Writes the subsequent lines of binary code for a command's operands.
 * Parameters:
 *    - cmd: A pointer to the Command structure representing the command.
 *    - file: The output file to write the binary code.
 *    - address: An array containing the address types of the operands.
 *    - operands: A 2D array containing the operands' labels and indices.
 * Returns: None.
 */
void commandBinaryNextLines(Command *cmd, FILE *file, int address[2], char operands[2][2][LABEL_LENGTH]);

/*
 * Description: Writes the binary code for an immediate operand.
 * Parameters:
 *    - text: The text representation of the immediate value.
 *    - file: The output file to write the binary code.
 * Returns: None.
 */
void commandBinaryWriteImm(char *text, FILE *file);

/*
 * Description: Writes the binary code for a direct operand.
 * Parameters:
 *    - text: The text representation of the direct value.
 *    - file: The output file to write the binary code.
 * Returns: None.
 */
void commandbinaryWriteDir(char *text, FILE *file);

/*
 * Description: Writes the binary code for a line containing register operands.
 * Parameters:
 *    - reg1: The first register operand.
 *    - reg2: The second register operand.
 *    - file: The output file to write the binary code.
 * Returns: None.
 */
void commandBinaryRegistersLine(char *reg1, char *reg2, FILE *file);

/*
 * Description: Parses a line from the assembly code containing an entry or extern directive.
 * Parameters:
 *    - buffer: The line from the assembly code.
 *    - label: A boolean flag indicating whether the line contains a label.
 *    - wordType: The type of directive (entry or extern).
 *    - symbolTable: The symbol table.
 * Returns: Returns VALID if the line is parsed successfully, or an error code if there is a syntax error or other issue.
 */
int lineEntryExtern(const char *buffer, bool label, int wordType, HashTable symbolTable);

/*
 * Description: Parses a line from the assembly code containing data definitions.
 * Parameters:
 *    - buffer: The line from the assembly code.
 *    - label: A boolean flag indicating whether the line contains a label.
 *    - wordType: The type of data (data or string).
 *    - symbolTable: The symbol table.
 *    - DC: The data counter.
 *    - file: The output file to write the binary code.
 * Returns: Returns VALID if the line is parsed successfully, or an error code if there is a syntax error or other issue.
 */
int lineData(const char *buffer, bool label, int wordType, HashTable symbolTable, int *DC, FILE *file);

/*
 * Description: Parses a line from the assembly code containing a .define directive.
 * Parameters:
 *    - buffer: The line from the assembly code.
 *    - label: A boolean flag indicating whether the line contains a label.
 *    - defineList: A pointer to the list of defined symbols.
 *    - symbolTable: The symbol table.
 * Returns: Returns VALID if the line is parsed successfully, or an error code if there is a syntax error or other issue.
 */
int lineDefine(const char *buffer, bool label, NodeList *defineList, HashTable symbolTable);

/*
 * Description: Replaces defined symbols in the text with their corresponding values.
 * Parameters:
 *    - text: The text to process.
 *    - defineList: A pointer to the list of defined symbols.
 * Returns: None.
 */
void replaceDefine(char *text, NodeList defineList);

char *custom_strtok(char *str, const char *delim, char **savePtr);

#endif