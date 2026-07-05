#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HashNode {
    char *key;
    char *value;
    struct HashNode *next;
} HashNode;

unsigned int hash_function(const char *key, int table_size) {
    unsigned long hash = 5381;
    int c;
    while ((c = *key++)) {
        hash = ((hash << 5) + hash) + (unsigned char)c;
    }
    return hash % table_size;
}

HashNode** create_hash_table(int size) {
    if (size <= 0) return NULL;
    HashNode** table = calloc(size, sizeof(HashNode*));
    if(table == NULL) return NULL;
    return table;
}

void free_hash_table(HashNode **table, int size) {
    if (table == NULL) return;
    for (int i = 0; i < size; ++i) {
        HashNode *curr = table[i];
        while (curr != NULL){
            HashNode *next_node = curr->next;
            free(curr->key);
            free(curr->value);
            free(curr);
            curr = next_node;
        }
    }
    free(table);
}

int add(HashNode** hash_table, const char* key, const char* value, int table_size) {
    if(hash_table == NULL || key == NULL || value == NULL ) return 0;
    unsigned int index = hash_function(key , table_size);
    HashNode* curr = hash_table[index];
    while(curr != NULL){
        if(strcmp(curr->key, key) == 0){
            free(curr->value);
            curr->value = strdup(value);
            return 1;
        }
        curr = curr->next;
    }
    HashNode *new_node = malloc(sizeof(HashNode));
    if(new_node == NULL) return 0;
    new_node->key = strdup(key);
    new_node->value = strdup(value);
    new_node->next = hash_table[index];
    hash_table[index] = new_node;
    return 1;
}

void search(HashNode** hash_table, const char* key, int table_size){
    if(hash_table == NULL || key == NULL) return;
    unsigned int index = hash_function(key, table_size);
    HashNode* curr = hash_table[index];
    while(curr != NULL){
        if(strcmp(curr->key, key) == 0){
            printf("Found: %s = %s\n", key , curr->value);
            return;
        }
        curr = curr->next;
    }
    printf("Not found\n");
}
int delete_key(HashNode** hash_table, const char* key, int table_size) {
    if (hash_table == NULL || key == NULL) return 0;

    unsigned int index = hash_function(key, table_size);
    HashNode* curr = hash_table[index];
    HashNode* prev = NULL;

    while (curr != NULL) {
        if (strcmp(curr->key, key) == 0) {
            if (prev == NULL) {
                hash_table[index] = curr->next;
            } else {
                prev->next = curr->next;
            }
            free(curr->key);
            free(curr->value);
            free(curr);
            return 1;
        }
        prev = curr;
        curr = curr->next;
    }
    return 0; 
}

int main(){
    HashNode** hash_table = NULL;
    int table_size = 0;
    char command[64];

    printf("=== Hash Table ===\n\n");
    printf("Commands:\n");
    printf(" create - create a new table\n");
    printf(" add - add or update a key-value pair\n");
    printf(" search - find value by key\n");
    printf(" delete - delete entry\n");
    printf(" exit - exit the program\n\n");

    while (1) {
        printf("> ");
        if (scanf("%63s", command) != 1) break;

        if (strcmp(command, "create") == 0) {
            if (hash_table != NULL) {
                free_hash_table(hash_table, table_size);
            }
            printf("Enter table size: ");
            if (scanf("%d", &table_size) != 1 || table_size <= 0) {
                printf("Error: size must be > 0\n");
                table_size = 0;
                hash_table = NULL;
            } else {
                hash_table = create_hash_table(table_size);
                if (hash_table) {
                    printf("Table created successfully (size: %d)\n", table_size);
                }
            }
        }
        else if (strcmp(command, "add") == 0) {
            if (!hash_table) {
                printf("Create a table first!\n");
                continue;
            }
            char key[256], value[256];
            printf("Key: ");
            scanf(" %255s", key);
            printf("Value: ");
            scanf(" %255s", value);
            if (add(hash_table, key, value, table_size)) {
                printf("Added successfully.\n");
            } else {
                printf("Failed to add.\n");
            }
        }
        else if (strcmp(command, "search") == 0) {
            if (!hash_table) {
                printf("Create a table first!\n");
                continue;
            }
            char key[256];
            printf("Key: ");
            scanf(" %255s", key);
            search(hash_table, key, table_size);
        }
        else if (strcmp(command, "delete") == 0) {
            if (!hash_table) {
                printf("Create a table first!\n");
                continue;
            }
            char key[256];
            printf("Key: ");
            scanf(" %255s", key);
            if (delete_key(hash_table, key, table_size)) {
                printf("Deleted successfully.\n");
            } else {
                printf("Key not found.\n");
            }
        }
        else if (strcmp(command, "exit") == 0) {
            if (hash_table != NULL) {
                free_hash_table(hash_table, table_size);
            }
            printf("Goodbye!\n");
            break;
        }
        else {
            printf("Unknown command.\n");
        }
    }
    return 0;
}
