#pragma once

#include<string>
#include<vector>

class StringUtils {
public:
	static std::string trim(std::string str);
	static std::vector<std::string> split(const std::string& str, char delimiter);
};