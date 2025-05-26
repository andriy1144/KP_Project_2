#include "LiteratureManager.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "FileException.hpp"
#include "EntityNotFoundException.hpp"

#include "ConsoleUtils.hpp"

const std::string LiteratureManager::FILE_NAME = "literature.txt";

LiteratureManager::LiteratureManager() : 
	_nextLiteratureId(1), _fileManager(FILE_NAME) {
	loadLiterature();
}
LiteratureManager::~LiteratureManager() = default;

void LiteratureManager::loadLiterature() {
	std::ifstream file(FILE_NAME);
	if (!file.is_open()) throw FileException(FILE_NAME, "������� �������� �����(���� �����������/�����������)");

	std::string line;
	while (std::getline(file, line)) {
		try {
			std::stringstream ss(line);
			std::string id, libId, title, author, type, inventoryNumber,
				location, isReadingRoomOnly, issuePeriodDays,
				entryDate, isArchived, archiveDate;

			std::getline(ss, id, '|');
			std::getline(ss, libId, '|');
			std::getline(ss, title, '|');
			std::getline(ss, author, '|');
			std::getline(ss, type, '|');

			addLiteratureType(type);

			std::getline(ss, inventoryNumber, '|');
			std::getline(ss, location, '|');
			std::getline(ss, isReadingRoomOnly, '|');
			std::getline(ss, issuePeriodDays, '|');
			std::getline(ss, entryDate, '|');
			std::getline(ss, isArchived, '|');
			std::getline(ss, archiveDate, '|');

			int litId = std::stoi(id);
			int libraryId = std::stoi(libId);

			auto lit = std::make_shared<Literature>(litId, libraryId, title, author, type, inventoryNumber, location,
				isReadingRoomOnly == "1", std::stoi(issuePeriodDays), std::stoll(entryDate));
			
			lit->setArchived(isArchived == "1", std::stoll(archiveDate));

			_literature.push_back(std::move(lit));
			if (litId >= _nextLiteratureId) {
				_nextLiteratureId = litId + 1;
			}
		}
		catch (const std::exception& e) {
			ConsoleUtils::printError("������� �� ��� ���������� ����� ����� " + FILE_NAME);
			ConsoleUtils::printError(e.what());
		}
	}

	ConsoleUtils::printSuccess(std::to_string(_literature.size()) + " ��������� ����������� � �����.");

	file.close();
}

std::shared_ptr<Literature> LiteratureManager::getLiteratureById(int id) const {
	for(const auto& lit : _literature) {
		if (lit->getId() == id) {
			return lit;
		}
	}
	throw EntityNotFoundException("˳��������", "˳�������� � id - " + std::to_string(id) + " �� ��������!");
}

int LiteratureManager::addLiterature(const std::string& title, const std::string& author, const std::string& type,
	const std::string& inventoryNumber, const std::string& location, bool isReadingRoomOnly,
	int libraryId, int issuePeriodDays) {
	
	auto lit = std::make_shared<Literature>(_nextLiteratureId, libraryId, title, author, type, inventoryNumber, location,
		isReadingRoomOnly == 1, issuePeriodDays, time(nullptr));
	addLiteratureType(lit->getType());
	
	_literature.push_back(lit);
	try {
		_fileManager.saveToFile(*lit);
	}
	catch (FileException e) {
		ConsoleUtils::printError(e.what());
		try {
			_fileManager.updateFile(
				std::vector<std::shared_ptr<Serializable>>(
					_literature.begin(), _literature.end())
			);
		}
		catch (FileException e) {
			ConsoleUtils::printError(e.what());
			ConsoleUtils::printWarning("������ ���� �� ���� ��������� � ����!!! ��� ��������� ������ ����� ������������ ������� �� ��������� �����");
		}
	}
	return _nextLiteratureId++;
}

void LiteratureManager::archiveLiteratureById(int id) {
	for (auto& lit : _literature) {
		if (lit->getId() == id) {
			lit->setArchived(true, time(nullptr));
			_fileManager.updateFile(std::vector<std::shared_ptr<Serializable>>(
				_literature.begin(), _literature.end()));
			break;
		}
	}
}

void LiteratureManager::addLiteratureType(const std::string& type) {
	if (std::find(_literatureTypes.begin(), _literatureTypes.end(), type) == _literatureTypes.end()) {
		_literatureTypes.push_back(type);
	}
}

bool LiteratureManager::checkLiteratureIssueDaysValid(int id, int issuePeriodDays) const {
	const auto& lit = getLiteratureById(id);
	return lit->getIssuePeriodDays() >= issuePeriodDays;
}

int LiteratureManager::printLiteratureMenu() {
	if (_literature.empty()) throw std::runtime_error("˳�������� ����!");

	std::cout << "���� ���������:" << std::endl;
	for (const auto& lit : _literature) {
		std::cout << lit->toString() << std::endl;
	}

	int id = ConsoleUtils::getInputInt("������ ����� ��������� � ��������� ������:");

	return getLiteratureById(id)->getId();
}

void LiteratureManager::printLiteratureTypes() const {
	std::cout << "����� ���� ���������(�������):" << std::endl;
	for (const auto& type : _literatureTypes) {
		std::cout << "- " << type << std::endl;
	}
}

std::vector<std::shared_ptr<Literature>> LiteratureManager::getLiterature() const {
	return _literature;
}