#include "StringUtils.hpp"

#include <vector>
#include <sstream>
#include <locale>

std::string StringUtils::trim(std::string str) {
    str.erase(str.find_last_not_of(" \n\r\t") + 1);
    str.erase(0, str.find_first_not_of(" \n\r\t"));
    return str;
}

std::vector<std::string> StringUtils::split(const std::string& str, char delimiter) {
    std::vector<std::string> tokens;
    std::istringstream tokenStream(str);
    std::string token;
    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(trim(token));
    }
    return tokens;
}

bool StringUtils::hasSymbols(const std::string& str, const std::string& symbols) {
    for (char c : str) {
        if (symbols.find(c) != std::string::npos) {
            return true;
        }
    }
    return false;
}