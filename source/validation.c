#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/validation.h"
#include "../headers/general.h"
#include "../headers/firstrun.h"
#include "../headers/commands_protocol.h"

/* functions */

int commandSyntax(char *text)
{
    int i;
    for (i = 0; i < CMD_AMOUNT; i++)
    {
        if (equals(text, commandsList[i].syntax))
            return i;
    }

    return UNKNOWN;
}

int registerSyntax(char *text)
{
    int length = strlen(text);
    char registerAmount = '0' + REGISTER_AMOUNT;

    if (length != 2)
        return UNKNOWN;

    if (text[0] != 'r' || text[1] < '0' || text[1] > registerAmount)
        return UNKNOWN;

    return (int)text[1];
}

int instructionSyntax(char *text)
{
    const char *instructionsList[INSTRUCTION_AMOUNT] = {DATA_SYNTAX, STRING_SYNTAX, ENTRY_SYNTAX, EXTERN_SYNTAX};

    int i;

    for (i = 0; i < INSTRUCTION_AMOUNT; i++)
        if (equals(text, instructionsList[i]))
            return i;

    return UNKNOWN;
}

bool declareValid(char *text, int index)
{
    if (commandSyntax(text) != UNKNOWN || instructionSyntax(text) != UNKNOWN)
        return FALSE;

    if (index != DECLARE_MACRO && registerSyntax(text) != UNKNOWN)
        return FALSE;

    return TRUE;
}

bool defineSyntax(char *text)
{
    return equals(DEFINE_SYNTAX, text) ? TRUE : FALSE;
}

int wordSyntax(char *word)
{
    int temp = instructionSyntax(word);
    if (temp != UNKNOWN)
        return temp;
    if (defineSyntax(word))
        return WORD_DEFINE;
    if (commandSyntax(word) != UNKNOWN)
        return WORD_CMD;
    temp = strlen(word);
    if (temp > LABEL_LENGTH || !isalpha(word[0]) || word[temp - 1] != ':')
        return UNKNOWN;

    word++;
    while (*word != ':')
    {
        if (!isalpha(*word) && !isdigit(*word))
            return UNKNOWN;
        word++;
    }

    return WORD_LABEL;
}

void errorPrint(int error, char *fileName, int line, bool *errorFlag)
{
    if (error != VALID)
    {
        char errorText[ERROR_AMOUNT][ERROR_TEXT_LENGTH] =
            {
                "Illegal declaration",
                "Invalid number",
                "Unknown command",
                "Invalid line syntax",
                "Symbol already declared",
                "Label in illegal place",
                "Missing comma",
                "String contain not printable char",
                "Macro already declared",
                "Extraneous text after end of data",
                "Illegal operand",
                "Unknown label",
                "number is out of range",
                "Unknown operand type",
                "Invalid array index",
            };

        printf("[%s] Error: %s (line %d)\n", fileName, errorText[error], line);

        *errorFlag = TRUE;
    }
}

int operandType(const char *text)
{
    char *word, *pos, textCopy[LABEL_LENGTH];

    strcpy(textCopy, text);
    pos = textCopy;

    if (*pos == '#')
    {
        pos++;
        if (!isNumber(pos))
            return UNKNOWN;

        return ADDRESS_IMM;
    }

    if (registerSyntax(textCopy) != UNKNOWN)
        return ADDRESS_REG;

    modifyText(textCopy, '[');
    modifyText(textCopy, ']');

    word = strtok(textCopy, " \r\t\n");

    if (declareValid(word, DECLARE_LABEL))
    {
        strcpy(textCopy, word);
        pos = textCopy;
        word = strtok(NULL, " \r\t\n");

        if (word == NULL)
        {
            if (!isalpha(*pos))
                return UNKNOWN;

            while (*pos != '\0')
            {
                if (!isalpha(*pos) && !isdigit(*pos))
                    return UNKNOWN;

                pos++;
            }
            return ADDRESS_DIR;
        }

        if (*word == '[')
        {
            word = strtok(NULL, " \r\t\n");
            if (word == NULL)
                return UNKNOWN;
            if (isNumber(word) && atoi(word) >= 0)
            {
                word = strtok(NULL, " \r\t\n");
                if (word == NULL)
                    return UNKNOWN;
                if (*word == ']')
                {
                    word = strtok(NULL, " \r\t\n");
                    if (word == NULL)
                        return ADDRESS_DEF_ID;
                }
            }
        }
    }

    return UNKNOWN;
}