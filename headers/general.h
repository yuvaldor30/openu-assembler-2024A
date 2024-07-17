#ifndef _GENERAL_H
#define _GENERAL_H

/* defines */
#define equals(x, y) (!strcmp((x), (y)))

#define FILE_LINE_LENGTH 82 /* Line length input file */
#define FILE_NAME_LENGTH 64 /* File name length */

#define BINARY_WORD_SIZE 14                  /* Size of word */
#define BINARY_OPCODE_SIZE 6                 /* Size of opcode in first word */
#define BINARY_A_R_E_SIZE 2                  /* Size of A,R,E in word*/
#define BINARY_REGISTER_SIZE 3               /* Size of register in word */
#define BINARY_IMM_SIZE BINARY_WORD_SIZE - 2 /* Size of immediately value line */
#define BINARY_LABEL_SIZE 12                 /* Size of label in word */

/* Int value range depends on word size */
#define MAX_INT_VALUE ((1 << (BINARY_IMM_SIZE - 1)) - 1)
#define MIN_INT_VALUE (-(1 << (BINARY_IMM_SIZE - 1)))

/* Encrypted syntax of values */
#define ENCRYPTED_CHAR_ZERO '*'
#define ENCRYPTED_CHAR_ONE '#'
#define ENCRYPTED_CHAR_TWO '%'
#define ENCRYPTED_CHAR_THREE '!'

#define ENCRYPED_LINE_LENGTH (BINARY_WORD_SIZE / 2) + 1

enum FILE_EXTENTIONS_INDEX /* File extentions indices */
{
    EXTENTION_AS,
    EXTENTION_AM,
    EXTENTION_OBJECT,
    EXTENTION_ENTRIES,
    EXTENTION_EXTERNALS,
    EXTENTION_RUN1_CODE,
    EXTENTION_RUN1_DATA,
    EXTENTION_RUN2,
    EXTENTION_AMOUNT
};

/* typedef */

typedef int bool;

/* function declaration */

/*
 * Description: Removes the file extension from a string.
 * Parameters:
 *    - text: The string from which to remove the extension.
 * Returns: None.
 */
void extentionRemove(char *text);

/*
 * Description: Checks if a string represents a number.
 * Parameters:
 *    - text: The string to check.
 * Returns: Returns TRUE if the string represents a number, otherwise returns FALSE.
 */
bool isNumber(char *text);

/*
 * Description: Modifies a text string by adding spaces around a specified character.
 * Parameters:
 *    - text: The text string to modify.
 *    - c: The character around which to add spaces.
 * Returns: None.
 */
void modifyText(char *text, char c);

/*
 * Description: Converts a pair of binary digits to an encrypted character.
 * Parameters:
 *    - text: The pair of binary digits to convert.
 * Returns: Returns the encrypted character corresponding to the pair of binary digits.
 */
char encryptedChar(const char *text);

/*
 * Description: Adds a file extension to a string.
 * Parameters:
 *    - target: Pointer to the buffer where the result will be stored.
 *    - text: The string to which to add the extension.
 *    - extentionIndex: The index of the extension to add.
 * Returns: None. The result is stored in the 'target' parameter.
 */
void extentionAdd(char *target, char *text, int extentionIndex);

/*
 * Description: Converts an integer value to a binary string representation.
 * Parameters:
 *    - target: Pointer to the buffer where the result will be stored.
 *    - value: The integer value to convert.
 *    - size: The size of the binary string representation.
 * Returns: None. The result is stored in the 'target' parameter.
 */
void parseNumToBinary(char *target, int value, int size);

/*
 * Description: Replaces occurrences of a word in a sentence with a specified value.
 * Parameters:
 *    - target: Pointer to the buffer where the result will be stored.
 *    - sentence: The sentence in which to replace occurrences.
 *    - word: The word to replace.
 *    - value: The value with which to replace occurrences of the word.
 * Returns: None. The result is stored in the 'target' parameter.
 */
void replaceWordWithValue(char *target, const char *sentence, const char *word, int value);

/*
 * Description: Converts a binary string to an encrypted string.
 * Parameters:
 *    - target: Pointer to the buffer where the result will be stored.
 *    - text: The binary string to convert.
 * Returns: None. The result is stored in the 'target' parameter.
 */
void parseBinaryToEncrypted(char *target, char *text);

/*
 * Description: Extracts the next word from a string.
 * Parameters:
 *    - word: Pointer to the buffer where the extracted word will be stored.
 *    - len: Maximum length of the buffer to store the word.
 *    - ptr: Pointer to a pointer to the input string. This pointer will be updated to point to the next position after the extracted word.
 * Returns: Returns TRUE if a word was successfully extracted, FALSE otherwise.
 */
bool extractWord(char *word, int len, char **ptr);

#endif