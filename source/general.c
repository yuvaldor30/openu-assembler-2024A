#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/general.h"
#include "../headers/validation.h"
/* functions */

void extentionRemove(char *text)
{
    int i = 0;
    while (text[i] != '\0' && text[i] != '.')
        i++;

    if (text[i] == '.')
        text[i] = '\0';
}

void extentionAdd(char *target, char *text, int extentionIndex)
{
    const char *extentionsNames[EXTENTION_AMOUNT] = {".as", ".am", ".ob", ".ent", ".ext", "_run1_code", "_run1_data", "_run2"};

    char newText[FILE_NAME_LENGTH];
    memset(newText, 0, FILE_NAME_LENGTH);

    strcat(newText, text);
    strcat(newText, extentionsNames[extentionIndex]);
    strcpy(target, newText);
}

bool isNumber(char *text)
{
    if (*text == '+' || *text == '-')
        text++;

    while (*text != '\0')
    {
        if (*text < '0' || *text > '9')
            return FALSE;

        text++;
    }

    return TRUE;
}

void parseNumToBinary(char *target, int value, int size)
{
    int isNegative, i;

    isNegative = (value < 0);

    if (isNegative)
        value *= -1;

    i = size - 1;
    target[size] = '\0';
    while (value > 0)
    {
        target[i--] = (value % 2) + '0';
        value /= 2;
    }

    while (i >= 0)
        target[i--] = '0';

    if (isNegative)
    {
        for (i = 0; i < size; i++)
            target[i] = (target[i] == '0') ? '1' : '0';

        for (i = size - 1; i >= 0; i--)
        {
            if (target[i] == '0')
            {
                target[i] = '1';
                break;
            }
            else
            {
                target[i] = '0';
            }
        }
    }
}

void modifyText(char *text, char c)
{
    char *pos = text;
    size_t len = strlen(text);
    char *resultPos;

    char *result = (char *)malloc(3 * len + 1); /* Allocate enough space for the modified string */

    if (result == NULL)
    {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }

    resultPos = result;

    while (*pos != '\0')
    {
        if (*pos == c && pos[1] != '\0' && pos[1] != ' ') /* Add space before and after the comma */
            resultPos += sprintf(resultPos, "%c ", c);
        else if (*pos != ' ' && pos[1] == c)
        {
            resultPos += sprintf(resultPos, "%c ", *pos); /* Add the current character to the result */
        }
        else
            resultPos += sprintf(resultPos, "%c", *pos); /* Add the current character to the result */

        pos++; /* Move to the next character in the original string */
    }

    *resultPos = '\0';    /* Null-terminate the modified string */
    strcpy(text, result); /* Copy the modified string back to the original string */
    free(result);         /* Free the allocated memory */
}

void replaceWordWithValue(char *target, const char *sentence, const char *word, int value)
{
    const char *ptr = sentence;
    size_t wordLen = strlen(word);
    size_t targetIndex = 0;

    while (*ptr != '\0')
    {
        if (strncmp(ptr, word, wordLen) == 0)
        {
            if ((ptr == sentence || !isalpha(*(ptr - 1))) && !isalpha(*(ptr + wordLen)))
            {
                char valueStr[12];
                sprintf(valueStr, "%d", value);
                strcpy(target + targetIndex, valueStr);
                targetIndex += strlen(valueStr);
                ptr += wordLen;
            }
            else
                target[targetIndex++] = *ptr++;
        }
        else
            target[targetIndex++] = *ptr++;
    }

    target[targetIndex] = '\0';
}

void parseBinaryToEncrypted(char *target, char *text)
{
    int i;
    char newText[ENCRYPED_LINE_LENGTH];
    char *pos = text;
    memset(newText, 0, ENCRYPED_LINE_LENGTH);

    for (i = 0; i < ENCRYPED_LINE_LENGTH - 1; i++)
    {
        newText[i] = encryptedChar(pos);
        pos += 2;
    }
    newText[ENCRYPED_LINE_LENGTH - 1] = '\0';

    strcpy(target, newText);
}

char encryptedChar(const char *text)
{
    if (*text == '0')
        if (text[1] == '0')
            return ENCRYPTED_CHAR_ZERO;
        else
            return ENCRYPTED_CHAR_ONE;
    else if (text[1] == '0')
        return ENCRYPTED_CHAR_TWO;
    else
        return ENCRYPTED_CHAR_THREE;
}

bool extractWord(char *word, int len, char **ptr)
{
    char *text = *ptr;

    int textLen = strlen(text);
    bool foundStart = FALSE;
    int i;

    memset(word, 0, len);

    if (!text[0])
        return FALSE;

    i = 0;

    while (i < textLen)
    {
        if (!isspace(text[i]))
        {
            *ptr += i; 
            text = *ptr;
            foundStart = TRUE;
            break;
        }
        i++;
    }

    if (!foundStart)
        return FALSE;

    textLen = strlen(text);

    i = 0;

    while (i < textLen)
    {
        if (isspace(text[i]))
        {
            memcpy(word, text, i);
            *ptr += i;           
            return TRUE;
        }

        i++;
    }

    memcpy(word, text, textLen);

    *ptr += textLen;

    return TRUE;
}