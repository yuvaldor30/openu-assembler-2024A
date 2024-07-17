#include <stdio.h>
#include "../headers/general.h"
#include "../headers/macro.h"
#include "../headers/validation.h"
#include "../headers/node.h"

/* functions */

void macroCopyToFile(FILE *file, char *macro)
{
    char buffer[FILE_LINE_LENGTH];
    FILE *macroFile = fopen(macro, "r");

    while (fgets(buffer, sizeof(buffer), macroFile))
        fputs(buffer, file);

    fclose(macroFile);
}

void macroRemoveMacroFiles(HashTable hashTable)
{
    int i;
    Node *node;

    for (i = 0; i < HASH_TABLE_SIZE; i++)
        if (hashTable[i] != NULL)
        {
            node = hashTable[i];

            while (node != NULL)
            {
                remove(node->word);
                node = node->next;
            }
        }
}
