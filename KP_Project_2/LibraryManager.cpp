#include "LibraryManager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"

#include "ConsoleUtils.hpp"
#include "StringUtils.hpp"

const std::string LibraryManager::FILE_NAME = "libraries.txt";

LibraryManager::LibraryManager()
	: _nextLibraryId(1), _fileManager(FILE_NAME) {
	loadLibraries();
}
LibraryManager::~LibraryManager() = default;

void LibraryManager::loadLibraries() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "Помилка відкриття файлу(файл незнайдений/недоступний)");
	
	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, name, address, halls;
			std::getline(ss, id, '|');
			std::getline(ss, name, '|');
			std::getline(ss, address, '|');
			std::getline(ss, halls, '|');
			
			int libraryId = std::stoi(id);
			std::vector<std::string> hallsList = StringUtils::split(halls, ';');

			auto library = std::make_shared<Library>(libraryId, name, address, hallsList);

			_libraries.push_back(library);
			if (libraryId >= _nextLibraryId) {
				_nextLibraryId = libraryId + 1;
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError("Помилка під час зчитування рядка файлу " + FILE_NAME);
			ConsoleUtils::printError(e.what());
		}
	}

	std::cout << "\x1B[32m" << _libraries.size() << " бібліотеки завантажено з файлу.\033[0m\t\t" << std::endl;

	file.close();
}

int LibraryManager::addLibrary(const std::string& name, const std::string& address, const std::string& halls) {
	std::vector<std::string> hallsList = StringUtils::split(halls,',');

	auto library = std::make_shared<Library>(_nextLibraryId, name, address, hallsList);

	_libraries.push_back(library);

	try {
		_fileManager.saveToFile(*library);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(
				std::vector<std::shared_ptr<Serializable>>(
				_libraries.begin(), _libraries.end())
			);
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
		}
	}
	return _nextLibraryId++;
}

void LibraryManager::deleteLibraryById(int id) {
	for (auto it = _libraries.begin(); it != _libraries.end(); ++it) {
		if ((*it)->getId() == id) {
			_libraries.erase(it);
			try {
				_fileManager.updateFile(
					std::vector<std::shared_ptr<Serializable>>(
					_libraries.begin(), _libraries.end())
				);
			}
			catch (FileException e) {
				ConsoleUtils::printError(e.what());
				ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
			}
			return;
		}
	}
	throw EntityNotFoundException("Бібліотека", "Біблотеку з id - " + std::to_string(id) + " не знайдено!");
}

std::shared_ptr<Library> LibraryManager::getLibraryById(int id) const {
	for (const auto& library : _libraries) {
		if (library->getId() == id) {
			return library;
		}
	}
	throw EntityNotFoundException("Бібліотека", "Біблотеку з id - " + std::to_string(id) + " не знайдено!");
}

int LibraryManager::printLibraryMenu() {
	if (_libraries.empty()) throw std::runtime_error("Список бібліотек порожній!");
	
	std::cout << "Меню бібліотек: " << std::endl;
	for (const auto& library : _libraries) {
		std::cout << library->toString() << std::endl;
	}

	int id = ConsoleUtils::getInputInt("Введіть номер бібліотеки з верхнього списку: ");

	return getLibraryById(id)->getId();
}

std::string LibraryManager::printHallsByLibraryId(int libraryId) const {
	for (const auto& library : _libraries) {
		if (library->getId() == libraryId) {
			const auto hallsLitst = library->getHalls();
			
			std::cout << "Зали у бібліотеці " << library->getName() << ":" << std::endl;
			if (hallsLitst.empty()) throw std::runtime_error("Доступні читальні зали бібліотеки відсутні.");

			for (const auto& hall : hallsLitst) {
				std::cout << " - " << hall << std::endl;
			}

			std::string hallId = ConsoleUtils::getInputString("Введіть ідентифікатор читальної зали із запропонованих зверху: ");

			for(const auto& hall : hallsLitst) {
				if (hall == hallId) {
					return hallId;
				}
			}
				
			throw EntityNotFoundException("Читальний зал", "Читальний зал з id - " + hallId + " не знайдено!");
		}
	}
	throw EntityNotFoundException("Бібліотека", "Біблотеку з id - " + std::to_string(libraryId) + " не знайдено!");
}

std::vector<std::shared_ptr<Library>> LibraryManager::getLibraries() const{
		return _libraries;
}