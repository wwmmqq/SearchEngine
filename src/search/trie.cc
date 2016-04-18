#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>

#define TEST_MYTRIETREE_
using namespace std;

int NNN = 0;
class Node;
typedef Node* node_ptr;
typedef std::map<char, node_ptr> node_map;
typedef node_map::value_type node_value;


/////////////////////Node///////////////////////////////
class Node {
private:
	char chr;
	bool isleaf;
	node_map m_children; //use hashmap or map to save children for efficient search

public:
	Node() { chr = ' '; isleaf = false; }
	~Node() {}

	char content() { return chr; }
	void setContent(const char c) { chr = c; }
	void trueIsleaf() { isleaf = true; }
	void falseIsleaf(){ isleaf = false; }
	bool getIslef() { return isleaf; }

	node_map children() { return m_children; }

	node_ptr findChild(const char c);
	node_ptr appendChild(char c);
};


node_ptr Node::findChild(const char c) {

	node_map::const_iterator iter = m_children.find(c);
	if(iter != m_children.end()){
		return iter->second;
	}
	return NULL;
}

node_ptr Node::appendChild(const char c) {

	node_ptr child = new Node();
	NNN++;
	child->setContent(c);
	m_children.insert(node_value(c, child)); 
	return child;
}


/////////////////// trie///////////////////////////////////////
class Trie {
private:
	node_ptr my_root;

	unsigned int trie_words = 0;
	unsigned int trie_height = 0;
	unsigned int trie_nodes = 0;

	std::vector<std::string> m_all_words;//TODO: when would you clear this vector?
	std::string m_tmp_string;

	void deleteNode(node_ptr & current_node);
	void saveWord(node_ptr & current_node);//TODO: use a functor here to avoid m_all_words?
	void preOrderTraverse(const node_ptr my_root,void (Trie::*handleNode)(node_ptr & current_node));
	void postOrderTraverse(const node_ptr my_root,void (Trie::*handleNode)(node_ptr & current_node));

public:
	Trie() { my_root = new Node(); };
	~Trie();

	void insertWord(std::string s);
	bool deleteWord(const std::string s);
	bool search(const std::string s);
	void getAllWords();
	void printAllWords();

	unsigned int getWordsCnt() { return trie_words; }
	unsigned int getTrieHeight() { return trie_height; }
	unsigned int getTrieNodeCnt() { return trie_nodes; }

	void addTrieWords() { trie_words++; }
	void addTrieNode() { trie_nodes++; }
	void setTrieHeight(unsigned int height) { trie_height = height;}
	
};

Trie::~Trie(){
	postOrderTraverse(my_root, &Trie::deleteNode);
}

void Trie::deleteNode(node_ptr & current_node){
	//cout<<"deleting ["<<current_node->content()<<"]"<<endl;
	delete current_node;
}

void Trie::saveWord(node_ptr & current_node){
	//only suitable for preorder
	m_tmp_string += current_node->content();

	//cout<<"Constructing word:["<<m_tmp_string<<"]"<<endl;
	if( current_node->getIslef() ){
		//cout<<"Word construction complete :["<<m_tmp_string<<"]"<<endl;
		m_all_words.push_back(m_tmp_string);
		//if get end, clear buffer
		if(current_node->children().empty())
			m_tmp_string.clear();
	}

}

void Trie::preOrderTraverse(node_ptr current_node,void (Trie::*handleNode)(node_ptr & current_node)){
	if(!current_node) return;

	node_map current_children = current_node->children();

	node_map::iterator iter;
	for(iter = current_children.begin();iter!=current_children.end();++iter){
		(this->*handleNode)(iter->second);
		preOrderTraverse(iter->second,handleNode);
	}
}

void Trie::postOrderTraverse(node_ptr current_node,void (Trie::*handleNode)(node_ptr & current_node)){
	if(!current_node) return;

	node_map current_children = current_node->children();

	node_map::iterator iter;
	for(iter = current_children.begin();iter!=current_children.end();++iter){
		postOrderTraverse(iter->second,handleNode);
		(this->*handleNode)(iter->second);
	}
}

void Trie::insertWord(std::string s){
	node_ptr current = my_root;

	for (unsigned int i = 0; i < s.length(); ++i ){   
		node_ptr child = current->findChild(s[i]);
		if ( child == NULL ) {
			current = current->appendChild(s[i]);
			addTrieNode();
		} else {
			current = child;
		}
	}

	if(current != my_root && !current->getIslef()) {
		current->trueIsleaf();
		addTrieWords();
	}
}

bool Trie::search(const std::string s) {
	node_ptr current = my_root;

	for (unsigned int i = 0; i < s.length(); i++ ){
		current = current->findChild(s[i]);
		if ( current == NULL )
			return false;
	}

	bool search_result = current->getIslef();
	return search_result;
}

bool Trie::deleteWord(const std::string s){
	node_ptr current_node = my_root;

	for (unsigned int i = 0; i < s.length(); i++ ){
		current_node = current_node->findChild(s[i]);
		if ( current_node == NULL )return false;
	}

	if(!current_node->getIslef())
		return false;

	//simple delete:change getIslef to false
	current_node->falseIsleaf();
	return true;

	//TODO: delete previous_node father node until meet a getIslef
	//better to have a virtual tree. i.e.Radix tree
}


void Trie::getAllWords(){
	preOrderTraverse(my_root,&Trie::saveWord);
}

void Trie::printAllWords(){
	getAllWords();

	cout<<"All words acquired. Printing:"<<endl;
	for(unsigned int i=0;i<m_all_words.size();++i)
		std::cout<<"["<< m_all_words.at(i) <<"]"<<std::endl;
}


void trie_tree_test()
{
    Trie tree;

    std::ifstream infile("../test.txt");
    if(!infile) {
        std::cout << "File opening failed\n";
    }
    unsigned int raw_words_count = 0;
    std::string tmp;
    while (!infile.eof()) {
        std::getline(infile, tmp, '\n');
        //cout << tmp << endl;
        tree.insertWord(tmp);
        raw_words_count++;
        tmp.clear();
    }
    cout << raw_words_count << endl;
    cout << NNN <<endl;

    std::string myin;
    while(std::cin>> myin){
        if(myin == "stop")
            break;
        cout<< tree.search(myin)<<endl;
    }
}
// Test program
int test2()
{
	//trie_tree_test();
	Trie	myTree3;
	string	words3[30] = { 
				"a", 
				"aa", 
				"aaa", 
				"aaaa", 
				"aaaaa",
				"aaaaaa",
				"aaaaaaa",
				"aaaaaaaa",
				"aaaaaaaaa",
				"aaaaaaaaaa",
				"aaaaaaaaaaa",
				"aaaaaaaaaaaa",
				"aaaaaaaaaaaaa",
				"aaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
				"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaa" };
	int	len3 = 30;

	cout << "Test Set #3  (" << len3 << ")"
			<< endl << endl;

	for (int i=0; i<len3; i++)
		myTree3.insertWord(words3[i]);

	std::string myin;
	while(std::cin>> myin){
	    if(myin == "stop")
	        break;
	    bool s_result = myTree3.search(myin);
	    if (!s_result) 
	    	myTree3.insertWord(myin);
	    cout << myTree3.getWordsCnt()<<endl;
	}
	return 0;
}

#ifdef TEST_MYTRIETREE_

int main(int argc, char const *argv[])
{
	test2();
	return 0;
}
#endif