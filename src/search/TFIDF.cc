#include <iostream>
#include <algorithm>
#include <string>
#include <unordered_set>
#include <vector>
#include <fstream>
#include <sstream>

using std::cout; using std::endl;
using std::vector;using std::string;

std::unordered_set<string> STOPWRODS;
vector<string> Q;

void load_stop_words()
{
	std::ifstream infile("../data/stop_words.txt");
	if(!infile) {  
        std::cout << "File opening failed\n";
    }
	string tmp;
	while (!infile.eof()) {
		std::getline(infile, tmp, '\n');
		STOPWRODS.insert(tmp);
		tmp.clear();
	}
}

bool find_stop_words(std::string goal)
{
	auto search = STOPWRODS.find(goal);
    if(search != STOPWRODS.end()) {
    	return true;
    }
    return false;
}

std::string read_file_text(std::string & filename)
{
	std::ifstream in(filename);
	std::string str((std::istreambuf_iterator<char>(in)),
		            std::istreambuf_iterator<char>());
	return str;
}

void token2()
{
	char input[100] = "A bird came down the walk";
	char *token = std::strtok(input, " ");
	   while (token != NULL) {
	       std::cout << token << '\n';
	       token = std::strtok(NULL, " ");
	   }
}



void queries(vector<string> &Q)
{
	std::ifstream in("../data/query/0601_2011");
	if(!in) {
		std::cout << "query file opening failed\n";
		return;
	}
	string tmp;
	while (!in.eof()) {
		std::getline(in, tmp, '\n');
		Q.push_back(tmp);
		tmp.clear();
	}
	
	//cout << Q.size() <<endl;
}