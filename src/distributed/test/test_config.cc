#include <iostream>
#include "../src/config.h"
using namespace std;

int main(int argc, char const *argv[])
{
	Config mycfg("myconfig.cfg");
	mycfg.Print();
	std:: cout << "getvalue port: " << mycfg.getvalue<int>("port") << std::endl;
	return 0;
}

