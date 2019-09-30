#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "dictionary.h"
#include "dictionary.c"

#define DICTIONARY "wordlist.txt"
#define INPUT "test_wordlist.txt"
#define OUTPUT "misspelled.txt"

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]);
bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]);
bool check_word(const char* word, hashmap_t hashtable[]);

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    // initialize each value in hash table to NULL
    for(int i = 0; i < HASH_SIZE; i++)
    {
        hashtable[i] = NULL;
    }
    FILE* word_list = fopen(DICTIONARY, "r");
    if (word_list == NULL)
        return false;
    char buffer[LENGTH+1];
    // while not end of file
    while (fscanf(word_list, "%s", buffer) > 0) {
        node* new_node = malloc(sizeof(node));
        new_node->next = NULL;
        strcpy(new_node->word, buffer);
        int bucket = hash_function(new_node->word);
        // if first node in bucket
        if (hashtable[bucket] == NULL)
            hashtable[bucket] = new_node;
        else
        {
            new_node->next = hashtable[bucket];
            hashtable[bucket] = new_node;
        }
    }
    fclose(word_list);
    return true;
} // end load_dictionary

bool check_word(const char* word, hashmap_t hashtable[]) {
    int word_length = strlen(word);
    char lowercase[LENGTH+1];
    // convert letters to lowercase
    for (int i = 0; i < word_length; i++)
    {
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]) ;
        else
            lowercase[i] = word[i];
    }
    lowercase[word_length] = '\0';
    int bucket = hash_function(lowercase);
    node* current = hashtable[bucket];
    // search until end of linked list
    while (current != NULL)
    {
        if (strcmp(lowercase, current->word) == 0)
        // word found
            return true;
        current = current->next;
    }
    return false;
}

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    int count = 0;
    //FILE* check = fopen(fp, "r");
    //fopen(fp, "r");
    char buffer[LENGTH+1];
    //while (fscanf(check, "%s", buffer) > 0)
    //while (fscanf(fp, "%s", buffer) > 0)
    while(fgets(buffer, LENGTH, (FILE*)fp) > 0) {
        if (!check_word(buffer, hashtable)) {
            strcpy(misspelled[count], buffer);
            count++;
        }
    }
    //fclose(check);
    //fclose(fp);
    return count;
}

/*
int main(int argc, char* argv[]){
    hashmap_t hashtable[HASH_SIZE];
    printf("Load dictionary\n");
    load_dictionary(DICTIONARY, hashtable);
    FILE *fp = fopen(INPUT, "r");
    char *misspelled[MAX_MISSPELLED];
    printf("Check words\n");
    int result = check_words(fp, hashtable, misspelled);
    printf("%d misspelled words\n", result);
}
*/
