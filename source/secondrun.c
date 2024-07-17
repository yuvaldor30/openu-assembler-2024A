#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/general.h"
#include "../headers/node.h"
#include "../headers/macro.h"
#include "../headers/validation.h"
#include "../headers/assembler.h"
#include "../headers/firstrun.h"
#include "../headers/secondrun.h"

/* functions */

void generateEncryptedFile(char *fileName, int *lines)
{
    char fileRun2[FILE_NAME_LENGTH], fileOB[FILE_NAME_LENGTH];
    FILE *readFile, *writeFile;
    char buffer[FILE_LINE_LENGTH], *word, encryptedWord[ENCRYPED_LINE_LENGTH];

    extentionAdd(fileRun2, fileName, EXTENTION_RUN2);
    extentionAdd(fileOB, fileName, EXTENTION_OBJECT);

    readFile = fopen(fileRun2, "r");
    writeFile = fopen(fileOB, "w");

    fprintf(writeFile, "  %d %d\n", lines[0] - COMPILER_START_MEMORY, lines[1] - lines[0]);
    while (fgets(buffer, sizeof(buffer), readFile))
    {
        word = strtok(buffer, " \r\t\n");
        fprintf(writeFile, "%s ", word);

        word = strtok(NULL, "\t\n");

        parseBinaryToEncrypted(encryptedWord, word);
        fprintf(writeFile, "%s\n", encryptedWord);
    }

    fclose(readFile);
    fclose(writeFile);
}

void assemblerUpdateTable(HashTable symbolTable, int symbol, int value)
{
    int i;
    Node *node;
    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        node = symbolTable[i];
        while (node != NULL)
        {
            if (node->type == symbol)
                node->value += value;
            node = node->next;
        }
    }
}

void generateExtraFile(char *fileName, HashTable symbolTable, int extention, int type)
{

    char fileExt[FILE_NAME_LENGTH];
    int i;
    bool flagCreated = FALSE;
    Node *node;
    FILE *writeFile;

    extentionAdd(fileExt, fileName, extention);
    remove(fileExt);

    for (i = 0; i < HASH_TABLE_SIZE; i++)
    {
        node = symbolTable[i];
        while (node != NULL)
        {
            if (node->type == type)
            {
                if (!flagCreated)
                {
                    writeFile = fopen(fileExt, "w");
                    flagCreated = TRUE;
                }
                fprintf(writeFile, "%s 0%d\n", node->word, node->value);
            }
            node = node->next;
        }
    }
}

int entryFileCheck(FILE *readFile, HashTable symbolTable, char *fileName)
{
    char buffer[FILE_LINE_LENGTH], *text;
    int line = COMPILER_START_MEMORY, empty = FALSE;

    while (fgets(buffer, sizeof(buffer), readFile))
    {
        text = strtok(buffer, " \r\t\n");
        if (wordSyntax(text) == WORD_LABEL)
            text = strtok(NULL, " \r\t\n");
        if (wordSyntax(text) == WORD_INS_ENTRY)
        {
            text = strtok(NULL, " \r\t\n");
            if (symbolModifyType(symbolTable, text, SYMBOL_ENTRY) == INVALID)
            {
                errorPrint(ERROR_UNKNOWN_LABEL, fileName, line, &empty);
                return INVALID;
            }
        }

        line++;
    }

    return VALID;
}

int codeFileComplete(FILE *readFile, FILE *writeFile, HashTable symbolTable)
{
    char buffer[FILE_LINE_LENGTH], *text;
    int line = COMPILER_START_MEMORY;

    while (fgets(buffer, sizeof(buffer), readFile))
    {
        text = strtok(buffer, " \r\t\n");
        fprintf(writeFile, "%04d ", line);
        if (labelBinaryLine(buffer, symbolTable, writeFile) == INVALID)
            fputs(text, writeFile);
        else if (symbolType(symbolTable, text) == SYMBOL_EXTERN)
        {
            if (symbolValue(symbolTable, text) == 0)
                symbolModifyValue(symbolTable, text, line);
            else
                symbolInsert(symbolTable, text, line, SYMBOL_EXTERN);
        }
        fputs("\n", writeFile);

        line++;
    }

    return line;
}

int labelBinaryLine(char *text, HashTable symbolTable, FILE *file)
{
    char binary[BINARY_WORD_SIZE + 1];
    if (nodeTableExist(symbolTable, text))
    {
        parseNumToBinary(binary, symbolType(symbolTable, text) == SYMBOL_EXTERN ? 0 : symbolValue(symbolTable, text), BINARY_LABEL_SIZE);
        fputs(binary, file);
        fputs(symbolType(symbolTable, text) == SYMBOL_EXTERN ? "01" : "10", file);
        return VALID;
    }

    return INVALID;
}

int dataFileCopy(FILE *readFile, FILE *writeFile, int line)
{
    char buffer[FILE_LINE_LENGTH];

    while (fgets(buffer, sizeof(buffer), readFile))
    {
        fprintf(writeFile, "0%d ", line);
        fputs(buffer, writeFile);
        line++;
    }

    return line;
}