#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <unordered_set>

#include "mytrietree.cc"

using std::string; using std::endl; using std::cout;

std::unordered_set<string> STOPWRODS;

void add_stop_words_by_rule() {
	std::ofstream outfile;
	outfile.open("/home/wmq/Desktop/SearchEngine/data/stop_words.txt", 
				std::ofstream::out | std::ofstream::app);

	string temp = "aaa";
	outfile << "temp";
	for (char ch = 'a'; ch <= 'z'; ch++) {
		temp[0] = temp[1] = temp[2] = ch;
		if (ch != 'z')
			outfile << temp <<"\n";
		else
			outfile << temp;
	}
	
	outfile.close();
}

void load_stop_words() {

	std::ifstream infile("/home/wmq/Desktop/SearchEngine/data/stop_words.txt");
	if(!infile) {
        cout << "File opening failed\n";
    }
	std::string tmp;
	while (!infile.eof()) {
		std::getline(infile, tmp, '\n');
		STOPWRODS.insert(tmp);
		tmp.clear();
	}
}

bool find_stop_words(std::string goal) {

	auto search = STOPWRODS.find(goal);
    if(search != STOPWRODS.end()) {
    	return true;
    }
    return false;
}

std::string read_file_text(std::string & filename) {

	std::ifstream in(filename);
	std::string str((std::istreambuf_iterator<char>(in)),
		            std::istreambuf_iterator<char>());
	return str;
}

bool chDir(std::string path) {
	if ( -1 != chdir(path.c_str())){
		return true;
	}
	else {
		cout << "failed to cd " << path << " !"<< endl;
		return false;
	}
}

void text_token_all(const std::string & mystr, Trie& myTree) {

	int token_nu = 0;
    std::istringstream iss(mystr);

    do {
        std::string sub;
        iss >> sub;
        if (sub.size() != 0) {
        	if (!find_stop_words(sub)){
        		token_nu++;
        		//cout<< sub << endl;
        		myTree.addWord(sub);
        	}
        }
    } while (iss);
    cout << "*********** " << token_nu << endl;
    cout << "current word is : " <<myTree.trie_words <<endl;
}

int main()
{
	//add_stop_words_by_rule(); return 0;
	Trie myTree;

	std::string data_path = "/home/wmq/Desktop/SearchEngine/data/processed";

	load_stop_words();

	if (chDir(data_path))
		cout << "current work path is : " << data_path << endl;
	else
		return -1;

	
	for (int text_id = 0; text_id <= 101710; text_id++) {
		std::string filename = std::string("report") + std::to_string(text_id) + std::string(".txt");

		std::string text = read_file_text(filename);
		text_token_all(text, myTree);
	}

	cout << "\n total word is : " <<myTree.trie_words <<endl;
	return 0;
}
