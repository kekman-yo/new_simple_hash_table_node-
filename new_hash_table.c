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
        hash = ((hash << 5) + hash) + (unsigned char)c; // hash * 33 + c
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

            free(table[i]->key);
            free(table[i]->value);
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
        
    curr = curr->next;
}
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
            printf("Find by %s:%s", key , curr->value)
            return;}
        curr = curr->next;
}
    printf("Not found\n");
}

int main(){

}
