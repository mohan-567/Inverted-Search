#ifndef INVERTED_INDEX_H
#define INVERTED_INDEX_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define HASH_SIZE 26
#define WORD_SIZE 50
#define FILE_NAME_SIZE 100
#define FAILURE 0
#define SUCCESS 1

// Structure for sub-node (file information)
typedef struct SubNode {
    char file_name[FILE_NAME_SIZE];
    int word_count;
    struct SubNode *next;
} SubNode;

// Structure for main node (word information)
typedef struct MainNode {
    char word[WORD_SIZE];
    int file_count;
    SubNode *sub_link;
    struct MainNode *next;
} MainNode;

// Hash Table Structure
typedef struct HashTable {
    MainNode *table[HASH_SIZE];
} HashTable;

// Global Hash Table
extern HashTable hash_table;

// Function Declarations
int get_hash_index(char *word);
int is_valid_file(const char *filename);
void create_database(char *filename);
void display_database();
void search_database();
void save_database();
void update_database();
char *to_lowercase(char *str);

#endif
