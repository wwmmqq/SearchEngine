#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <sstream>
#include <string>
#include <map>

class Config
{
private:
	std::string filepath;
	std::map<std::string, std::string> _data_map;

public:
	Config(std::string filepath);
	~Config(){}

	bool load(std::string /*filepath*/);

	template<typename T>
	T getvalue(std::string key) {
		std::string str = _data_map[key];
		if(str == "") {
			std::cerr << "warning: " << key << " not defined in "<< filepath << " !"<< std::endl; 
		}
		std::stringstream ss;
		ss << str;
		T value;
		ss >> value;
		return value;	
	}

	void Print();
};

#include "config.cc"
#endif