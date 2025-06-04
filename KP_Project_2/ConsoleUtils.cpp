#include "ConsoleUtils.hpp"

#include <iostream>

#include "StringUtils.hpp"

void ConsoleUtils::clearInputBuffer() {
	std::cin.clear();
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

std::string ConsoleUtils::getInputString(const std::string& prompt) {
	std::string input;
	
	do {
		std::cout << prompt;
		std::getline(std::cin, input);
		input = StringUtils::trim(input);

		if (input.empty()) {
			ConsoleUtils::printWarning("¬вед≥ть непорожн≥й р€док: ");
		}
		
		if (StringUtils::hasSymbols(input, "|;:")) {
			ConsoleUtils::printWarning("–€док не повинен м≥стити один ≥з цих символ≥в '|', ':', ';': ");
		}

	} while (input.empty()
		|| StringUtils::hasSymbols(input, "|;:"));

	return input;
}

int ConsoleUtils::getInputInt(const std::string& prompt) {
	int value;
	std::cout << prompt;
	while (!(std::cin >> value)) {
		std::cout << "¬вед≥ть д≥йсне ц≥ле число: ";
		clearInputBuffer();
	}
	clearInputBuffer();
	return value;
}

float ConsoleUtils::getInputFloat(const std::string& prompt) {
	float value;
	std::cout << prompt;
	while (!(std::cin >> value)) {
		std::cout << "¬вед≥ть д≥йсне число: ";
		clearInputBuffer();
	}
	clearInputBuffer();
	return value;
}

void ConsoleUtils::printSuccess(const std::string& message) {
	std::cout << "\033[32m" << message << "\033[0m" << std::endl;
}

void ConsoleUtils::printError(const std::string& message) {
	std::cout << "\033[31m" << message << "\033[0m" << std::endl;
}

void ConsoleUtils::printWarning(const std::string& message) {
	std::cout << "\033[33m" << message << "\033[0m" << std::endl;
}

bool ConsoleUtils::printConfirmation(const std::string& message) {
	std::cout << "\x1B[35m" << message << "\033[0m" << std::endl;
	std::string input = getInputString("¬вед≥ть 'так' або 'н≥': ");
	return input == "так";
}

void ConsoleUtils::printInfo(const std::string& message) {
	std::cout << "\033[34m" << message << "\033[0m" << std::endl;
}

void ConsoleUtils::printArrayFormatted(const std::vector<std::shared_ptr<EntityObject>>& data, const char& delimeter, int len) {
	for (const auto& entity : data) {
		std::cout << entity->toString();
		printDelimeter(delimeter, len);
	}
}

void ConsoleUtils::printDelimeter(const char& delimeter, int len) {
	std::cout << "\n" << std::string(len, delimeter) << "\n";
}