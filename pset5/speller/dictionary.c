/**
 * Implements a dictionary's functionality.
 */

#include "dictionary.h"

// define the node structure
typedef struct node
{
    char word[LENGTH + 1];
    struct node* next;
}
node;

// define a variable to keep track of number of words loaded from dictionary
int word_count = 0;

// define hashtable and nullify all elements
node* hashtable[HASHTABLE_SIZE] = {NULL};

// define hash function
/**
 * Hash function via reddit user delipity:
 * https://www.reddit.com/r/cs50/comments/1x6vc8/pset6_trie_vs_hashtable/cf9nlkn
 */
int hash_it(char* needs_hashing)
{
    unsigned int hash = 0;
    for (int i=0, n=strlen(needs_hashing); i<n; i++)
        hash = (hash << 2) ^ needs_hashing[i];
    return hash % HASHTABLE_SIZE;
}


/**
 * Returns true if word is in dictionary else false.
 */
bool check(const char *word)
{
    // create char array to store word copy
    int length = strlen(word);
    char copy[length + 1];
    
    // convert word to lowercase and store in copy
    for (int i =0; i < length; i++)
        copy[i] = tolower(word[i]);
    
    // add null terminator to signify end of word
    copy[length] = '\0';
    
    // obtain hash index, and return false if index is empty
    int index = hash_it(copy);
    if (hashtable[index] == NULL)
        return false;
    
    // start at first node of index
    node* cursor = hashtable[index];
    
    // check through linked list at the calculated index
    while (cursor != NULL)
    {
        // if word and copy are the same, word is indeed in the dictionary
        if (strcmp(cursor->word, copy) == 0)
            return true;
        // otherwise, move on to the next node
        cursor = cursor->next;
    }
    // if entire linked list is searched and word is not found, return false
    return false;
}

/**
 * Loads dictionary into memory. Returns true if successful else false.
 */
bool load(const char *dictionary)
{
    // open dictionary file
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        printf("Error when opening dictionary file.\n");
        return false;
    }
    
    // create array for word to be stored in
    char word[LENGTH+1];
    
    // run while loop over dictionary file
    while (fscanf(file, "%s", word) != EOF)
    {
        // allocate memory for each new node
        node* new_node = malloc(sizeof(node));
        if (new_node == NULL)
        {
            printf("Unable to malloc space for node.\n");
            return false;
        }
        
        // put word into new node
        strcpy(new_node->word, word);
        word_count++;
        
        // run hash function to determine where to put the word in the hashtable
        int index = hash_it(new_node->word);
        
        // if hashtable is empty, insert
        if (hashtable[index] == NULL)
        {
            hashtable[index] = new_node;
            new_node->next = NULL;
        }
        // if not empty, add to linked-list
        else
        {
            new_node->next = hashtable[index];
            hashtable[index] = new_node;
        }
    }
    
    // close file
    fclose(file);
    
    return true;

}

/**
 * Returns number of words in dictionary if loaded else 0 if not yet loaded.
 */
unsigned int size(void)
{
    if (word_count > 0)
    {
        return word_count;
    }
    else
    {
        return 0;
    }
}

/**
 * Unloads dictionary from memory. Returns true if successful else false.
 */
bool unload(void)
{
    // create a for loop over the size of the hashtable
    for (int i = 0; i < HASHTABLE_SIZE; i++)
    {
        node* cursor = hashtable[i];
        while (cursor != NULL)
        {
            // to prevent loss of linked-list, assign a temp pointer at cursor
            node* temp = cursor;
            cursor = cursor->next;
            free(temp);
        }
    }
    return true;
}
