#ifndef _HASH_NODE_H
#define _HASH_NODE_H

#include "general.h"

/* defines */

#define HASH_TABLE_SIZE 64	/* Hash table size */
#define NODE_WORD_LENGTH 32 /* Node's word length */

enum SYMBOL_TYPES /* Symbol indices for hash table */
{
	SYMBOL_DATA,
	SYMBOL_CODE,
	SYMBOL_ENTRY,
	SYMBOL_EXTERN,
	SYMBOL_DEFINE,
	SYMBOL_MACRO,
	SYMBOL_AMOUNT
};

/* Node structure for hash table */

typedef struct Node
{
	char word[NODE_WORD_LENGTH];
	int value;
	int type;
	struct Node *next;
} Node;

typedef struct Node *HashTable[HASH_TABLE_SIZE]; /* Hash table structure */
typedef struct Node *NodeList;					 /* Node list structure */

/* function declaration of the data structure */

/*
 * Description: Calculates the hash value for a given word.
 * Parameters:
 *    - word: The word for which to calculate the hash value.
 * Returns: The calculated hash value.
 */
int nodeHashWord(char *word);

/*
 * Description: Inserts a macro into the hash table.
 * Parameters:
 *    - hashTable: The hash table to insert the macro into.
 *    - word: The macro name to insert.
 * Returns: An integer representing the status of the operation.
 */
int macroInsert(HashTable hashTable, char *word);

/*
 * Description: Inserts a node into the hash table.
 * Parameters:
 *    - hashTable: The hash table to insert the node into.
 *    - word: The word to insert.
 *    - value: The value associated with the word.
 * Returns: An integer representing the status of the operation.
 */
int nodeInsert(HashTable hashTable, char *word, int value);

/*
 * Description: Inserts a symbol into the hash table.
 * Parameters:
 *    - hashTable: The hash table to insert the symbol into.
 *    - word: The symbol name to insert.
 *    - value: The value associated with the symbol.
 *    - type: The type of the symbol.
 * Returns: An integer representing the status of the operation.
 */
int symbolInsert(HashTable hashTable, char *word, int value, int type);

/*
 * Description: Inserts a node into the node list.
 * Parameters:
 *    - nodeList: The node list to insert the node into.
 *    - word: The word to insert.
 *    - value: The value associated with the word.
 * Returns: An integer representing the status of the operation.
 */
int nodeListInsert(NodeList *nodeList, char *word, int value);

/*
 * Description: Checks if a word exists in the hash table.
 * Parameters:
 *    - hashTable: The hash table to search.
 *    - word: The word to search for.
 * Returns: A boolean value indicating whether the word exists in the hash table.
 */
bool nodeTableExist(HashTable hashTable, char *word);

/*
 * Description: Checks if a word exists in the node list.
 * Parameters:
 *    - nodeList: The node list to search.
 *    - word: The word to search for.
 * Returns: A boolean value indicating whether the word exists in the node list.
 */
bool nodeListExist(NodeList nodeList, char *word);

/*
 * Description: Clears the contents of the hash table.
 * Parameters:
 *    - hashTable: The hash table to clear.
 * Returns: None.
 */
void nodeTableClear(HashTable hashTable);

/*
 * Description: Modifies the value associated with a symbol in the hash table.
 * Parameters:
 *    - hashTable: The hash table containing the symbol.
 *    - word: The symbol name to modify.
 *    - value: The new value to assign to the symbol.
 * Returns: An integer representing the status of the operation.
 */
int symbolModifyValue(HashTable hashTable, char *word, int value);

/*
 * Description: Modifies the type associated with a symbol in the hash table.
 * Parameters:
 *    - hashTable: The hash table containing the symbol.
 *    - word: The symbol name to modify.
 *    - type: The new type to assign to the symbol.
 * Returns: An integer representing the status of the operation.
 */
int symbolModifyType(HashTable hashTable, char *word, int type);

/*
 * Description: Retrieves the value associated with a symbol in the hash table.
 * Parameters:
 *    - hashTable: The hash table containing the symbol.
 *    - word: The symbol name.
 * Returns: The value associated with the symbol.
 */
int symbolValue(HashTable hashTable, char *word);

/*
 * Description: Retrieves the type associated with a symbol in the hash table.
 * Parameters:
 *    - hashTable: The hash table containing the symbol.
 *    - word: The symbol name.
 * Returns: The type associated with the symbol.
 */
int symbolType(HashTable hashTable, char *word);

/*
 * Description: Frees the memory allocated for a linked list of nodes.
 * Parameters:
 *    - nodeList: Pointer to the head of the linked list.
 * Returns: None.
 */
void listFreeMemory(NodeList *nodeList);

/*
 * Description: Frees the memory allocated for a hash table and its associated nodes.
 * Parameters:
 *    - hashTable: The hash table to be freed.
 * Returns: None.
 */
void tableFreeMemory(HashTable hashTable);

#endif
