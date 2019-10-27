#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "dictionary.h"

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    int word_count = 0;
    for(int i = 0; i < HASH_SIZE; i++)
        hashtable[i] = NULL;
    FILE* word_list = fopen(dictionary_file, "r");
    if (word_list == NULL) {        
        return false;
    }
    char buffer[LENGTH*20000];
        while (fscanf(word_list, "%s", buffer) > 0) {
            if (strlen(buffer) <= LENGTH) {
            node* new_node = malloc(sizeof(node));
            new_node->next = NULL;
            strcpy(new_node->word, buffer);
            int bucket = hash_function(new_node->word);
            if (bucket >= 0 && bucket < HASH_SIZE) {
                if (hashtable[bucket] == NULL)
                    hashtable[bucket] = new_node;
                else
                {
                    new_node->next = hashtable[bucket];
                    hashtable[bucket] = new_node;
                }
            }
            word_count++;
    fclose(word_list);
    return true;
} // end load_dictionary

bool check_word(const char* word, hashmap_t hashtable[]) {
    int word_length = strlen(word);
    if (word_length > LENGTH) {        
        return false;
    }
    char lowercase[strlen(word)];
    int i;
    for (i = 0; i < word_length - 1; i++) {
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]);
        else
            lowercase[i] = word[i];
    }
    if ((word[i] != ',') && (word[i] != '.') && (word[i] != ';')
        && (word[i] != ':') && (word[i] != '!') && (word[i] != '?')) {
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]);
        else
            lowercase[i] = word[i];
        i++;
    }
    lowercase[i] = '\0';
    int bucket = hash_function(lowercase);
    if (bucket >= 0 && bucket < HASH_SIZE) {
        node* current = hashtable[bucket];
                while (current != NULL) {
            if (strcmp(lowercase, current->word) == 0)
                return true;
            current = current->next;
        }
    }
    return false;
} // end check_word

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    int misspelled_count = 0;
    char buffer[LENGTH*20000];
        while (fscanf(fp, "%s", buffer) > 0 && misspelled_count < MAX_MISSPELLED) {
        if (strlen(buffer) <= LENGTH) {
        if (!check_word(buffer, hashtable)) {
           char copy[strlen(buffer)];
       
            int i;
            for (i = 0; i < strlen(buffer) - 1; i++) {
                if(isupper(buffer[i]))
                    copy[i] = tolower(buffer[i]);
                else
                    copy[i] = buffer[i];
            }
            if ((buffer[i] != ',') && (buffer[i] != '.') && (buffer[i] != ';')
                && (buffer[i] != ':') && (buffer[i] != '!') && (buffer[i] != '?')) {
                if(isupper(buffer[i]))
                    copy[i] = tolower(buffer[i]);
                else
                    copy[i] = buffer[i];
                i++;
            }
            copy[i] = '\0';
            
            misspelled[misspelled_count] = malloc(strlen(buffer));            
            strcpy(misspelled[misspelled_count], copy);            
            misspelled_count++;
        } // end if
        } // end if
        
    } // end while
    
    return misspelled_count;
} // end check_word
