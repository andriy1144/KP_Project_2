#pragma once

#include <string>
#include <vector>
#include <memory>

#include "EntityObject.hpp"

class ConsoleUtils {
private:
	static void clearInputBuffer();
public:
	static std::string getInputString(const std::string& prompt);
	static int getInputInt(const std::string& prompt);
	static float getInputFloat(const std::string& prompt);

	static void printSuccess(const std::string& message);
	static void printError(const std::string& message);
	static void printWarning(const std::string& message);
	static void printInfo(const std::string& message);

	static bool printConfirmation(const std::string& message);
	static void printArrayFormatted(const std::vector<std::shared_ptr<EntityObject>>& data, const char& delimeter, int len);
	static void printDelimeter(const char& delimeter, int len);
};