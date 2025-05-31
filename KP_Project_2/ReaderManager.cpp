#include "ReaderManager.hpp"

#include <fstream>
#include <sstream>
#include <iostream>

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"

#include "ConsoleUtils.hpp"
#include "StringUtils.hpp"

const std::string ReaderManager::FILE_NAME = "readers.txt";

ReaderManager::ReaderManager() : _nextReaderId(1), _fileManager(FILE_NAME) {
	loadReaders();
}
ReaderManager::~ReaderManager() = default;

void ReaderManager::loadReaders() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "Помилка відкриття файлу(файл незнайдений/недоступний)");

	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, category, libraryId, subId, booksCount;
			std::string firstName, lastName, email, phoneNumber,
				address, city, age;
			std::getline(ss, id, '|');
			std::getline(ss, firstName, '|');
			std::getline(ss, lastName, '|');
			std::getline(ss, email, '|');
			std::getline(ss, phoneNumber, '|');
			std::getline(ss, address, '|');
			std::getline(ss, city, '|');
			std::getline(ss, age, '|');

			std::getline(ss, category, '|');
			std::getline(ss, libraryId, '|');
			std::getline(ss, subId, '|');
			std::getline(ss, booksCount, '|');

			int readerId = std::stoi(id);
			int regLibraryId = std::stoi(libraryId);
			int subscriptionId = std::stoi(subId);
			int booksCountInt = std::stoi(booksCount);
			int ageInt = std::stoi(age);

			std::string attributes;
			std::getline(ss, attributes, '|');
			
			std::vector < std::string > stringAttrs = StringUtils::split(attributes, ';');
			std::vector<std::pair<std::string, std::string>> attrs;
			for(const auto& attr : stringAttrs) {
				if (!attr.empty()) {
					std::string key = attr.substr(0, attr.find(':'));
					std::string value = attr.substr(attr.find(':') + 1);
					attrs.push_back({ key, value });
				}
			}

			auto reader = std::make_shared<Reader>(readerId, 
				firstName,lastName,email,phoneNumber,address,city,ageInt, 
				category, regLibraryId,subscriptionId, attrs);
			reader->setBooksCount(booksCountInt);

			_readers.push_back(reader);
			if (readerId >= _nextReaderId) {
				_nextReaderId = readerId + 1;
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError("Помилка під час зчитування рядка файлу " + FILE_NAME);
			ConsoleUtils::printError(e.what());
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_readers.size()) + " читачів завантажено з файлу.");
	file.close();
}

int ReaderManager::addReader(const std::string firstName,
	const std::string lastName, const std::string email,
	const std::string phoneNumber, const std::string address,
	const std::string city, int age,
	const std::string& category, int libraryId, int subscriptionId,
	const std::vector<std::pair<std::string, std::string>>& attributes) {
	const auto& reader = std::make_shared<Reader>(_nextReaderId,
		firstName, lastName, email, phoneNumber,address,city,age,
		category, libraryId,subscriptionId, attributes);

	_readers.push_back(reader);
	try {
		_fileManager.saveToFile(*reader);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(
				std::vector<std::shared_ptr<Serializable>>(
					_readers.begin(), _readers.end())
			);
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
		}
	}
	return _nextReaderId++;
}

void ReaderManager::changeReaderBookCount(int id, int count) {
	const auto& reader = getReaderById(id);
	reader->setBooksCount(reader->getBooksCount() + count);
}

void ReaderManager::deleteReaderById(int id) {
	for (auto it = _readers.begin(); it != _readers.end(); ++it) {
		if ((*it)->getId() == id) {
			_readers.erase(it);
			try {
				_fileManager.updateFile(
					std::vector<std::shared_ptr<Serializable>>(
						_readers.begin(), _readers.end())
				);
			}
			catch (FileException e) {
				ConsoleUtils::printError(e.what());
				ConsoleUtils::printWarning("Останні зміни не було збережено у файл!!! Для уникнення втрати даних перезапустіть систему та спробуйте знову");
			}
			return; 
		}
	}
	throw EntityNotFoundException("Читач", "Читача з id - " + std::to_string(id) + " не знайдено!");
}

bool ReaderManager::isActiveReadersByLibraryId(int libraryId) const {
	for (const auto& reader : _readers) {
		if (reader->getLibId() == libraryId) {
			return true;
		}
	}
	return false;
}

bool ReaderManager::isActiveReaderBySubscriptionId(int subscriptionId) const {
	for (const auto& reader : _readers) {
		if (reader->getSubscriptionId() == subscriptionId) {
			return true;
		}
	}
	return false;
}

int ReaderManager::printReadersMenu() {
	if (_readers.empty()) throw std::runtime_error("Список читачів порожній!");
	
	std::cout << "Меню читачів: " << std::endl;
	for (const auto& reader : _readers) {
		std::cout << reader->toString();
		ConsoleUtils::printDelimeter('-', 150);
	}

	int id = ConsoleUtils::getInputInt("Виберіть номер читача зі списку вгорі: ");
	
	return getReaderById(id)->getId();
}

std::shared_ptr<Reader> ReaderManager::getReaderById(int id) const {
	for (const auto& reader : _readers) {
		if (reader->getId() == id) {
			return reader;
		}
	}
	throw EntityNotFoundException("Читач", "Читача з id - " + std::to_string(id) + " не знайдено!");
}

std::pair<int, int> ReaderManager::getReaderBookCountAndSubscriptionId(int readerId) const {
	const auto& reader = getReaderById(readerId);
	return { reader->getBooksCount(), reader->getSubscriptionId() };
}

std::vector<std::shared_ptr<Reader>> ReaderManager::getReaders() const {
	return _readers;
}