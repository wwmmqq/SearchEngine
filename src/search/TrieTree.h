#ifndef Trie_Tree_H
#define Trie_Tree_H

#define CHAR_TO_INDEX(c) ((int)(c) - (int)'a')
#define TIME(start, end) ((end - start)/CLOCKS_PER_SEC)

#include <iostream>
#include <cstdlib> 

struct TrieNode {
    unsigned int freq;
    struct TrieNode *children[26];
    bool isLeaf;

    TrieNode():freq(0), isLeaf(false){
    	for (int i = 0; i < 26; i++){
    		children[i] = nullptr;
    	}
    }
};

class TrieTree {
private:
    TrieNode* root;
    unsigned int trie_words = 0;
    unsigned int trie_height = 0;
    unsigned int trie_nodes = 0;
public:

    TrieTree();
    ~TrieTree();

    unsigned int count_words() const;
    unsigned int height() const;
    unsigned int count_trie_nodes() const;
    unsigned int trie_tree_size() const;

    void insert(const char *key);
    bool search(const char *key);
    int search2(const char *key);

    void free_trie_tree(TrieNode *p);
};

#endif