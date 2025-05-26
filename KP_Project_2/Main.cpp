#include <iostream>
#include <Windows.h>

#include "LibrarySystem.hpp"
#include "ConsoleUtils.hpp"

int main() {
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251); // ��������� ���������� ������

    try {
        LibrarySystem librarySystem;		
		librarySystem.run();
		return 0;
	}
	catch (const std::exception& e) {
		ConsoleUtils::printError("�������: " + std::string(e.what()));
		ConsoleUtils::printError("������������ �������� �� ��������� �����.");
	}
}