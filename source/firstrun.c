#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include "../headers/general.h"
#include "../headers/node.h"
#include "../headers/validation.h"
#include "../headers/firstrun.h"
#include "../headers/assembler.h"
#include "../headers/commands_protocol.h"

int lineCommand(const char *buffer, bool label, HashTable symbolTable, int *IC, FILE *file)
{
    char bufferCopy[FILE_LINE_LENGTH], bufferCopy2[FILE_LINE_LENGTH];
    char *ptr, *ptr2;
    char word[LABEL_LENGTH], word2[LABEL_LENGTH], labelName[LABEL_LENGTH], operands[2][2][LABEL_LENGTH];
    int operandAddress[2] = {0, 0}, i;
    Command *cmd;

    strcpy(bufferCopy, buffer);
    strcpy(bufferCopy2, buffer);
    modifyText(bufferCopy, ',');
    modifyText(bufferCopy2, ',');
    ptr = bufferCopy;
    ptr2 = bufferCopy2;

    extractWord(word, LABEL_LENGTH, &ptr);
    if (label)
    {
        strcpy(labelName, word);
        labelName[strlen(word) - 1] = '\0';

        if (!declareValid(word, DECLARE_LABEL))
            return ERROR_SAVED_WORD;

        if (nodeTableExist(symbolTable, labelName))
            return ERROR_REGISTERED_SYMBOL;

        symbolInsert(symbolTable, labelName, *IC + COMPILER_START_MEMORY, SYMBOL_CODE);

        extractWord(word, LABEL_LENGTH, &ptr);
    }

    cmd = &commandsList[commandSyntax(word)];

    if (cmd->arguments > 0)
    {
        extractWord(word, LABEL_LENGTH, &ptr);

        if (word == NULL || word[0] == 0)
            return ERROR_INVALID_SYNTAX;
        operandAddress[0] = operandType(word); /* source */
        operandAddress[1] = operandAddress[0]; /* destination */
        if (operandAddress[0] == UNKNOWN)
            return ERROR_UNKNOWN_OPERAND;

        if (cmd->arguments > 1)
        {
            extractWord(word, LABEL_LENGTH, &ptr);
            if (word == NULL || word[0] == 0)
                return ERROR_INVALID_SYNTAX;
            if (*word != ',' || strlen(word) > 1)
                return ERROR_MISSING_COMMA;

            extractWord(word, LABEL_LENGTH, &ptr);
            if (word == NULL || word[0] == 0)
                return ERROR_INVALID_SYNTAX;
            operandAddress[1] = operandType(word);
            if (operandAddress[1] == UNKNOWN)
                return ERROR_UNKNOWN_OPERAND;
        }
        else
            operandAddress[0] = 0;
    }

    if (extractWord(word, LABEL_LENGTH, &ptr))
        return ERROR_EXTRA_TEXT;

    if (operandAddress[0] == UNKNOWN || operandAddress[1] == UNKNOWN)
        return ERROR_INVALID_SYNTAX;

    if ((cmd->arguments != 0 && cmd->argumentAddressType[1][operandAddress[1]] == FALSE) || (cmd->arguments == 2 && cmd->argumentAddressType[0][operandAddress[0]] == FALSE))
        return ERROR_INVALID_OPERAND;
    commandBinaryFirstLine(cmd, file, operandAddress);

    modifyText(bufferCopy2, '[');
    modifyText(bufferCopy2, ']');

    extractWord(word2, LABEL_LENGTH, &ptr2); /* label / command */

    if (label)
        extractWord(word2, LABEL_LENGTH, &ptr2);

    if (cmd->arguments == 0)
    {

        *IC += 1;
        return VALID;
    }
    else if (cmd->arguments == 1)
    {
        extractWord(word2, LABEL_LENGTH, &ptr2);

        strcpy(operands[1][0], word2);

        if (operandAddress[1] == ADDRESS_DEF_ID)
        {
            extractWord(word2, LABEL_LENGTH, &ptr2);
            extractWord(word2, LABEL_LENGTH, &ptr2);
            if (!isNumber(word2) || atoi(word2) < 0 || atoi(word2) > MAX_INT_VALUE)
                return ERROR_INVALID_INDEX;
            strcpy(operands[1][1], word2);
            *IC += 1;
        }
    }
    else
    {
        for (i = 0; i < cmd->arguments; i++, extractWord(word2, LABEL_LENGTH, &ptr2))
        {
            extractWord(word2, LABEL_LENGTH, &ptr2);
            strcpy(operands[i][0], word2);

            if (operandAddress[i] == ADDRESS_DEF_ID)
            {
                extractWord(word2, LABEL_LENGTH, &ptr2);
                extractWord(word2, LABEL_LENGTH, &ptr2);
                if (!isNumber(word2) || atoi(word2) < 0 || atoi(word2) > MAX_INT_VALUE)
                    return ERROR_INVALID_INDEX;
                strcpy(operands[i][1], word2);
                extractWord(word2, LABEL_LENGTH, &ptr2);
                *IC += 1;
            }
        }
        if (operandAddress[0] != ADDRESS_REG || operandAddress[1] != ADDRESS_REG)
            *IC += 1;
    }

    commandBinaryNextLines(cmd, file, operandAddress, operands);

    *IC += 2;
    return VALID;
}

void commandBinaryFirstLine(Command *cmd, FILE *file, int operandAddress[2])
{
    char text[BINARY_WORD_SIZE + 1];
    fputs("0000", file);                   /* not in use */
    parseNumToBinary(text, cmd->index, 4); /* opcode */
    fputs(text, file);
    parseNumToBinary(text, operandAddress[0], 2); /* source operand type */
    fputs(text, file);
    parseNumToBinary(text, operandAddress[1], 2); /* dest operand type */
    fputs(text, file);
    fputs("00\n", file); /* A,R,E */
}

void commandBinaryNextLines(Command *cmd, FILE *file, int address[2], char operands[2][2][LABEL_LENGTH])
{
    int i;
    for (i = 0; i < cmd->arguments; i++)
    {
        if (cmd->arguments == 1)
            i++;
        switch (address[i])
        {
        case ADDRESS_IMM:
            commandBinaryWriteImm(operands[i][0], file);
            break;

        case ADDRESS_DIR:
            commandbinaryWriteDir(operands[i][0], file);
            break;

        case ADDRESS_DEF_ID:
            commandbinaryWriteDir(operands[i][0], file);
            commandBinaryWriteImm(operands[i][1], file);
            break;
        case ADDRESS_REG:
            if (i == 0 || address[0] != ADDRESS_REG)
                commandBinaryRegistersLine(operands[0][0], address[1] == ADDRESS_REG ? operands[1][0] : "0", file);
            break;
        }
    }
}

void commandBinaryWriteImm(char *text, FILE *file)
{
    char binary[BINARY_WORD_SIZE + 1];
    if (*text == '#')
        text++;
    parseNumToBinary(binary, atoi(text), 12);
    fputs(binary, file);
    fputs("00\n", file);
}

void commandbinaryWriteDir(char *text, FILE *file)
{
    fputs(text, file);
    fputs("\n", file);
}

void commandBinaryRegistersLine(char *reg1, char *reg2, FILE *file)
{
    char text[BINARY_WORD_SIZE + 1];
    fputs("000000", file);
    parseNumToBinary(text, (int)reg1[1], 3);
    fputs(text, file);
    parseNumToBinary(text, (int)reg2[1], 3);
    fputs(text, file);
    fputs("00\n", file);
}

int lineEntryExtern(const char *buffer, bool label, int wordType, HashTable symbolTable)
{
    char bufferCopy[FILE_LINE_LENGTH];
    char *word, labelName[LABEL_LENGTH];
    strcpy(bufferCopy, buffer);

    word = strtok(bufferCopy, " \r\t\n"); /* .type / label: */
    if (label)
        word = strtok(NULL, " \r\t\n"); /* .type (ignoring label) */

    word = strtok(NULL, " \r\t\n"); /* label name */

    if (wordType == WORD_INS_ENTRY)
        return VALID;

    if (!declareValid(word, DECLARE_LABEL))
        return ERROR_SAVED_WORD;

    if (nodeTableExist(symbolTable, word))
        return ERROR_REGISTERED_SYMBOL;

    strcpy(labelName, word);

    word = strtok(NULL, " \r\t\n");

    if (word != NULL)
        return ERROR_EXTRA_TEXT;

    symbolInsert(symbolTable, labelName, 0, SYMBOL_EXTERN);

    return VALID;
}

int lineData(const char *buffer, bool label, int wordType, HashTable symbolTable, int *DC, FILE *file)
{
    char bufferCopy[FILE_LINE_LENGTH];
    char *word, labelName[LABEL_LENGTH], text[BINARY_WORD_SIZE + 1];
    int length = 0, i, value;

    strcpy(bufferCopy, buffer);
    if (wordType == WORD_INS_DATA)
        modifyText(bufferCopy, ',');

    word = strtok(bufferCopy, " \r\t\n");

    if (label)
    {
        if (!declareValid(word, DECLARE_LABEL))
            return ERROR_SAVED_WORD;

        strcpy(labelName, word);
        labelName[strlen(word) - 1] = '\0';

        if (nodeTableExist(symbolTable, labelName))
            return ERROR_REGISTERED_SYMBOL;

        word = strtok(NULL, " \r\t\n");
    }

    if (wordType == WORD_INS_STRING)
    {
        word = strtok(NULL, "\r\t\n");
        while (*word == ' ' || *word == '\t')
            word++;

        if (word[0] != '"' || word[strlen(word) - 1] != '"')
            return ERROR_INVALID_SYNTAX;
        word++;

        while (word[length] != '"')
        {
            if (!isprint(word[length]))
                return ERROR_NOT_PRINTABLE_CHAR;

            parseNumToBinary(text, word[length], 14);
            fputs(text, file);
            fputs("\n", file);
            length++;
        }

        fputs("00000000000000\n", file); /* \0 char */
        length++;

        if (word[length] != '\0')
            return ERROR_EXTRA_TEXT;
    }
    else
    {
        word = strtok(NULL, " \r\t\n");

        for (i = 0; (word != NULL); i++, word = strtok(NULL, " \r\t\n"))
        {
            if (i % 2 != 0 && *word != ',')
                return ERROR_MISSING_COMMA;

            if (i % 2 == 0)
            {
                if (isNumber(word))
                {
                    value = atoi(word);
                    if (value < MIN_INT_VALUE || value > MAX_INT_VALUE)
                        return ERROR_OUT_OF_RANGE;

                    parseNumToBinary(text, value, 14);
                    fputs(text, file);
                    fputs("\n", file);
                    length++;
                }
                else
                    return ERROR_NOT_NUMBER;
            }
        }

        if (i % 2 == 0)
            return ERROR_EXTRA_TEXT;
    }

    symbolInsert(symbolTable, labelName, *DC, SYMBOL_DATA);
    *DC += length;

    return VALID;
}

int lineDefine(const char *buffer, bool label, NodeList *defineList, HashTable symbolTable)
{
    char bufferCopy[FILE_LINE_LENGTH];
    char *word, labelName[LABEL_LENGTH];
    int value;
    strcpy(bufferCopy, buffer);

    word = strtok(bufferCopy, " \r\t\n"); /* .define already checked at labelWordType */
    word = strtok(NULL, " \r\t\n");       /* label name */

    if (label)
        return ERROR_LABEL_ILLEGAL_PLACE;

    if (!declareValid(word, DECLARE_DEFINE))
        return ERROR_SAVED_WORD;

    if (nodeTableExist(symbolTable, word))
        return ERROR_REGISTERED_SYMBOL;

    strcpy(labelName, word);

    word = strtok(NULL, " \r\t\n"); /* '=' char */

    if (*word != '=')
        return ERROR_INVALID_SYNTAX;

    word = strtok(NULL, " \r\t\n"); /* number */

    if (!isNumber(word))
        return ERROR_NOT_NUMBER;

    value = atoi(word);

    if (value < MIN_INT_VALUE || value > MAX_INT_VALUE)
        return ERROR_OUT_OF_RANGE;

    nodeListInsert(defineList, labelName, value);
    symbolInsert(symbolTable, labelName, value, SYMBOL_DEFINE);

    return VALID;
}

void replaceDefine(char *text, NodeList defineList)
{
    Node *current = defineList;
    char newText[FILE_LINE_LENGTH];

    while (current != NULL)
    {
        replaceWordWithValue(newText, text, current->word, current->value);
        strcpy(text, newText);
        current = current->next;
    }
}
