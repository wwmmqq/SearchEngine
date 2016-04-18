#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <unistd.h>
#include <unordered_set>
#include <unordered_map>
#include <algorithm>
#include <ctime>

#include "base_function.cc"
#include "mytrietree.cc"
#include "BM25.cc"


using std::string; using std::endl; using std::cout;

std::unordered_set<string> STOPWRODS;
//std::unordered_set<unsigned int> SearchResultSet;
//std::unordered_set<string> Dict;
unsigned int token_nu = 0;


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
        	if (!find_stop_words(sub)) {
        		token_nu++;
        		text_token_len++;
        		auto search_map = Document.find(sub);
        		if(search_map != Document.end()) {
        			search_map->second++;
        		} else {
        			Document.emplace(sub, 1);
        		}
        	}
        }
    } while (iss);

    for (const auto &p : Document) {
    	//std::cout << p.first << " : " << p.second << "\n";
    	myTree.insertWord(p.first, docid, p.second, text_token_len);
    	//Dict.emplace(p.first);
    }

    //cout << "*********** " << token_nu << endl;
    //cout << "current word is : " <<myTree.getWordsCnt() <<endl;
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


    std::clock_t c_start1 = std::clock();

	for (unsigned int text_id = 0; text_id <= 101710; text_id++) {

		std::string filename = std::string("report") + std::to_string(text_id) + std::string(".txt");

		cout << filename<<"\n";
		std::string text = read_file_text(filename);
		if(text.size() != 0) {
			documentToken(text_id, text, myTree);
		}
		//cout << "66666666666666666"<<endl;
    	//int ok;
   		//std::cin>>ok;
	}
	std::clock_t c_end1 = std::clock();
	cout<<"CPU time used(索引建立时间): "
            << 1000.0*1000.0 * (c_end1-c_start1) / CLOCKS_PER_SEC << " s\n";

	//cout << "\n Dict total word is : " <<  Dict.size() << endl;// total word is : 48932
	cout << "\n Trie total word is : " <<myTree.getWordsCnt() <<endl;

	//////////////// test trie ///////////////////////////////////////
	/*
	std::unordered_set<unsigned int> sr[3];
	std::unordered_set<unsigned int> SearchResultSet1;
	std::unordered_set<unsigned int> SearchResultSet2;
	std::unordered_set<unsigned int> SearchResultSet3;
	sr[0] = SearchResultSet1;
	sr[1] = SearchResultSet2;
	sr[2] = SearchResultSet3;
	*/

	node_ptr result_ptr[10];

	std::string myin;
	int search_id = 0;
	while(std::cin >> myin) {
	    if(myin == "stop")
	        break;

	    node_ptr search_p = myTree.search(myin);
	    result_ptr[search_id] = search_p;

	    if (search_p != nullptr) {
	    	cout<< "ok" <<endl;

	    	std::cout <<"sum of doc: " <<search_p->DOCID.size() << std::endl;

	    	int i = 0;
	    	for(auto it = search_p->DOCID.begin(); it !=  search_p->DOCID.end(); ++it) {
	    		if(i++ < 10)
	    			cout<<"Top 10 id: "<< i<<" " << myin << " in : "<< it->first 
	    					<< " freq: " << it->second->getFreq()
	    					<< " doc_len: " << it->second->getDoc_len() << "\n";
	    		//sr[search_id].emplace(it->first);
	    	}
	    }
	    else
	    	cout << "no" <<endl;

	    search_id++;
	}
	/*
	cout<< "all : " << sr[0].size() << "\n";
	cout<< "all : " << sr[1].size() << "\n";
	cout<< "all : " << sr[2].size() << "\n";

	std::unordered_set<unsigned int> ggdd;
	set_intersection(sr[0].begin(), sr[0].end(), sr[1].begin(), sr[1].end(), std::inserter(ggdd,ggdd.end()));

	cout<< "after vintersection all : " << ggdd.size() << "\n";
	*/

	std::unordered_map<unsigned int, double> DocOutSet;
	for (int i = 0; i < search_id; i++) {
		double avgdl = 0.0;
		unsigned int tf = 0;
		unsigned int D_N = result_ptr[i]->DOCID.size();

		for(auto it = result_ptr[i]->DOCID.begin(); it !=  result_ptr[i]->DOCID.end(); ++it) {
			avgdl += it->second->getDoc_len();
		}
		avgdl = avgdl / D_N;

		for(auto it = result_ptr[i]->DOCID.begin(); it !=  result_ptr[i]->DOCID.end(); ++it) {
			tf = it->second->getFreq();
			double di_score = BM25OneTokenScore(tf, D_N ,avgdl);

			unsigned int doc_id = it->first;
			auto find_di = DocOutSet.find(doc_id);
    		if(find_di != DocOutSet.end()) {
    			find_di->second += di_score;
    		} else {
    			DocOutSet.emplace(doc_id, di_score);
    		}//end if
		}
	}//end first for

	std::unordered_map<double, unsigned int> dst = flip_map(DocOutSet);
	int dst_i = 1;
	for (auto it = dst.begin(); it != dst.end(); ++it) {
		if (dst_i++ > 10)
			break;
		string sline = string("report") + std::to_string(it->second);
		cout << sline << " " << it->first << "\n";
	}

	std::clock_t c_start2 = std::clock();
	std::clock_t c_end2 = std::clock();
	cout<<"CPU time used:(检索时间): "
            << 1000.0*1000.0 * (c_end2-c_start2) / CLOCKS_PER_SEC << " s\n"
	return 0;
}

/******** base line**********
预处理时间:4014s
预处理后数据集大小:147MB (原来298MB)
索引建立时间:519s
索引文件大小:143MB
检索时间:
	共:441s
	平均:12.6s

P@10:0.3911
Map:0.3240
****************************/
