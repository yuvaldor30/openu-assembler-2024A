#ifndef _VALIDATION_H
#define _VALIDATION_H

#include "general.h"

/* defines */
#define COMMAND_SYNTAX_LENGTH 4 /* Command syntax length */
#define REGISTER_AMOUNT 8       /* Amount of registers in the system*/

/* Instructions syntax */
#define DEFINE_SYNTAX ".define"
#define DATA_SYNTAX ".data"
#define STRING_SYNTAX ".string"
#define ENTRY_SYNTAX ".entry"
#define EXTERN_SYNTAX ".extern"

#define ERROR_TEXT_LENGTH 64 /* Max error output length */
#define LABEL_LENGTH 32      /* Label size */

enum VALIDATION_INDEX /* Validation indices for */
{
    UNKNOWN = -3,
    INVALID,
    VALID,
    FALSE,
    TRUE
};

enum ERROR_INDEX /* Error indices for error output*/
{
    ERROR_SAVED_WORD,
    ERROR_NOT_NUMBER,
    ERROR_UNKNOWN_COMMAND,
    ERROR_INVALID_SYNTAX,
    ERROR_REGISTERED_SYMBOL,
    ERROR_LABEL_ILLEGAL_PLACE,
    ERROR_MISSING_COMMA,
    ERROR_NOT_PRINTABLE_CHAR,
    ERROR_REGISTERED_MACRO,
    ERROR_EXTRA_TEXT,
    ERROR_INVALID_OPERAND,
    ERROR_UNKNOWN_LABEL,
    ERROR_OUT_OF_RANGE,
    ERROR_UNKNOWN_OPERAND,
    ERROR_INVALID_INDEX,
    ERROR_AMOUNT

};

enum COMMAND_INDEX /* Commands indices */
{
    CMD_MOV,
    CMD_CMP,
    CMD_ADD,
    CMD_SUB,
    CMD_NOT,
    CMD_CLR,
    CMD_LEA,
    CMD_INC,
    CMD_DEC,
    CMD_JMP,
    CMD_BNE,
    CMD_RED,
    CMD_PRN,
    CMD_JSR,
    CMD_RTS,
    CMD_HLT,
    CMD_AMOUNT
};

enum ADDRESS_INDEX /* Address indices for operands */
{
    ADDRESS_IMM,
    ADDRESS_DIR,
    ADDRESS_DEF_ID,
    ADDRESS_REG,
    ADDRESS_AMOUNT
};

enum INSTRUCTIONS_INDEX /* Insructions indices */
{
    INSTRUCTION_DATA,
    INSTRUCTION_STRING,
    INSTRUCTION_ENTRY,
    INSTRUCTION_EXTERN,
    INSTRUCTION_AMOUNT
};

enum WORD_TYPE_INDEX /* Word types indices for line type*/
{
    WORD_INS_DATA = INSTRUCTION_DATA,
    WORD_INS_STRING,
    WORD_INS_ENTRY,
    WORD_INS_EXTERN,
    WORD_DEFINE,
    WORD_CMD,
    WORD_LABEL,
    WORD_AMOUNT
};

enum DECLARATION_INDEX /* Declaration indices for input validation */
{
    DECLARE_DEFINE,
    DECLARE_MACRO,
    DECLARE_LABEL,
    DECLARE_AMOUNT
};

/* typedef */
typedef struct Command
{
    char syntax[COMMAND_SYNTAX_LENGTH];
    int index;
    int arguments;
    bool argumentAddressType[2][ADDRESS_AMOUNT];

} Command;

/* function declaration */

/*
 * Description: Checks if a given text matches a known command syntax.
 * Parameters:
 *    - text: The text to check.
 * Returns: The index of the matched command if found, otherwise UNKNOWN.
 */
bool commandSyntax(char *text);

/*
 * Description: Checks if a given text matches a known instruction syntax.
 * Parameters:
 *    - text: The text to check.
 * Returns: The index of the matched instruction if found, otherwise UNKNOWN.
 */
int instructionSyntax(char *text);

/*
 * Description: Checks if a given text matches a known register syntax.
 * Parameters:
 *    - text: The text to check.
 * Returns: The index of the matched register if found, otherwise UNKNOWN.
 */
int registerSyntax(char *text);

/*
 * Description: Checks if a given text matches the define syntax.
 * Parameters:
 *    - text: The text to check.
 * Returns: TRUE if the text matches the define syntax, otherwise FALSE.
 */
bool defineSyntax(char *text);

/*
 * Description: Checks the syntax of a word and determines its type.
 * Parameters:
 *    - word: The word to check.
 * Returns: The type of the word.
 */
int wordSyntax(char *word);

/*
 * Description: Checks if a declaration is valid.
 * Parameters:
 *    - text: The text to check.
 *    - index: The index to specify the type of declaration.
 * Returns: TRUE if the declaration is valid, otherwise FALSE.
 */
bool declareValid(char *text, int index);

/*
 * Description: Prints an error message based on the error code, file name, and line number.
 * Parameters:
 *    - error: The error code.
 *    - fileName: The name of the file where the error occurred.
 *    - line: The line number where the error occurred.
 *    - errorFlag: A flag indicating if an error occurred.
 * Returns: None.
 */
void errorPrint(int error, char *fileName, int line, bool *errorFlag);

/*
 * Description: Determines the type of an operand.
 * Parameters:
 *    - text: The text representing the operand.
 * Returns: The type of the operand.
 */
int operandType(const char *text);

#endif