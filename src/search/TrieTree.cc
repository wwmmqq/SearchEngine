#include <fstream>
#include "TrieTree.h"
#define Trie_Tree_MAIN
using std::cout;
using std::endl;

int MMM = 0;

TrieTree::TrieTree() : root(new TrieNode()) {
    this->trie_nodes++;
}

TrieTree::~TrieTree()
{
    free_trie_tree(this->root);
    cout << "trie_nodes: " << MMM << endl;
    cout << "MMM: " << MMM <<endl;
}

// Private destroyTree() will delete tree
// Postcondition: Tree is deleted, all memory is deallocated
void TrieTree::free_trie_tree(TrieNode *p)
{
    if(p != nullptr) {
        for(int i = 0; i < 26; i++){
            if (p->children[i])
                free_trie_tree(p->children[i]);
        }
        delete p;
        MMM++;
        p = nullptr;
    }
}


unsigned int TrieTree::count_words() const
{
    return this->trie_words;
}

unsigned int TrieTree::height() const
{
    return this->trie_height;
}

unsigned int TrieTree::count_trie_nodes() const
{
    return this->trie_nodes;
}

unsigned TrieTree::trie_tree_size() const
{
    return this->trie_nodes * sizeof(TrieNode);
}

// If not present, inserts key into trie
// If the key is prefix of trie node, just marks leaf node
void TrieTree::insert(const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    
    if (length > this->trie_height)
        this->trie_height = length;

    TrieNode * p = this->root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
        if (!p->children[index]){
            p->children[index] = new TrieNode();
            this->trie_nodes++;
        }
        p = p->children[index];
    }
 
    // mark last node as leaf
    if (p->isLeaf == false) {
        p->isLeaf = true;
        this->trie_words++;
    }
    p->freq++;
}

// Returns true if key presents in trie, else false
bool TrieTree::search(const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *p = this->root;
 
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
 
        if (!p->children[index])
            return false;
 
        p = p->children[index];
    }
 
    return (p != NULL && p->isLeaf);
}

int TrieTree::search2(const char *key)
{
    int level;
    int length = strlen(key);
    int index;
    struct TrieNode *p = this->root;
    
    for (level = 0; level < length; level++)
    {
        index = CHAR_TO_INDEX(key[level]);
    
        if (!p->children[index])
            return 0;
        p = p->children[index];
    }

    if (p != nullptr && p->isLeaf)
        return p->freq;
    else{
        return 0;
    }
}

void trie_tree_test()
{
    TrieTree tree;

    std::ifstream infile("test.txt");
    if(!infile) {
        std::cout << "File opening failed\n";
    }

    unsigned int raw_words_count = 0;
    std::string tmp;
    while (!infile.eof()) {
        std::getline(infile, tmp, '\n');
        //cout << tmp << endl;
        tree.insert(tmp.c_str());
        raw_words_count++;
        tmp.clear();
    }
    cout << "raw_words_count: " << raw_words_count <<endl; 

    cout << "count_words: " <<tree.count_words() <<endl;
    cout << "height: " <<tree.height() <<endl;
    cout << "count_trie_nodes: " <<tree.count_trie_nodes() <<endl;
    cout << "trie_tree_size: " << tree.trie_tree_size() <<"B"<<endl;
    std::string myin;
    while(std::cin>> myin){
        if(myin == "stop")
            break;
        cout << tree.search2(myin.c_str()) <<endl;
    }
}


#ifdef Trie_Tree_MAIN
int main(int argc, char const *argv[])
{
    trie_tree_test();
    unsigned int sz = 10;
    const int GB = 1024*1024*1024*4, MAX = GB;

    try {
        char* a = new char[MAX];
        *(a+1024*1024)  = 'M';
        cout <<  *(a+1024*1024) << "B"<<endl;
        cout << sizeof(char) << "B"<<endl;

        std::string iii;
        while(std::cin>>iii){
            if(iii == "stop")
                break;
        }
        delete a;
    }
    catch (...) {
        std::cout << "out of memory\n";
    }
    return 0;
}
#endif