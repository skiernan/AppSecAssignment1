#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "dictionary.h"

/*
char* format(char* word){
    //return "test";
    int word_length = strlen(word);
    char formatted[LENGTH+1];
    for (int i = 0; i < word_length; i++)
    {
        // if end of word is punctuation, skip
        if ((i + 1) == word_length) {
            if ((word[i] == ',') || (word[i] == '.') || (word[i] == ';')
                || (word[i] == ':') || (word[i] == '!') || (word[i] == '?'))
                word_length--;
        }
        // if uppercase, convert to lowercase
        else if(isupper(word[i]))
            formatted[i] = tolower(word[i]) ;
        else
            formatted[i] = word[i];
    }
    formatted[word_length] = '\0';
    return formatted;
} // end format
*/

bool load_dictionary(const char* dictionary_file, hashmap_t hashtable[]) {
    int word_count = 0;
    // initialize each value in hash table to NULL
    for(int i = 0; i < HASH_SIZE; i++)
        hashtable[i] = NULL;
    FILE* word_list = fopen(dictionary_file, "r");
    if (word_list == NULL) {
        //printf("Failure loading dictionary");
        return false;
    }
    //char buffer[LENGTH+1];
    //char buffer[LENGTH*2];
    char buffer[LENGTH*20000];
    // while not end of file
    while (fscanf(word_list, "%s", buffer) > 0) {
    //while ((fscanf(word_list, "%s", buffer) > 0) && (word_count < HASH_SIZE)) {
        if (strlen(buffer) <= LENGTH) {
            node* new_node = malloc(sizeof(node));
            new_node->next = NULL;
            strcpy(new_node->word, buffer);
            int bucket = hash_function(new_node->word);
            // if first node in bucket
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
            //if (word_count == HASH_SIZE) {
//            if (word_count == HASH_SIZE * 500) {
  //              fclose(word_list);
                //printf("Maximum %d words in dictionary\n", word_count);
    //            return true;
      //      }
        } // end if
        //else
            //printf("Dictionary word too long: %s\n", buffer);
    } // end while
    //printf("%d words in dictionary\n", word_count);
    fclose(word_list);
    return true;
} // end load_dictionary

bool check_word(const char* word, hashmap_t hashtable[]) {
    int word_length = strlen(word);
    if (word_length > LENGTH) {
        //printf("check_word input overflow: %s\n", word);
        return false;
    }
    //char lowercase[LENGTH+1];
    char lowercase[strlen(word)];
    /*
    //  // convert letters to lowercase
    for (int i = 0; i < word_length; i++)
    {
        // if last character is punctuation, skip it
        if ((i + 1) == word_length) {
            if ((word[i] == ',') || (word[i] == '.') || (word[i] == ';')
                || (word[i] == ':') || (word[i] == '!') || (word[i] == '?'))
                //i++;
                //lowercase[i] = '\0';
                word_length--;
        }
        //if (((i + 1) == word_length) && (word[i] == ('.' || ',' || ';')) )
        //    i++;
        // if uppercase, convert to lowercase
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]);
        else
            lowercase[i] = word[i];
    }
    lowercase[word_length] = '\0';
    */
    int i;
    for (i = 0; i < word_length - 1; i++) {
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]);
        else
            lowercase[i] = word[i];
    }
    // // if last character is punctuation, skip it
    // if last character is punctuation, do not add to word
    if ((word[i] != ',') && (word[i] != '.') && (word[i] != ';')
        && (word[i] != ':') && (word[i] != '!') && (word[i] != '?')) {
        if(isupper(word[i]))
            lowercase[i] = tolower(word[i]);
        //else if ((word[i] != ',') && (word[i] != '.') && (word[i] != ';')
        //    && (word[i] != ':') && (word[i] != '!') && (word[i] != '?'))
        else
            lowercase[i] = word[i];
        i++;
    }
    lowercase[i] = '\0';
    // if (lowercase[word_length - 1] == ('.' || ',' || ';')) {
    //if (!isalpha(lowercase[word_length - 1])) {
    //    printf("punctuation: %s\n", lowercase);
    //    lowercase[word_length - 1] = '\0';
    //}
    //lowercase[word_length] = '\0';
    int bucket = hash_function(lowercase);
    if (bucket >= 0 && bucket < HASH_SIZE) {
        node* current = hashtable[bucket];
        // search until end of linked list
        while (current != NULL) {
            if (strcmp(lowercase, current->word) == 0)
                return true;    // word found
            current = current->next;
        }
    }
    //printf("misspelled word: %s\n", lowercase);
    return false;
} // end check_word

int check_words(FILE* fp, hashmap_t hashtable[], char* misspelled[]) {
    int misspelled_count = 0;
    //char *output[MAX_MISSPELLED];
    //FILE* check = fopen(fp, "r");
    //fopen(fp, "r");
    //if (fp == NULL)
    //    printf("Failure loading input file");
    //char buffer[LENGTH+1];
    //char buffer[LENGTH*2];
    char buffer[LENGTH*20000];
    //while (fscanf(check, "%s", buffer) > 0)
    //while (fscanf(fp, "%s", buffer) > 0)
    //while (fgets(buffer, LENGTH, (FILE*)fp) > 0) {
    //while (fgets(buffer, LENGTH, fp) != NULL) {
    //while (fscanf(fp, "%s", buffer) > 0) {
    while (fscanf(fp, "%s", buffer) > 0 && misspelled_count < MAX_MISSPELLED) {
        //printf("%s\n", buffer);
        //if (buffer[strlen(buffer-1)] == ',' || '.')
        //    buffer[strlen(buffer-1)] = '\0';
        //printf("%s\n", buffer);
        if (strlen(buffer) <= LENGTH) {
        if (!check_word(buffer, hashtable)) {
            //char copy[LENGTH+1];
            char copy[strlen(buffer)];
            //int i = strlen(buffer) - 1;
            //if ((buffer[i] == ',') || (buffer[i] == '.') || (buffer[i] == ';')
            //    || (buffer[i] == ':') || (buffer[i] == '!') || (buffer[i] == '?'))
            //    buffer[i] == '\0';
            //printf("%d misspelled words\n", misspelled_count);
            //printf("%s\n", misspelled[misspelled_count]);
            /*
            int i = 0;
            int j = strlen(buffer) - 1;
            while(i < j) {
                copy[i] = buffer[i];
                i++;
            }
            if ((buffer[i] == ',') || (buffer[i] == '.') || (buffer[i] == ';')
                || (buffer[i] == ':') || (buffer[i] == '!') || (buffer[i] == '?'))
                j--;
            else copy[i] = buffer[i];
            copy[j] == '\0';
            */
            int i;
            for (i = 0; i < strlen(buffer) - 1; i++) {
                if(isupper(buffer[i]))
                    copy[i] = tolower(buffer[i]);
                else
                    copy[i] = buffer[i];
            }
            // if last character is punctuation, do not add to word
            if ((buffer[i] != ',') && (buffer[i] != '.') && (buffer[i] != ';')
                && (buffer[i] != ':') && (buffer[i] != '!') && (buffer[i] != '?')) {
                if(isupper(buffer[i]))
                    copy[i] = tolower(buffer[i]);
                else
                    copy[i] = buffer[i];
                i++;
            }
            copy[i] = '\0';
            //misspelled[misspelled_count] = (char *)malloc((LENGTH+1)*sizeof(char));
            misspelled[misspelled_count] = malloc(strlen(buffer));
            //strcpy(misspelled[misspelled_count], buffer);
            strcpy(misspelled[misspelled_count], copy);
            //printf("%s\n", misspelled[misspelled_count]);
            //strcpy(output[misspelled_count], buffer);
            //printf("%s\n", output[misspelled_count]);
            misspelled_count++;
        } // end if
        } // end if
        //else
            //printf("Input file word too long: %s\n", buffer);
        //if (misspelled_count > 2) {
        //    printf("Maximum %d misspelled words reached\n", misspelled_count);
        //    return misspelled_count;
        //}

    } // end while
    //fclose(check);
    //fclose(fp);
    //printf("%d misspelled words\n", misspelled_count);
    //for (int i = 0; i < misspelled_count; i++)
        //printf("%d %s\n", i, misspelled[i]);
    return misspelled_count;
} // end check_word
