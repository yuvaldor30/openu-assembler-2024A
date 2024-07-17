#ifndef _MACRO_H
#define _MACRO_H

#include "node.h"

/* defines */

#define LABEL_MACRO "mcr"        /* Macro command syntax */
#define LABEL_END_MACRO "endmcr" /* End macro command syntax */

/* function declaration */

/*
 * Description: Copies the contents of a macro file to another file.
 * Parameters:
 *    - file: The destination file to which to copy the contents of the macro file.
 *    - macro: The name of the macro file to copy.
 * Returns: None.
 */
void macroCopyToFile(FILE *file, char *macro);

/*
 * Description: Removes all macro files stored in the hash table and clears the hash table.
 * Parameters:
 *    - hashTable: The hash table containing the macro file names.
 * Returns: None.
 */
void macroRemoveMacroFiles(HashTable hashTable);

#endif