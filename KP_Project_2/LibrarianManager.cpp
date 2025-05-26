#include "LibrarianManager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "ConsoleUtils.hpp"

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"

const std::string LibrarianManager::FILE_NAME = "librarians.txt";

LibrarianManager::LibrarianManager() : _nextLibrarianId(1), _fileManager(FILE_NAME) {
	loadLibrarians();
}
LibrarianManager::~LibrarianManager() = default;

void LibrarianManager::loadLibrarians() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "Помилка відкриття файлу(файл незнайдений/недоступний)");
	
	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, libId, hallId;
			std::string firstName, lastName, email, phoneNumber,
				address, city, age;
			std::getline(ss, id, '|');
			std::getline(ss, libId, '|');

			std::getline(ss, firstName, '|');
			std::getline(ss, lastName, '|');
			std::getline(ss, email, '|');
			std::getline(ss, phoneNumber, '|');
			std::getline(ss, address, '|');
			std::getline(ss, city, '|');
			std::getline(ss, age, '|');

			std::getline(ss, hallId, '|');

			int librarianId = std::stoi(id);
			int libraryId = std::stoi(libId);
			int ageInt = std::stoi(age);

			auto librarian = std::make_shared<Librarian>(librarianId, libraryId, 
				firstName, lastName,email,phoneNumber,address,city, ageInt,hallId);

			_librarians.push_back(librarian);

			if (librarianId >= _nextLibrarianId) {
				_nextLibrarianId = librarianId + 1;
			}
		}
		catch (...) {
			ConsoleUtils::printError("Помилка під час зчитування рядка файлу " + FILE_NAME);
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_librarians.size()) + " бібліотекарів завантажено з файлу.");
	file.close();
}

std::shared_ptr<Librarian> LibrarianManager::getLibrarianById(int id) const {
	for(const auto& librarian : _librarians) {
		if (librarian->getId() == id) {
			return librarian;
		}
	}
	throw EntityNotFoundException("Бібліотекар", "Бібліотекаря з id - " + std::to_string(id) + " не знайдено!");
}

int LibrarianManager::addLibrarian(const std::string& firstName, 
	const std::string& lastName, const std::string& email, const std::string& phoneNumber,
	const std::string& address, const std::string& city, const int& age,
	const std::string& hallId, const int& libId) {
	auto librarian = std::make_shared<Librarian>(_nextLibrarianId, libId, 
		firstName, lastName,email,phoneNumber,address,city,age, hallId);
	_librarians.push_back(librarian);
	try {
		_fileManager.saveToFile(*librarian);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(std::vector<std::shared_ptr<Serializable>>(_librarians.begin(), _librarians.end()));
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
		}
	}
	return _nextLibrarianId++;
}

void LibrarianManager::deleteLibrarianById(int id) {
	for (auto it = _librarians.begin(); it != _librarians.end(); ++it) {
		if ((*it)->getId() == id) {
			_librarians.erase(it);
			
			try {
				_fileManager.updateFile(
					std::vector<std::shared_ptr<Serializable>>(
						_librarians.begin(), _librarians.end())
				);
			}
			catch (FileException e) {
				ConsoleUtils::printError(e.what());
				ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
			}
			return;
		}
	}
	throw EntityNotFoundException("Бібліотекарі", "Бібліотекаря з id - " + std::to_string(id) + " не знайдено!");
}

int LibrarianManager::printLibrarianMenu() {
	if (_librarians.empty()) throw std::runtime_error("Список бібліотекарів порожній!");

	std::cout << "Меню бібліотекарів:\n";
	for (const auto& librarian : _librarians) {
		std::cout << librarian->toString() << std::endl;
	}

	int id = ConsoleUtils::getInputInt("Виберіть ID бібліотекарів із верхнього списку:");

	return getLibrarianById(id)->getId();
}

std::vector<std::shared_ptr<Librarian>> LibrarianManager::getLibrarians() const {
	return _librarians;
}