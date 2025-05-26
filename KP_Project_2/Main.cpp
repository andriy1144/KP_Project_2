#include <iostream>
#include <Windows.h>

#include "LibrarySystem.hpp"
#include "ConsoleUtils.hpp"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251); // Українська локалізація консолі

    try {
        LibrarySystem librarySystem;		
		librarySystem.run();
		return 0;
	}
	catch (const std::exception& e) {
		ConsoleUtils::printError("Помилка: " + std::string(e.what()));
		ConsoleUtils::printError("Перезапустіть програму та спробуйте знову.");
	}
}