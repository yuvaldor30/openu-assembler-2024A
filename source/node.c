#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/node.h"
#include "../headers/general.h"
#include "../headers/validation.h"

int nodeHashWord(char *word)
{
	int hash = 0, i;

	for (i = 0; word[i] != '\0'; i++)
		hash += tolower(word[i]);

	return hash % HASH_TABLE_SIZE;
}

int macroInsert(HashTable hashTable, char *word)
{
	return symbolInsert(hashTable, word, 0, 0);
}

int nodeInsert(HashTable hashTable, char *word, int value)
{
	return symbolInsert(hashTable, word, value, 0);
}

int symbolInsert(HashTable hashTable, char *word, int value, int type)
{
	int index = nodeHashWord(word);

	Node *newNode = malloc(sizeof(Node));
	Node *current;

	if (!newNode)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
		return INVALID;
	}

	strcpy(newNode->word, word);
	newNode->type = type;
	newNode->value = value;
	newNode->next = NULL;

	if (hashTable[index] == NULL)
		hashTable[index] = newNode;
	else
	{
		current = hashTable[index];

		while (current->next != NULL)
			current = current->next;

		current->next = newNode;
	}

	return VALID;
}

void listFreeMemory(NodeList *nodeList)
{
	Node *temp = *nodeList;
	Node *next;

	if (temp == NULL)
		return;

	next = temp->next;
	free(temp);

	while (next != NULL)
	{
		temp = next;
		next = next->next;
		free(temp);
	}
}

void tableFreeMemory(HashTable hashTable)
{
	int i;
	Node *temp;
	Node *next;

	for (i = 0; i < HASH_TABLE_SIZE; i++)
	{
		if (hashTable[i] == NULL)
			continue;

		temp = hashTable[i];
		next = temp->next;
		free(temp);

		while (next != NULL)
		{
			temp = next;
			next = next->next;
			free(temp);
		}
	}
}

int nodeListInsert(NodeList *nodeList, char *word, int value)
{
	Node *newNode = malloc(sizeof(Node));
	Node *current;

	if (!newNode)
	{
		fprintf(stderr, "Memory allocation failed\n");
		exit(EXIT_FAILURE);
		return INVALID;
	}

	strcpy(newNode->word, word);
	newNode->next = NULL;
	newNode->value = value;
	newNode->type = 0;

	if (*nodeList == NULL)
		*nodeList = newNode;
	else
	{
		current = *nodeList;

		while (current->next != NULL && !equals(current->word, word))
			current = current->next;

		if (equals(current->word, word))
			return INVALID; /* Word already exists */

		current->next = newNode;
	}

	return VALID;
}

bool nodeTableExist(HashTable hashTable, char *word)
{
	int index = nodeHashWord(word);
	Node *node = hashTable[index];

	if (node == NULL)
		return FALSE;

	while (node->next != NULL && !equals(node->word, word))
		node = node->next;

	return equals(node->word, word) ? TRUE : FALSE;
}

bool nodeListExist(NodeList nodeList, char *word)
{
	Node *current = nodeList;

	if (current == NULL)
		return FALSE;

	while (current->next != NULL && !equals(word, current->word))
		current = current->next;

	return equals(word, current->word) ? TRUE : FALSE;
}

void nodeTableClear(HashTable hashTable)
{
	int i;

	for (i = 0; i < HASH_TABLE_SIZE; i++)
		hashTable[i] = NULL;
}

int symbolModifyValue(HashTable hashTable, char *word, int value)
{
	int index = nodeHashWord(word);
	Node *current = hashTable[index];

	if (!nodeTableExist(hashTable, word))
		return INVALID;

	while (current->next != NULL && !equals(current->word, word))
		current = current->next;

	if (equals(current->word, word))
	{
		current->value = value;
		return VALID;
	}

	return INVALID;
}

int symbolModifyType(HashTable hashTable, char *word, int type)
{
	int index = nodeHashWord(word);
	Node *current = hashTable[index];

	if (!nodeTableExist(hashTable, word))
	{
		printf("%s\n", word);
		return INVALID;
	}

	while (current->next != NULL && !equals(current->word, word))
		current = current->next;

	if (equals(current->word, word))
	{
		current->type = type;
		return VALID;
	}

	return INVALID;
}

int symbolValue(HashTable hashTable, char *word)
{
	int index = nodeHashWord(word);
	Node *current = hashTable[index];

	while (current->next != NULL && !equals(current->word, word))
		current = current->next;

	return current->value;
}

int symbolType(HashTable hashTable, char *word)
{
	int index = nodeHashWord(word);
	Node *current = hashTable[index];

	while (current->next != NULL && !equals(current->word, word))
		current = current->next;

	return current->type;
}
