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

int main(int argc, char const *argv[])
{
    assemblerFiles(argv, argc);
    return 0;
}

void assemblerFiles(char const *argv[], int argc)
{
    FILE *tempFile;
    char fileAS[FILE_NAME_LENGTH], fileName[FILE_NAME_LENGTH];
    int i;
    for (i = 1; i < argc; i++)
    {
        strcpy(fileName, argv[i]);
        extentionAdd(fileAS, fileName, EXTENTION_AS);
        tempFile = fopen(fileAS, "r");

        if (tempFile)
        {
            generateFiles(argv[i]);
            fclose(tempFile);
        }
        else
            printf("Error: <%s> file does not exist", fileAS);
    }
}

void generateFiles(const char *fileName)
{
    HashTable symbolTable = {NULL};
    char fileNameCopy[FILE_NAME_LENGTH];
    strcpy(fileNameCopy, fileName);

    if (runPreAssembler(fileNameCopy, symbolTable) == VALID)
    {
        printf("[%s] .am (preAssembler) successfully created\n", fileName);
        if (runFirst(fileNameCopy, symbolTable) == VALID)
        {
            if (runFinal(fileNameCopy, symbolTable) == VALID)
                printf("[%s] .ob (object) successfully created\n\n", fileName);
            else
                printf("[%s] assembler stopped, please fix errors.\n\n", fileName);
        }
        else
            printf("[%s] assembler stopped, please fix errors.\n\n", fileName);
    }
    else
        printf("[%s] assembler stopped, please fix errors.\n\n", fileName);
}

int runPreAssembler(char *fileName, HashTable macroTable)
{
    char fileAS[FILE_NAME_LENGTH], fileAM[FILE_NAME_LENGTH];
    FILE *readFile, *writeFile;

    FILE *writeMacroFile; /* Temporary file to write macro */
    FILE *writeTo;        /* write to macro / am file*/

    char buffer[FILE_LINE_LENGTH], bufferCopy[FILE_LINE_LENGTH];
    char *word;

    bool flagError = FALSE;
    int line = 0;

    extentionAdd(fileAS, fileName, EXTENTION_AS);
    extentionAdd(fileAM, fileName, EXTENTION_AM);

    readFile = fopen(fileAS, "r");  /* Source file */
    writeFile = fopen(fileAM, "w"); /* Am File file*/
    writeTo = writeFile;

    if (readFile == NULL)
    {
        printf("Error opening file <%s>\n", fileAS);
        return INVALID;
    }

    while (fgets(buffer, sizeof(buffer), readFile) && ++line)
    {
        strcpy(bufferCopy, buffer);
        word = strtok(bufferCopy, " \r\t\n");
        if (!word)
            continue;

        if (nodeTableExist(macroTable, word)) /* It's a macro label */
            macroCopyToFile(writeFile, word);

        else if (equals(word, LABEL_MACRO)) /* It's a macro declartion */
        {

            word = strtok(NULL, " \r\t\n"); /* name of macro */

            if (word)
            {
                if (!declareValid(word, DECLARE_MACRO))
                    errorPrint(ERROR_SAVED_WORD, fileAS, line, &flagError);

                else if (!nodeTableExist(macroTable, word))
                {
                    writeMacroFile = fopen(word, "w");
                    writeTo = writeMacroFile;
                    macroInsert(macroTable, word);
                }
                else
                    errorPrint(ERROR_REGISTERED_MACRO, fileAS, line, &flagError);
            }
            else
                errorPrint(ERROR_INVALID_SYNTAX, fileAS, line, &flagError);
        }
        else if (equals(word, LABEL_END_MACRO))
        {
            fclose(writeMacroFile);
            writeTo = writeFile;
        }
        else
            fputs(buffer, writeTo);
    }

    fclose(readFile);
    fclose(writeFile);

    macroRemoveMacroFiles(macroTable);

    if (flagError)
        remove(fileAM);

    return flagError ? INVALID : VALID;
}

int runFirst(char *fileName, HashTable symbolTable)
{
    char fileAM[FILE_NAME_LENGTH], fileRun1Code[FILE_NAME_LENGTH], fileRun1Data[FILE_NAME_LENGTH];
    FILE *readFile, *writeCodeFile, *writeDataFile;

    char buffer[FILE_LINE_LENGTH], bufferCopy[FILE_LINE_LENGTH];
    char *word;

    int line = 0, IC = 0, DC = 0, wordType = 0;
    bool flagLabel = FALSE;
    bool flagError = FALSE;

    NodeList defineList = NULL;

    extentionAdd(fileAM, fileName, EXTENTION_AM);
    extentionAdd(fileRun1Code, fileName, EXTENTION_RUN1_CODE);
    extentionAdd(fileRun1Data, fileName, EXTENTION_RUN1_DATA);

    readFile = fopen(fileAM, "r");
    writeCodeFile = fopen(fileRun1Code, "w");
    writeDataFile = fopen(fileRun1Data, "w");

    if (readFile == NULL)
    {
        perror("Error opening file");
        return INVALID;
    }

    while (fgets(buffer, sizeof(buffer), readFile) && ++line)
    {
        flagLabel = FALSE;

        replaceDefine(buffer, defineList);
        strcpy(bufferCopy, buffer);
        word = strtok(bufferCopy, " \r\t\n");

        if (word == NULL || *word == ';')
            continue;
        wordType = wordSyntax(word);
        if (wordType == WORD_LABEL)
        {
            word = strtok(NULL, " \r\t\n");
            flagLabel = TRUE;
            wordType = wordSyntax(word);
        }
        switch (wordType)
        {
        case WORD_DEFINE:
            errorPrint(lineDefine(buffer, flagLabel, &defineList, symbolTable), fileAM, line, &flagError);
            break;
        case WORD_INS_DATA:
            errorPrint(lineData(buffer, flagLabel, wordType, symbolTable, &DC, writeDataFile), fileAM, line, &flagError);
            break;
        case WORD_INS_STRING:
            errorPrint(lineData(buffer, flagLabel, wordType, symbolTable, &DC, writeDataFile), fileAM, line, &flagError);
            break;
        case WORD_INS_ENTRY:
            errorPrint(lineEntryExtern(buffer, flagLabel, wordType, symbolTable), fileAM, line, &flagError);
            break;
        case WORD_INS_EXTERN:
            errorPrint(lineEntryExtern(buffer, flagLabel, wordType, symbolTable), fileAM, line, &flagError);
            break;
        case WORD_CMD:
            errorPrint(lineCommand(buffer, flagLabel, symbolTable, &IC, writeCodeFile), fileAM, line, &flagError);
            break;
        default:
            errorPrint(ERROR_UNKNOWN_COMMAND, fileAM, line, &flagError);
            break;
        }
    }

    assemblerUpdateTable(symbolTable, SYMBOL_DATA, IC + COMPILER_START_MEMORY);
    fclose(readFile);
    fclose(writeCodeFile);
    fclose(writeDataFile);

    listFreeMemory(&defineList);
    if (flagError)
    {
        remove(fileRun1Code);
        remove(fileRun1Data);
    }

    return flagError ? INVALID : VALID;
}

int runFinal(char *fileName, HashTable symbolTable)
{
    char fileRun2[FILE_NAME_LENGTH], fileAM[FILE_NAME_LENGTH], fileRun1Code[FILE_NAME_LENGTH], fileRun1Data[FILE_NAME_LENGTH];
    FILE *writeFile, *readAM, *readCodeFile, *readDataFile;
    int lines[2] = {0, 0};
    extentionAdd(fileRun2, fileName, EXTENTION_RUN2);
    extentionAdd(fileAM, fileName, EXTENTION_AM);
    extentionAdd(fileRun1Code, fileName, EXTENTION_RUN1_CODE);
    extentionAdd(fileRun1Data, fileName, EXTENTION_RUN1_DATA);

    writeFile = fopen(fileRun2, "w");
    readAM = fopen(fileAM, "r");
    readCodeFile = fopen(fileRun1Code, "r");
    readDataFile = fopen(fileRun1Data, "r");
    if (readCodeFile == NULL || readDataFile == NULL)
    {
        perror("Error opening file");
        return INVALID;
    }

    if (entryFileCheck(readAM, symbolTable, fileAM) == INVALID)
    {
        printf("entry check is not valid\n");
        return INVALID;
    }

    fclose(readAM);

    lines[0] = codeFileComplete(readCodeFile, writeFile, symbolTable);
    fclose(readCodeFile);
    remove(fileRun1Code);

    lines[1] = dataFileCopy(readDataFile, writeFile, lines[0]);
    fclose(readDataFile);
    remove(fileRun1Data);

    if (lines[0] + lines[1] > COMPILER_LAST_MEMORY)
    {
        fclose(writeFile);
        remove(fileRun2);
        printf("[%s] Error: Compiler memory exceed from %d lines\n", fileName, COMPILER_LAST_MEMORY);
        return INVALID;
    }

    fclose(writeFile);
    generateEncryptedFile(fileName, lines);
    remove(fileRun2);
    generateExtraFile(fileName, symbolTable, EXTENTION_ENTRIES, SYMBOL_ENTRY);
    generateExtraFile(fileName, symbolTable, EXTENTION_EXTERNALS, SYMBOL_EXTERN);

    tableFreeMemory(symbolTable);

    return VALID;
}
