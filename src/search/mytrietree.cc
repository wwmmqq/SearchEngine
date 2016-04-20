#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <fstream>

//#define TEST_MYTRIETREE_
using namespace std;

int NNN = 0;
class Node;
typedef Node* node_ptr;
typedef std::map<char, node_ptr> node_map;
typedef node_map::value_type node_value;

typedef struct document* doc_ptr;

//// index_node infor

struct document {

	unsigned int D_ID;
	unsigned int freq;// tf
	char location = 'a';// a for title, b for content
	unsigned int doc_len;

	document(unsigned int id, unsigned int myfreq, unsigned int len, char location):
		D_ID(id), freq(myfreq), doc_len(len), location(location) {}
	~document(){};
	unsigned int getFreq() { return freq; }
	unsigned int getDoc_len() { return doc_len; }
	unsigned int getD_ID() { return D_ID; }
	char getLocation() { return location; }
};

/////////////////////Node///////////////////////////////
class Node {
private:
	char chr;
	bool isleaf;
	node_map m_children; //use hashmap or map to save children for efficient search
public:
	//std::map<unsigned int, unsigned int> DOCID2;// id and freq
	std::map<unsigned int, doc_ptr> DOCID;

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

	void insertWord(std::string s, unsigned int docid = 0, unsigned int freq=0, char location='b', unsigned int doc_len=0);
	bool deleteWord(const std::string s);
	node_ptr search(const std::string s);
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
	int len_docid = current_node->DOCID.size();
	if( len_docid!= 0) {
		//std::map<unsigned int, doc_ptr>::iterator it;
		for(auto it = current_node->DOCID.begin(); it !=  current_node->DOCID.end(); ++it)
			delete it->second;
	}
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

void Trie::insertWord(std::string s, unsigned int docid, unsigned int freq, char location, unsigned int doc_len){
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

	//current->DOCID.push_back(docid);
	auto search_map = current->DOCID.find(docid);
	if(search_map == current->DOCID.end()) {
		current->DOCID.emplace(docid, new document(docid, freq, doc_len, 'a'));
	} else {
		//TODO : nothing ? is ok
	}


	if(current != my_root && !current->getIslef()) {
		current->trueIsleaf();
		addTrieWords();
	}
}

node_ptr Trie::search(const std::string s) {
	node_ptr current = my_root;

	for (unsigned int i = 0; i < s.length(); i++ ){
		current = current->findChild(s[i]);
		if ( current == NULL )
			return nullptr;
	}

	bool search_result = current->getIslef();
	// std::cout <<"sum of doc: " <<current->DOCID.size() << std::endl;
	// for(int i = 0; i < current->DOCID.size() && i < 10; i++)
	// 	std::cout <<"Top 10 id:  " <<current->DOCID[i] << std::endl;
	if (search_result) {
		return current;
	}
	return nullptr;
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




#ifdef TEST_MYTRIETREE_

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
        tree.insertWord(tmp, 1, 1, 'b',0);
        raw_words_count++;
        tmp.clear();
    }
    cout << raw_words_count << endl;
    cout << NNN <<endl;

    std::string myin;
    while(std::cin>> myin){
        if(myin == "stop")
            break;
        if (tree.search(myin) != nullptr)
        	cout<< "ok" <<endl;
        else
        	cout << "no" <<endl;
    }
}

int main(int argc, char const *argv[])
{
	trie_tree_test();
	return 0;
}
#endif