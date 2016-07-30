#ifndef KV_STORE_H
#define KV_STORE_H

#include <unordered_map>
#include <string>

class KV {
private:
	std::unordered_map<std::string, std::string> _kv_map;
public:
	KV(){};
	~KV(){};
	void set_key(std::string key, std::string value)
	{
		_kv_map[key] = value;
		return;
	}

	std::string get_value(std::string key)
	{
		std::string value;
		try {
			value = _kv_map.at(key);
		} catch(std::out_of_range) {
			value = "null";
		}
		return value;
	}
};

#endif