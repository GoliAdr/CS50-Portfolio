// Implements a dictionary's functionality (CS50 pset5)

#include <ctype.h>      // tolower()
#include <stdbool.h>    // bool
#include <stdio.h>      // FILE, fopen, fscanf, fclose
#include <stdlib.h>     // malloc, free
#include <string.h>     // strcpy, strcmp
#include <strings.h>    // strcasecmp (case-insensitive compare)

#include "dictionary.h" // LENGTH and prototypes

// ========================
// Data structures
// ========================

typedef struct node
{
    char word[LENGTH + 1];     // one dictionary word (max 45 chars + NUL)
    struct node *next;         // pointer to next node in the bucket (linked list)
}
node;

const unsigned int N = 65536;  // number of buckets in the hash table (power of two) 2^16

static node *table[65536];     // the hash table: array of N bucket heads (all start as NULL)
static unsigned int word_count = 0; // how many words we loaded (for size())

// ========================
// Helpers
// ========================

// A simple, solid hash (case-insensitive djb2-like) → index 0..N-1
unsigned int hash(const char *word)          // map a word to a bucket index
{
    unsigned long h = 5381;                  // start value
    for (const unsigned char *p = (const unsigned char *) word; *p; p++)
    {
        int c = tolower(*p);                 // fold to lowercase (case-insensitive)
        h = ((h << 5) + h) + c;              // h = h*33 + c
    }
    return (unsigned int) (h % N);           // fit into 0..N-1
}

// ========================
// Required functions
// ========================

// Return true if word is in dictionary (case-insensitive)
bool check(const char *word)
{
    unsigned int idx = hash(word);           // bucket index for this word
    for (node *cur = table[idx]; cur != NULL; cur = cur->next) // walk the list
    {
        if (strcasecmp(cur->word, word) == 0) // case-insensitive compare
        {
            return true;                     // found it
        }
    }
    return false;                            // not found
}

// Load dictionary into memory; return true on success
bool load(const char *dictionary)
{
    // open the dictionary file for reading
    FILE *fp = fopen(dictionary, "r");       // text file with one word per line
    if (fp == NULL)                          // failed to open
    {
        return false;
    }

    // initialize table to NULL (defensive; usually static arrays start NULL)
    for (unsigned int i = 0; i < N; i++)
    {
        table[i] = NULL;                     // empty buckets
    }

    char w[LENGTH + 1];                      // buffer to read each word
    while (fscanf(fp, "%45s", w) == 1)       // read word by word (max 45 chars)
    {
        node *n = malloc(sizeof(node));      // allocate a node
        if (n == NULL)                       // out of memory
        {
            fclose(fp);
            return false;
        }

        strcpy(n->word, w);                  // copy the word into the node
        unsigned int idx = hash(n->word);    // find bucket

        // insert at head of list (O(1))
        n->next = table[idx];                // next points to old head
        table[idx] = n;                      // new node becomes the head

        word_count++;                        // track total words
    }

    fclose(fp);                              // close the file
    return true;                             // loaded ok
}

// Return number of words in dictionary (0 if not loaded yet)
unsigned int size(void)
{
    return word_count;                       // how many words load() inserted
}

// Unload dictionary from memory; return true on success
bool unload(void)
{
    // free every list in every bucket
    for (unsigned int i = 0; i < N; i++)
    {
        node *cur = table[i];                // start of this bucket
        while (cur != NULL)                  // walk the list
        {
            node *tmp = cur->next;           // save next
            free(cur);                       // free current node
            cur = tmp;                       // move on
        }
        table[i] = NULL;                     // clear bucket pointer (optional)
    }
    // reset count (optional)
    word_count = 0;
    return true;                             // everything freed
}
