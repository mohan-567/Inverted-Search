#include "inverted_index.h"

HashTable hash_table = {NULL}; // Initialize hash table

// Convert string to lowercase
char *to_lowercase(char *str) {
    for (int i = 0; str[i]; i++)
        str[i] = tolower(str[i]);
    return str;
}

// Function to compute hash index based on the first letter
int get_hash_index(char *word) {
    return tolower(word[0]) - 'a';
}

// Validate the file before processing
int is_valid_file(const char *filename) {
    // Check if file has .txt extension
    if (!strstr(filename, ".txt")) {
        printf("Error: Only .txt files are allowed. Skipping %s\n", filename);
        return FAILURE;
    }

    // Check if file exists
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Error: File %s does not exist. Skipping.\n", filename);
        return FAILURE;
    }

    // Check if file is empty
    fseek(fp, 0, SEEK_END);
    if (ftell(fp) == 0) {
        printf("Error: File %s is empty. Skipping.\n", filename);
        fclose(fp);
        return FAILURE;
    }
    fclose(fp);

    // Check if file is already indexed
    for (int i = 0; i < HASH_SIZE; i++) {
        MainNode *main_temp = hash_table.table[i];
        while (main_temp) {
            SubNode *sub_temp = main_temp->sub_link;
            while (sub_temp) {
                if (strcmp(sub_temp->file_name, filename) == 0) {
                    printf("Error: File %s is already indexed. Skipping.\n", filename);
                    return FAILURE;
                }
                sub_temp = sub_temp->next;
            }
            main_temp = main_temp->next;
        }
    }

    return SUCCESS;
}

// Function to create the database from a file
void create_database(char *filename) {
    if (!is_valid_file(filename)) return;

    FILE *fp = fopen(filename, "r");
    char word[WORD_SIZE];

    while (fscanf(fp, "%s", word) != EOF) {
        to_lowercase(word);
        int index = get_hash_index(word);
        MainNode *main_temp = hash_table.table[index];
        MainNode *prev = NULL;

        while (main_temp && strcmp(main_temp->word, word) != 0) {
            prev = main_temp;
            main_temp = main_temp->next;
        }

        if (!main_temp) {
            MainNode *new_main = (MainNode *)malloc(sizeof(MainNode));
            strcpy(new_main->word, word);
            new_main->file_count = 1;
            new_main->sub_link = NULL;
            new_main->next = NULL;

            SubNode *new_sub = (SubNode *)malloc(sizeof(SubNode));
            strcpy(new_sub->file_name, filename);
            new_sub->word_count = 1;
            new_sub->next = NULL;
            new_main->sub_link = new_sub;

            if (prev)
                prev->next = new_main;
            else
                hash_table.table[index] = new_main;
        } else {
            SubNode *sub_temp = main_temp->sub_link;
            while (sub_temp && strcmp(sub_temp->file_name, filename) != 0) {
                sub_temp = sub_temp->next;
            }
            if (sub_temp) {
                sub_temp->word_count++;
            } else {
                SubNode *new_sub = (SubNode *)malloc(sizeof(SubNode));
                strcpy(new_sub->file_name, filename);
                new_sub->word_count = 1;
                new_sub->next = main_temp->sub_link;
                main_temp->sub_link = new_sub;
                main_temp->file_count++;
            }
        }
    }
    fclose(fp);
    printf("Database created successfully from %s\n", filename);
}

// Function to update the database
void update_database() {
    FILE *backup_fp = fopen("backup.txt", "r");
    if (!backup_fp) {
        printf("Error: Backup file does not exist. Please create a database first.\n");
        return;
    }

    char filename[FILE_NAME_SIZE];
    while (fscanf(backup_fp, "%s", filename) != EOF) {
        create_database(filename); // Process each file in the backup
    }

    fclose(backup_fp);
    printf("Database updated successfully from backup.txt\n");
}

// Function to display the database
void display_database() {
    printf("\n===== Inverted Index Database =====\n");
    for (int i = 0; i < HASH_SIZE; i++) {
        MainNode *main_temp = hash_table.table[i];
        while (main_temp) {
            printf("Word: %s appears in %d files\n", main_temp->word, main_temp->file_count);
            SubNode *sub_temp = main_temp->sub_link;
            while (sub_temp) {
                printf("  -> \"%s\", count: %d\n", sub_temp->file_name, sub_temp->word_count);
                sub_temp = sub_temp->next;
            }
            main_temp = main_temp->next;
        }
    }
}

// Function to search for a word in the database (case-insensitive)
void search_database() {
    char search_word[WORD_SIZE];
    printf("Enter the word to search: ");
    scanf("%s", search_word);
    to_lowercase(search_word);

    int index = get_hash_index(search_word);
    MainNode *main_temp = hash_table.table[index];

    while (main_temp) {
        if (strcmp(main_temp->word, search_word) == 0) {
            printf("\nWord \"%s\" is present in %d files.\n", search_word, main_temp->file_count);
            SubNode *sub_temp = main_temp->sub_link;
            while (sub_temp) {
                printf("In \"%s\", count: %d\n", sub_temp->file_name, sub_temp->word_count);
                sub_temp = sub_temp->next;
            }
            return;
        }
        main_temp = main_temp->next;
    }
    printf("Word \"%s\" not found in the database.\n", search_word);
}

// Function to save the database to a file
void save_database() {
    FILE *fp = fopen("backup.txt", "w");
    if (!fp) {
        printf("Error: Cannot open file backup.txt\n");
        return;
    }

    for (int i = 0; i < HASH_SIZE; i++) {
        MainNode *main_temp = hash_table.table[i];
        while (main_temp) {
            fprintf(fp, "%s %d ", main_temp->word, main_temp->file_count);
            SubNode *sub_temp = main_temp->sub_link;
            while (sub_temp) {
                fprintf(fp, "%s %d ", sub_temp->file_name, sub_temp->word_count);
                sub_temp = sub_temp->next;
            }
            fprintf(fp, "\n");
            main_temp = main_temp->next;
        }
    }
    fclose(fp);
    printf("Database successfully saved to backup.txt\n");
}
