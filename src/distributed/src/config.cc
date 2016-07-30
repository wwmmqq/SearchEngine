#include <fstream>
#include "config.h"

Config::Config(std::string filepath)
{
	if(!load(filepath)) {
		std::cerr << "config file: "<< filepath << "not find !" << std::endl;
	}
}

bool Config::load(std::string filepath)
{
	this->filepath = filepath;
	std::fstream fs;
	fs.open(filepath.c_str());
	if(!fs.is_open()) {
		return false;
	}

	std::string line;
	int line_nu = -1;
	std::string domain;
	while(getline(fs, line)) {
		line_nu++;
		if(!line.size())
			continue;
		if('#' == line[0])
			continue;

		if('[' == line[0]) {
			domain = line;
			continue;
		}

		int pos = line.find('=');
		std::string key = line.substr(0, pos);//String containing the substring [pos, pos+count)
		//1.  Removes the character at position.
		//2. Removes the characters in the range [first, last).
		key.erase(std::find(key.begin(), key.end(), ' '));// Erase ' '
		for (auto &c: key) c = tolower(c);

		std::string value = line.substr(pos+1);
		value.erase(std::find(value.begin(), value.end(), ' '));// Erase ' '
		for (auto &c: value) c = tolower(c);
		this->_data_map[key] = value;
	}
	fs.close();
	return true;
}

void Config::Print()
{
	for(const auto &p: this->_data_map)
		std::cout << p.first << " : " << p.second << std::endl;
}

