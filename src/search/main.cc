#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <map>
#include <queue>
#include <algorithm>
#include <ctime>

#include "base_function.cc"
#include "mytrietree.cc"
#include "BM25.cc"

using std::vector;
using std::string; using std::endl; using std::cout;

std::unordered_set<string> STOPWRODS;
vector<vector<string> > QQ;
//std::unordered_set<unsigned int> SearchResultSet;
//std::unordered_set<string> Dict;
unsigned int token_nu = 0;

struct IdScore {
	unsigned int id;
	double score;

	IdScore(unsigned int id, double score):id(id),score(score){}

	bool operator < (const IdScore& a) const {
		return a.score < score;
	}
};


void outPutSearchResult(const string &filename, const std::vector<IdScore> &dst, const int pi) {
	std::ofstream outfile;
	outfile.open(filename, std::ofstream::out | std::ofstream::app);

	int cnt = 0;
	for (auto it = dst.begin(); it != dst.end(); ++it) {
		if(cnt >= 1000)
			break;
		
		string line =  std::to_string(pi) + " " + std::to_string(it->score) + " ";
		line += string("report") + std::to_string(it->id);
		outfile << line << "\n";
		cnt++;
	}
	outfile.close();
}

void load_stop_words() {

	std::ifstream infile("/home/wmq/Desktop/SearchEngine/data/stop_words.txt");
	if(!infile) {
        cout << "File opening failed\n";
    }
	string tmp;
	while (!infile.eof()) {
		std::getline(infile, tmp, '\n');
		STOPWRODS.emplace(tmp);
		tmp.clear();
	}
}

bool find_stop_words(const string &goal) {

	auto search = STOPWRODS.find(goal);
    if(search != STOPWRODS.end()) {
    	return true;
    }
    return false;
}

std::string read_file_text(const string & filename) {

	std::ifstream in(filename);
	std::string str((std::istreambuf_iterator<char>(in)),
		            std::istreambuf_iterator<char>());
	return str;
}

bool chDir(string path) {
	if ( -1 != chdir(path.c_str())){
		return true;
	}
	else {
		cout << "failed to cd " << path << " !"<< endl;
		return false;
	}
}

void documentToken(const unsigned int docid, const std::string & mystr, Trie& myTree) {

	unsigned int text_token_len = 0;
    std::istringstream iss(mystr);
    std::unordered_map<std::string, unsigned int> Document;

    do {
        std::string sub;
        iss >> sub;
        if (sub.size() != 0) {
    		token_nu++;
    		text_token_len++;
    		auto search_map = Document.find(sub);
    		if(search_map != Document.end()) {
    			search_map->second++;
    		} else {
    			Document.emplace(sub, 1);
    		}
        }
    } while (iss);

    for (const auto &p : Document) {
    	//std::cout << p.first << " : " << p.second << "\n";
    	myTree.insertWord(p.first, docid, p.second, 'b', text_token_len);
    }

    //cout << "*********** " << token_nu << endl;
    //cout << "current word is : " <<myTree.getWordsCnt() <<endl;
}

void queryTokenAll(string & filename)
{
	vector<string> tempv;
	std::ifstream in(filename);
	string line;
	string sub;

	while(std::getline(in, line)) {
		tempv.clear();
		std::istringstream iss(line);
		do {
	        iss >> sub;
	        if (sub.size() != 0) {
	        	//cout<< sub <<endl;
	        	tempv.push_back(sub);
	        }
    	} while (iss);
    	QQ.push_back(tempv);
	}
}


int test()
{
	string query_path = "/home/wmq/Desktop/SearchEngine/data/query/washedquery.txt";
	queryTokenAll(query_path);
	cout << "QQ size: " << QQ.size() << endl;

	for(int qi = 0; qi < QQ.size(); qi++)
	{
		std::string myin;
		for(int qii = 0; qii < QQ[qi].size(); qii++) {
			myin = QQ[qi][qii];
			cout << myin <<"**";
		}
		cout << endl;
	}
	return 0;
}

int main()
{
	std::clock_t c_start1 = std::clock();

	Trie myTree;
	double avgdl = 0.0;

	std::string data_path = "/home/wmq/Desktop/SearchEngine/data/processed";
	string query_path = "/home/wmq/Desktop/SearchEngine/data/query/washedquery.txt";
	string result_file = "/home/wmq/Desktop/SearchEngine/data/query/QQ_1000.txt";

	if (chDir(data_path))
		cout << "current work path is : " << data_path << endl;
	else
		return -1;

	std::map<unsigned int, double> DocOutMap;

	for (unsigned int text_id = 0; text_id <= 101710; text_id++) {

		std::string filename = std::string("report") + std::to_string(text_id) + std::string(".xml");

		//cout << filename <<"\n";
		std::string text = read_file_text(filename);
		if(text.size() != 0) {
			documentToken(text_id, text, myTree);
		}
	}

	std::clock_t c_end1 = std::clock();
	cout<<"CPU time used(索引建立时间): "
            << (c_end1-c_start1) / CLOCKS_PER_SEC << " s\n";//40 s

	cout << "\nTrie total word is : " <<myTree.getWordsCnt() <<"\n";//40351

	//////trie tree build end ////////
	
	std::vector<node_ptr> result_ptr_v;

	queryTokenAll(query_path);
	cout << "QQ size: " << QQ.size() << endl;

	double toatal_query_time = 0.0;
	for(int qi = 0; qi < QQ.size(); qi++)
	{	
		std::vector<IdScore> result_Q;
		cout << "*****************" << qi+1 <<"*****************" <<"\n";
		DocOutMap.clear();
		result_ptr_v.clear();
		std::string myin;

		std::clock_t c_start2 = std::clock();
		for(int qii = 0; qii < QQ[qi].size(); qii++) {
			myin = QQ[qi][qii];
		    node_ptr search_p = myTree.search(myin);

		    if (search_p != nullptr) {
		    	result_ptr_v.push_back(search_p);
		    	//cout<< "ok" <<endl;
		    	std::cout <<"doc about "<<myin<<" : "<< search_p->DOCID.size() << std::endl;
		    } else {
		    	cout << "no searched: " << myin <<"\n";
		    	continue;
		    }
		}
		
		for (int i = 0; i != result_ptr_v.size(); i++) {
			if(result_ptr_v[i] == nullptr)
				continue;

			unsigned int tf = 0;
			unsigned int n_qi = result_ptr_v[i]->DOCID.size();

			for(auto it = result_ptr_v[i]->DOCID.begin(); it !=  result_ptr_v[i]->DOCID.end(); ++it) {
				avgdl += it->second->getDoc_len();
			}
			avgdl = avgdl / n_qi;

			for(auto it = result_ptr_v[i]->DOCID.begin(); it != result_ptr_v[i]->DOCID.end(); ++it) {
				tf = it->second->getFreq();
				unsigned int dl = it->second->getDoc_len();
				double di_score = BM25OneTokenScore(tf, n_qi , dl, avgdl);

				unsigned int doc_id = it->first;
				auto find_di = DocOutMap.find(doc_id);
	    		if(find_di != DocOutMap.end()) {
	    			find_di->second += di_score;
	    		} else {
	    			DocOutMap.emplace(doc_id, di_score);
	    		}
			}

		}//end first for
		

		for (auto it = DocOutMap.begin(); it!=DocOutMap.end(); ++it) {
			//dst.emplace(it->second, it->first);
			result_Q.push_back(IdScore(it->first, it->second));
		}
		std::sort(result_Q.begin(), result_Q.end());

		std::clock_t c_end2 = std::clock();
		double cost_time2 = c_end2-c_start2;
		toatal_query_time += cost_time2;

		outPutSearchResult(result_file, result_Q, qi+101);
		cout<<"CPU time used:(检索时间): "
	            << cost_time2/CLOCKS_PER_SEC << " clock\n";
	}

	cout<<"CPU time used:(35 query 检索时间): "
	            << toatal_query_time/CLOCKS_PER_SEC << " s\n";
	cout<<"CPU time used:(平均检索时间): "
	            << toatal_query_time/CLOCKS_PER_SEC/35.0 << " s\n";
	/*
		CPU time used:(35 query 检索时间): 4.81858 s
		CPU time used:(平均检索时间): 0.137674 s

	*/            
	return 0;
}