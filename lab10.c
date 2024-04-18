#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ALPHABET_SIZE 26

// Trie node structure
struct TrieNode {
    struct TrieNode *children[ALPHABET_SIZE];
    int count;  // Count of words ending at this node
};

// Trie structure
struct Trie {
    struct TrieNode *root;
};

// Creates a new Trie node
struct TrieNode *getNewTrieNode() {
    struct TrieNode *node = (struct TrieNode *)malloc(sizeof(struct TrieNode) * 10);
    if (node) {
        int i;
        node->count = 0;
        for (i = 0; i < ALPHABET_SIZE; i++) {
            node->children[i] = NULL;
        }
    }
    return node;
}

// Initializes a trie structure
struct Trie *createTrie() {
    struct Trie *pTrie = (struct Trie *)malloc(sizeof(struct Trie)* 10);

    if (pTrie) {
        pTrie->root = getNewTrieNode();
    }
    return pTrie;
}

// Inserts the word to the trie structure
void insert(struct Trie *pTrie, char *word) {
    if (pTrie == NULL || pTrie->root == NULL) {
        fprintf(stderr, "Trie structure is not initialized properly.\n");
        return;
    }

    struct TrieNode *crawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (index < 0 || index >= ALPHABET_SIZE) {
            
            return; // This ensures that we only deal with lowercase letters a-z
        }
        if (crawl->children[index] == NULL) {
            crawl->children[index] = getNewTrieNode();
            if (crawl->children[index] == NULL) {
                fprintf(stderr, "Failed to create a new node for %c\n", *word);
                return;
            }
        }
        crawl = crawl->children[index];
        word++;
    }
    crawl->count++;
}

// Computes the number of occurrences of the word
int numberOfOccurances(struct Trie *pTrie, char *word) {
    if (!pTrie || !pTrie->root) return 0;
    struct TrieNode *crawl = pTrie->root;
    while (*word) {
        int index = *word - 'a';
        if (!crawl->children[index]) {
            return 0;
        }
        crawl = crawl->children[index];
        word++;
    }
    return crawl->count;
}

// Helper function to deallocate nodes recursively
void deallocateTrieNodes(struct TrieNode *node) {
    if (!node) return; 
    int i;
    for (i = 0; i < ALPHABET_SIZE; i++) {
        if (node->children[i]) {
            deallocateTrieNodes(node->children[i]);
        }
    }
    free(node);
}

// Deallocates the trie structure
struct Trie *deallocateTrie(struct Trie *pTrie) {
    if (!pTrie) return NULL;
    if (pTrie) {
        deallocateTrieNodes(pTrie->root);
        free(pTrie);
    }
    return NULL;
}

// This function will return number of words in the dictionary,
// and read all the words in the dictionary to the structure words
int readDictionary(char *filename, char **pInWords) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Unable to open file: %s\n", filename);
        return 0;
    }

    char buffer[100];
    int numWords = 0;
    while (fgets(buffer, sizeof(buffer), file)) {
        buffer[strcspn(buffer, "\n")] = 0;  // Strip newline character
        pInWords[numWords] = (char *)malloc(strlen(buffer) + 2);
        strcpy(pInWords[numWords], buffer);
        numWords++;
        if (numWords >= 256) break;  // Prevent overflow of pInWords array
    }

    fclose(file);
    return numWords;
}

int main(void)
{
	char *inWords[256];
	
	//read the number of the words in the dictionary
	int numWords = readDictionary("dictionary.txt", inWords);
	for (int i=0;i<numWords;++i)
	{
		printf("%s\n",inWords[i]);
	}
	
	struct Trie *pTrie = createTrie();
	for (int i=0;i<numWords;i++)
	{
		insert(pTrie, inWords[i]);
	}
	// parse lineby line, and insert each word to the trie data structure
	char *pWords[] = {"notaword", "ucf", "no", "note", "corg"};
	for (int i=0;i<5;i++)
	{
		printf("\t%s : %d\n", pWords[i], numberOfOccurances(pTrie, pWords[i]));
	}
	pTrie = deallocateTrie(pTrie);
	if (pTrie != NULL)
		printf("There is an error in this program\n");
	return 0;
}
